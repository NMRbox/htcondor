/***************************************************************
 *
 * Copyright (C) 1990-2007, Condor Team, Computer Sciences Department,
 * University of Wisconsin-Madison, WI.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You may
 * obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************/


#include "condor_common.h"

#include "condor_daemon_core.h"
#include "condor_config.h"
#include "globus_utils.h"
#include "grid_universe.h"
#include "directory.h"
#include "MyString.h"
#include "condor_ver_info.h"
#include "condor_attributes.h"
#include "exit.h"
#include "HashTable.h"
#include "condor_uid.h"
#include "condor_email.h"
#include "shared_port_endpoint.h"

// Initialize static data members
const int GridUniverseLogic::job_added_delay = 3;
const int GridUniverseLogic::job_removed_delay = 2;
GridUniverseLogic::GmanPidTable_t * GridUniverseLogic::gman_pid_table = NULL;
int GridUniverseLogic::rid = -1;
static const char scratch_prefix[] = "condor_g_scratch.";

// globals
GridUniverseLogic* _gridlogic = NULL;


GridUniverseLogic::GridUniverseLogic() 
{
	// This class should only be instantiated once
	ASSERT( gman_pid_table == NULL );

	// Make our hashtable
	gman_pid_table = new GmanPidTable_t(hashFunction);

	// Register a reaper for this grid managers
	rid = daemonCore->Register_Reaper("GManager",
		 &GridUniverseLogic::GManagerReaper,"GManagerReaper");

	// This class should register a reaper after the regular schedd reaper
	ASSERT( rid > 1 );

	return;
}

GridUniverseLogic::~GridUniverseLogic()
{
	if ( gman_pid_table) {
		gman_node_t * node;
		gman_pid_table->startIterations();
		while (gman_pid_table->iterate( node ) == 1 ) {
			if (daemonCore) {
				if ( node->add_timer_id >= 0 ) {
					daemonCore->Cancel_Timer(node->add_timer_id);
				}
				if ( node->remove_timer_id >= 0 ) {
					daemonCore->Cancel_Timer(node->remove_timer_id);
				}
				if ( node->pid > 0 ) {
					daemonCore->Send_Signal( node->pid, SIGQUIT );
				}
			}
			delete node;
		}
		delete gman_pid_table;
		rid = 0;
	}
	gman_pid_table = NULL;
	return;
}

void 
GridUniverseLogic::JobCountUpdate(const char* owner, const char* domain,
	   	const char* attr_value, const char* attr_name, int cluster, int proc, 
		int num_globus_jobs, int num_globus_unmanaged_jobs)
{
	// Quick sanity checks - this should never be...
	ASSERT( num_globus_jobs >= num_globus_unmanaged_jobs );
	ASSERT( num_globus_jobs >= 0 );
	ASSERT( num_globus_unmanaged_jobs >= 0 );

	// if there are unsubmitted jobs, the gridmanager apparently
	// does not know they are in the queue. so tell it some jobs
	// were added.
	if ( num_globus_unmanaged_jobs > 0 ) {
		JobAdded(owner, domain, attr_value, attr_name, cluster, proc);
		return;
	}

	// now that we've taken care of unsubmitted globus jobs, see if there 
	// are any globus jobs at all.  if there are, make certain that there
	// is a grid manager watching over the jobs and start one if there isn't.
	if ( num_globus_jobs > 0 ) {
		StartOrFindGManager(owner, domain, attr_value, attr_name, cluster, proc);
		return;
	}

	// if made it here, there is nothing we have to do
	return;
}


void 
GridUniverseLogic::JobAdded(const char* owner, const char* domain,
	   	const char* attr_value, const char* attr_name, int cluster, int proc)
{
	gman_node_t* node;

	node = StartOrFindGManager(owner, domain, attr_value, attr_name, cluster, proc);

	if (!node) {
		// if we cannot find nor start a gridmanager, there's
		// nobody to notify.
		return;
	}

	// start timer to signal gridmanager if we haven't already
	if ( node->add_timer_id == -1 ) {  // == -1 means no timer set
		node->add_timer_id = daemonCore->Register_Timer(job_added_delay,
			GridUniverseLogic::SendAddSignal,
			"GridUniverseLogic::SendAddSignal");
		daemonCore->Register_DataPtr(node);
	}
	

	return;
}

void 
GridUniverseLogic::JobRemoved(const char* owner, const char* domain,
	   	const char* attr_value, const char* attr_name,int cluster, int proc)
{
	gman_node_t* node;

	node = StartOrFindGManager(owner, domain, attr_value, attr_name, cluster, proc);

	if (!node) {
		// if we cannot find nor start a gridmanager, there's
		// nobody to notify.
		return;
	}

	// start timer to signal gridmanager if we haven't already
	if ( node->remove_timer_id == -1 ) {  // == -1 means no timer set
		node->remove_timer_id = daemonCore->Register_Timer(job_removed_delay,
			GridUniverseLogic::SendRemoveSignal,
			"GridUniverseLogic::SendRemoveSignal");
		daemonCore->Register_DataPtr(node);
	}

	return;
}

void
GridUniverseLogic::SendAddSignal()
{
	// This method is called via a DC Timer set in JobAdded method

	// First get our gridmanager node
	gman_node_t * node = (gman_node_t *)daemonCore->GetDataPtr();
	ASSERT(node);

	// Record in the node that there is no outstanding timer set anymore
	node->add_timer_id = -1;

	// Signal the gridmanager
	if ( node->pid ) {
		daemonCore->Send_Signal(node->pid,GRIDMAN_ADD_JOBS);
	}
}

void
GridUniverseLogic::SendRemoveSignal()
{
	// This method is called via a DC Timer set in JobRemoved method

	// First get our gridmanager node
	gman_node_t * node = (gman_node_t *)daemonCore->GetDataPtr();
	ASSERT(node);

	// Record in the node that there is no outstanding timer set anymore
	node->remove_timer_id = -1;

	// Signal the gridmanager
	if ( node->pid ) {
		daemonCore->Send_Signal(node->pid,GRIDMAN_REMOVE_JOBS);
	}
}

void
GridUniverseLogic::signal_all(int sig)
{
	// Iterate through our entire table and send the desired sig

	if (gman_pid_table) {
		gman_node_t* tmpnode;
		gman_pid_table->startIterations();
		while ( gman_pid_table->iterate(tmpnode) ) {
			if (tmpnode->pid) {
				daemonCore->Send_Signal(tmpnode->pid,sig);
			}
		}
	}
}


// Note: caller must deallocate return value w/ delete []
const char *
GridUniverseLogic::scratchFilePath(gman_node_t *gman_node, std::string & path)
{
	std::string filename;
	formatstr(filename,"%s%p.%d",scratch_prefix,
					gman_node,daemonCore->getpid());
	auto_free_ptr prefix(temp_dir_path());
	ASSERT(prefix);
	return dircat(prefix,filename.c_str(),path);
}


int 
GridUniverseLogic::GManagerReaper(int pid, int exit_status)
{
	gman_node_t* gman_node = NULL;
	std::string owner;

	// Iterate through our table to find the node w/ this pid
	// Someday we should perhaps also hash on the pid, but we
	// don't expect gridmanagers to exit very often, and there
	// are not that many of them.

	if (gman_pid_table) {
		gman_node_t* tmpnode;
		gman_pid_table->startIterations();
		while ( gman_pid_table->iterate(owner,tmpnode) ) {
			if (tmpnode->pid == pid ) {
				// found it!
				gman_node = tmpnode;
				break;
			}
		}
	}

	std::string owner_safe;
	std::string exit_reason;
	if(gman_node) { owner_safe = owner; }
	else { owner_safe = "Unknown"; }
	if ( WIFEXITED( exit_status ) ) {
		formatstr( exit_reason, "with return code %d",
							 WEXITSTATUS( exit_status ) );
	} else {
		formatstr( exit_reason, "due to %s",
							 daemonCore->GetExceptionString( exit_status ) );
	}
	dprintf(D_ALWAYS, "condor_gridmanager (PID %d, owner %s) exited %s.\n",
			pid, owner_safe.c_str(), exit_reason.c_str() );
	if(WIFEXITED(exit_status) && WEXITSTATUS(exit_status) == DPRINTF_ERROR) {
		const char *condorUserName = get_condor_username();

		dprintf(D_ALWAYS, 
			"The gridmanager had a problem writing its log. "
			"Check the permissions of the file specified by GRIDMANAGER_LOG; "
			"it needs to be writable by Condor.\n");

			/* send email to the admin about this, but only
			 * every six hours - enough to not be ignored, but
			 * not enough to be a pest.  If only my children were
			 * so helpful and polite.  Ah, well, we can always dream...
			 */
		static time_t last_email_re_gridmanlog = 0;
		if ( time(NULL) - last_email_re_gridmanlog > 6 * 60 * 60 ) {
			last_email_re_gridmanlog = time(NULL);
			FILE *email = email_admin_open("Unable to launch grid universe jobs.");
			if ( email ) {
				fprintf(email,
					"The condor_gridmanager had an error writing its log file.  Check the  \n"
					"permissions/ownership of the file specified by the GRIDMANAGER_LOG setting in \n"
					"the condor_config file.  This file needs to be writable as user %s to enable\n"
					"the condor_gridmanager daemon to write to it. \n\n"
					"Until this problem is fixed, grid universe jobs submitted from this machine cannot "
					"be launched.\n", condorUserName ? condorUserName : "*unknown*" );
				email_close(email);
			} else {
					// Error sending an email message
				dprintf(D_ALWAYS,"ERROR: Cannot send email to the admin\n");
			}
		}	
	}	// end if(WIFEXITED(exit_status) && WEXITSTATUS(exit_status) == DPRINTF_ERROR)

	if (!gman_node) {
		// nothing more to do, so return
		return 0;
	}

	// Cancel any timers before removing the node!!
	if (gman_node->add_timer_id != -1) {
		daemonCore->Cancel_Timer(gman_node->add_timer_id);
	}
	if (gman_node->remove_timer_id != -1) {
		daemonCore->Cancel_Timer(gman_node->remove_timer_id);
	}
	// Remove node from our hash table
	gman_pid_table->remove(owner);
	// Remove any scratch directory used by this gridmanager
	std::string scratchdirbuf;
	const char *scratchdir = scratchFilePath(gman_node, scratchdirbuf);
	ASSERT(scratchdir);
	if ( IsDirectory(scratchdir) && 
		 init_user_ids(gman_node->owner, gman_node->domain) ) 
	{
		priv_state saved_priv = set_user_priv();
			// Must put this in braces so the Directory object
			// destructor is called, which will free the iterator
			// handle.  If we didn't do this, the below rmdir 
			// would fail.
		{
			Directory tmp( scratchdir );
			tmp.Remove_Entire_Directory();
		}
		if ( rmdir(scratchdir) == 0 ) {
			dprintf(D_FULLDEBUG,"Removed scratch dir %s\n",scratchdir);
		} else {
			dprintf(D_FULLDEBUG,"Failed to remove scratch dir %s\n",
					scratchdir);
		}
		set_priv(saved_priv);
		uninit_user_ids();
	}

	// Reclaim memory from the node itself
	delete gman_node;

	return 0;
}


GridUniverseLogic::gman_node_t *
GridUniverseLogic::lookupGmanByOwner(const char* owner, const char* attr_value,
					int cluster, int proc)
{
	gman_node_t* result = NULL;
	std::string owner_key(owner);
	if(attr_value){
		owner_key += attr_value;
	}
	if (cluster) {
		formatstr_cat( owner_key, "-%d.%d", cluster, proc );
	}

	if (!gman_pid_table) {
		// destructor has already been called; we are probably
		// closing down.
		return NULL;
	}

	gman_pid_table->lookup(owner_key,result);

	return result;
}

int
GridUniverseLogic::FindGManagerPid(const char* owner,
					const char* attr_value,	
					int cluster, int proc)
{
	gman_node_t* gman_node;

	if ( attr_value && strlen(attr_value)==0 ) {
		attr_value = NULL;
	}

	if ( (gman_node=lookupGmanByOwner(owner, attr_value, cluster, proc)) ) {
		return gman_node->pid;
	}
	else {
		return -1;
	}
}

GridUniverseLogic::gman_node_t *
GridUniverseLogic::StartOrFindGManager(const char* owner, const char* domain,
	   	const char* attr_value, const char* attr_name, int cluster, int proc)
{
	gman_node_t* gman_node;
	int pid;

		// If attr_value is an empty string, convert to NULL since code
		// after this point expects that.
	if ( attr_value && strlen(attr_value)==0 ) {
		attr_value = NULL;
		attr_name = NULL;
	}

	if ( !owner ) {
		dprintf(D_ALWAYS,"ERROR - missing owner field\n");
		return NULL;
	}

	if ( (gman_node=lookupGmanByOwner(owner, attr_value, cluster, proc)) ) {
		// found it
		return gman_node;
	}

	// not found.  fire one up!  we want to run the GManager as the user.

	// but first, make certain we are not shutting down...
	if (!gman_pid_table) {
		// destructor has already been called; we are probably
		// closing down.
		return NULL;
	}


#ifndef WIN32
	if (strcasecmp(owner, "root") == 0 ) {
		dprintf(D_ALWAYS, "Tried to start condor_gmanager as root.\n");
		return NULL;
	}
#endif

	dprintf( D_FULLDEBUG, "Starting condor_gmanager for owner %s (%d.%d)\n",
			owner, cluster, proc);

	char *gman_binary;
	gman_binary = param("GRIDMANAGER");
	if ( !gman_binary ) {
		dprintf(D_ALWAYS,"ERROR - GRIDMANAGER not defined in config file\n");
		return NULL;
	}

	ArgList args;
	std::string error_msg;

	args.AppendArg("condor_gridmanager");
	args.AppendArg("-f");

	if ( attr_value && *attr_value && param_boolean( "GRIDMANAGER_LOG_APPEND_SELECTION_EXPR", false ) ) {
		const std::string filename_filter = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-_";
		std::string log_suffix = attr_value;
		size_t pos = 0;
		while( (pos = log_suffix.find_first_not_of( filename_filter, pos )) != std::string::npos ) {
			log_suffix[pos] = '_';
		}
		args.AppendArg("-a");
		args.AppendArg(log_suffix.c_str());
	}

	char *gman_args = param("GRIDMANAGER_ARGS");

	if(!args.AppendArgsV1RawOrV2Quoted(gman_args,error_msg)) {
		dprintf( D_ALWAYS, "ERROR: failed to parse gridmanager args: %s\n",
				 error_msg.c_str());
		free(gman_binary);
		free(gman_args);
		return NULL;
	}
	free(gman_args);

	std::string constraint;
	if ( !attr_name  ) {
		formatstr(constraint, "(%s=?=\"%s\"&&%s==%d)",
						   ATTR_OWNER,owner,
						   ATTR_JOB_UNIVERSE,CONDOR_UNIVERSE_GRID);
	} else {
		formatstr(constraint, "(%s=?=\"%s\"&&%s=?=\"%s\"&&%s==%d)",
						   ATTR_OWNER,owner,
						   attr_name,attr_value,
						   ATTR_JOB_UNIVERSE,CONDOR_UNIVERSE_GRID);

		args.AppendArg("-A");
		args.AppendArg(attr_value);
	}
	args.AppendArg("-C");
	args.AppendArg(constraint);

	std::string full_owner_name(owner);
	if ( domain && *domain ) {
		formatstr_cat( full_owner_name, "@%s", domain );
	}
	args.AppendArg("-o");
	args.AppendArg(full_owner_name);

	if (!init_user_ids(owner, domain)) {
		dprintf(D_ALWAYS,"ERROR - init_user_ids() failed in GRIDMANAGER\n");
		free(gman_binary);
		return NULL;
	}

	static bool first_time_through = true;
	if ( first_time_through ) {
		// Note: Because first_time_through is static, this block runs only 
		// once per schedd invocation.
		first_time_through = false;

		// Clean up any old / abandoned scratch dirs.
		dprintf(D_FULLDEBUG,"Checking for old gridmanager scratch dirs\n");
		char *prefix = temp_dir_path();
		ASSERT(prefix);
		Directory tmp( prefix, PRIV_USER );
		const char *f;
		char const *dot;
		int fname_pid;
		int mypid = daemonCore->getpid();
		int scratch_pre_len = strlen(scratch_prefix);
		while ( (f=tmp.Next()) ) {
				// skip regular files -- we only need to inspect subdirs
			if ( !tmp.IsDirectory() ) {
				continue;
			}
				// skip if it does not start with our prefix
			if ( strncmp(scratch_prefix,f,scratch_pre_len) ) {
				continue;
			}
				// skip if does not end w/ a pid
			dot = strrchr(f,'.');
			if ( !dot ) {
				continue;
			}
				// skip if this pid is still alive and not ours
			dot++;	// skip over period
			fname_pid = atoi(dot);
			if ( fname_pid != mypid && daemonCore->Is_Pid_Alive(fname_pid) ) {
					continue;
			}
				// if we made it here, blow away this subdir
			if ( tmp.Remove_Current_File() ) {
				dprintf(D_ALWAYS,"Removed old scratch dir %s\n",
				tmp.GetFullPath());
			}
		}	// end of while for cleanup of old scratch dirs

		dprintf(D_FULLDEBUG,"Done checking for old scratch dirs\n");			

		free(prefix);

	}	// end of once-per-schedd invocation block

	// Create a temp dir for the gridmanager and append proper
	// command-line arguments to tell where it is.
	bool failed = false;
	gman_node = new gman_node_t;
	std::string pathbuf;
	const char *finalpath = scratchFilePath(gman_node, pathbuf);
	priv_state saved_priv = set_user_priv();
	if ( (mkdir(finalpath,0700)) < 0 ) {
		// mkdir failed.  
		dprintf(D_ALWAYS,"ERROR - mkdir(%s,0700) failed in GRIDMANAGER, errno=%d (%s)\n",
				finalpath, errno, strerror(errno));
		failed = true;
	}
	set_priv(saved_priv);
	uninit_user_ids();
	args.AppendArg("-S");	// -S = "ScratchDir" argument
	args.AppendArg(finalpath);
	if ( failed ) {
		// we already did dprintf reason to the log...
		free(gman_binary);
		delete gman_node;
		return NULL;
	}

	if(IsFulldebug(D_FULLDEBUG)) {
		std::string args_string;
		args.GetArgsStringForDisplay(args_string);
		dprintf(D_FULLDEBUG,"Really Execing %s\n",args_string.c_str());
	}

	std::string daemon_sock = SharedPortEndpoint::GenerateEndpointName( "gridmanager" );
	pid = daemonCore->Create_Process( 
		gman_binary,			// Program to exec
		args,					// Command-line args
		PRIV_ROOT,				// Run as root, so it can switch to
		                        //   PRIV_CONDOR
		rid,					// Reaper ID
		TRUE, TRUE, NULL, NULL, NULL, NULL,
		NULL, NULL, 0, NULL, 0, NULL, NULL,
		daemon_sock.c_str()
		);

	free(gman_binary);

	if ( pid <= 0 ) {
		dprintf ( D_ALWAYS, "StartOrFindGManager: Create_Process problems!\n" );
		if (gman_node) delete gman_node;
		return NULL;
	}

	// If we made it here, we happily started up a new gridmanager process

	dprintf( D_ALWAYS, "Started condor_gmanager for owner %s pid=%d\n",
			owner,pid);

	// Make a new gman_node entry for our hashtable & insert it
	if ( !gman_node ) {
		gman_node = new gman_node_t;
	}
	gman_node->pid = pid;
	gman_node->owner[0] = '\0';
	gman_node->domain[0] = '\0';
	if ( owner ) {
		strcpy(gman_node->owner,owner);
	}
	if ( domain ) {
		strcpy(gman_node->domain,domain);
	}
	std::string owner_key(owner);
	if(attr_value){
		owner_key += attr_value;
	}
	if (cluster) {
		formatstr_cat( owner_key, "-%d.%d", cluster, proc );
	}

	ASSERT( gman_pid_table->insert(owner_key,gman_node) == 0 );

	// start timer to signal gridmanager if we haven't already
	if ( gman_node->add_timer_id == -1 ) {  // == -1 means no timer set
		gman_node->add_timer_id = daemonCore->Register_Timer(job_added_delay,
			GridUniverseLogic::SendAddSignal,
			"GridUniverseLogic::SendAddSignal");
		daemonCore->Register_DataPtr(gman_node);
	}

	// All done
	return gman_node;
}
