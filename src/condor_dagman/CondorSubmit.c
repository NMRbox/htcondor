
#include "CondorSubmit.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Required for linking with condor libs
extern "C" int SetSyscalls() { return 0; }

//
// Flag to determine if the fake submit should be used
//
extern bool DO_FAKE_SUBMIT;

static const int MAX_CMD_LENGTH = 200;

static char ErrorBuffer[MAX_CMD_LENGTH];

// CondorSubmit returns 0 on success, non-zero on failure

int CondorSubmit(const char *cmdfile, char *&errorString,
		 int &cluster, int &proc, int &subproc)
{
    FILE *fp;
    char *command;
    char buffer[MAX_CMD_LENGTH];
    int status;
    char *executable;
    
    if (DO_FAKE_SUBMIT)
    {
	executable = "./condor_submit.fake";
    }
    else
    {
	// the '-p' parameter to condor_submit will now cause condor_submit
	// to pause ~4 seconds after a successfull submit.  this prevents
	// the race condition of condor_submit finishing before dagman
	// does a pclose, which at least on SGI IRIX causes a nasty 'Broken Pipe'
	executable = "condor_submit -p";
    }

    subproc = 0;
    errorString = ErrorBuffer;
    
    command = new char[strlen(executable) + strlen(cmdfile) + 10];
    if (command == NULL)
    {
	strcpy(ErrorBuffer, "new() failed");
        return 1;
    }
    sprintf(command, "%s %s 2>&1", executable, cmdfile);

    fp = popen(command, "r");
    if (fp == NULL)
    {
	strcpy(ErrorBuffer, "popen() failed");
        return 1;
    }
    
    if (getline(fp, buffer, MAX_CMD_LENGTH) <= 0)
    {
	pclose(fp);
	strcpy(ErrorBuffer, "No output generated by condor_submit");
        return 1;
    }
    
    
    status = pclose(fp);
    printf("%s exited with status %d\n", executable, status);
#if 0
    //
    // condor_submit does not always return 0 on success,
    // so this check for a non-zero exit status cannot be
    // used
    //
    if (status != 0)
    {
	//
	// Assume that condor_submit returned an error code and
	// reported an error on its stderr stream
	//
	strcpy(ErrorBuffer, buffer);
        return 1;
    }
#endif
    
    status = sscanf(buffer, "Proc %d.%d", &cluster, &proc);
    if (status != 2)
    {
	//
	// If the word "ERROR" appears in the output, assume that
	// the output is an error message that needs to be returned
	// to the caller
	//
	if (strstr(buffer, "ERROR") || strstr(buffer, "Error") ||
	    strstr(buffer, "error"))
	{
	    strcpy(ErrorBuffer, buffer);
	    return 1;
	}
	// Truncate error string to 40 characters
	buffer[40] = '\0';
	// If the word "ERROR" was not in the output line, then I
	// don't know how to interpret the output line. So just copy
	// 40 characters into the error buffer
	sprintf(ErrorBuffer,
		"Could not parse condor_submit output: %s", buffer);
	return 1;
    }
    
    return 0;

}

