/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
 * CONDOR Copyright Notice
 *
 * See LICENSE.TXT for additional notices and disclaimers.
 *
 * Copyright (c)1990-1998 CONDOR Team, Computer Sciences Department, 
 * University of Wisconsin-Madison, Madison, WI.  All Rights Reserved.  
 * No use of the CONDOR Software Program Source Code is authorized 
 * without the express consent of the CONDOR Team.  For more information 
 * contact: CONDOR Team, Attention: Professor Miron Livny, 
 * 7367 Computer Sciences, 1210 W. Dayton St., Madison, WI 53706-1685, 
 * (608) 262-0856 or miron@cs.wisc.edu.
 *
 * U.S. Government Rights Restrictions: Use, duplication, or disclosure 
 * by the U.S. Government is subject to restrictions as set forth in 
 * subparagraph (c)(1)(ii) of The Rights in Technical Data and Computer 
 * Software clause at DFARS 252.227-7013 or subparagraphs (c)(1) and 
 * (2) of Commercial Computer Software-Restricted Rights at 48 CFR 
 * 52.227-19, as applicable, CONDOR Team, Attention: Professor Miron 
 * Livny, 7367 Computer Sciences, 1210 W. Dayton St., Madison, 
 * WI 53706-1685, (608) 262-0856 or miron@cs.wisc.edu.
****************************Copyright-DO-NOT-REMOVE-THIS-LINE**/

 

#ifndef __CONDOR_ADTYPES__
#define __CONDOR_ADTYPES__

/* these should really be const, but that causes problems with 
   ClassAd::SetMyTypeName and ClassAd::SetTargetTypeName.  -Jim B. */

static char STARTD_ADTYPE		[] = "Machine";
static char SCHEDD_ADTYPE		[] = "Scheduler";
static char MASTER_ADTYPE		[] = "DaemonMaster";
static char CKPT_SRVR_ADTYPE	[] = "CkptServer";
static char JOB_ADTYPE	 		[] = "Job";
static char QUERY_ADTYPE		[] = "Query";
static char COLLECTOR_ADTYPE	[] = "Collector";
static char CKPT_FILE_ADTYPE	[] = "CkptFile";
static char USERAUTH_ADTYPE   [] = "Authentication"; //mikeu added 
static char LICENSE_ADTYPE		[] = "License";

#endif
