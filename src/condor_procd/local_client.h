/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
  *
  * Condor Software Copyright Notice
  * Copyright (C) 1990-2006, Condor Team, Computer Sciences Department,
  * University of Wisconsin-Madison, WI.
  *
  * This source code is covered by the Condor Public License, which can
  * be found in the accompanying LICENSE.TXT file, or online at
  * www.condorproject.org.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  * AND THE UNIVERSITY OF WISCONSIN-MADISON "AS IS" AND ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY, OF SATISFACTORY QUALITY, AND FITNESS
  * FOR A PARTICULAR PURPOSE OR USE ARE DISCLAIMED. THE COPYRIGHT
  * HOLDERS AND CONTRIBUTORS AND THE UNIVERSITY OF WISCONSIN-MADISON
  * MAKE NO MAKE NO REPRESENTATION THAT THE SOFTWARE, MODIFICATIONS,
  * ENHANCEMENTS OR DERIVATIVE WORKS THEREOF, WILL NOT INFRINGE ANY
  * PATENT, COPYRIGHT, TRADEMARK, TRADE SECRET OR OTHER PROPRIETARY
  * RIGHT.
  *
  ****************************Copyright-DO-NOT-REMOVE-THIS-LINE**/

#ifndef _LOCAL_CLIENT_H
#define _LOCAL_CLIENT_H

#include "condor_common.h"

#if !defined(WIN32)
class NamedPipeWriter;
class NamedPipeReader;
class NamedPipeWatchdog;
#endif

class LocalClient {

public:

	LocalClient();

	// we use a plain old member function instead of the constructor
	// to do the real initialization work so that we can give our
	// caller an indication if something goes wrong
	//
	// init a new LocalClient that will connect to a LocalServer at
	// the given named pipe
	//
	bool initialize(const char*);

	// clean up
	//
	~LocalClient();

	// send the command contained in the given buffer
	//
	bool start_connection(void*, int);

	// end a command
	//
	void end_connection();

	// read response data from the server
	//
	bool read_data(void*, int);

private:

	// set true once we've been properly initialized
	//
	bool m_initialized;

	// implementation is totally different depending on whether we're
	// on Windows or UNIX. both use named pipes, but named pipes are not
	// (nearly) the same beast between the two
	//
#if defined(WIN32)
	char*  m_pipe_addr;
	HANDLE m_pipe;
#else
	static int         s_next_serial_number;
	int                m_serial_number;
	pid_t              m_pid;
	char*              m_addr;
	NamedPipeWriter*   m_writer;
	NamedPipeReader*   m_reader;
	NamedPipeWatchdog* m_watchdog;
#endif
};

#endif
