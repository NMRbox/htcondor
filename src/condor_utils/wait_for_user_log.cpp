/******************************************************************************
 *
 * Copyright (C) 1990-2018, Condor Team, Computer Sciences Department,
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
 ******************************************************************************/

#include "condor_common.h"
#include "condor_debug.h"
#include "condor_config.h"

#include "read_user_log.h"
#include "file_modified_trigger.h"
#include "wait_for_user_log.h"
#include "utc_time.h"

WaitForUserLog::WaitForUserLog( const std::string & f ) :
	filename( f ), reader( f.c_str(), true ), trigger( f ) { };

WaitForUserLog::~WaitForUserLog() { }

void
WaitForUserLog::releaseResources() {
	reader.releaseResources();
	trigger.releaseResources();
}

ULogEventOutcome
WaitForUserLog::readEvent( ULogEvent * & event, int timeout, bool following ) {
	if(! isInitialized()) {
		return ULOG_INVALID;
	}

	struct timeval then; condor_gettimestamp( then );
	ULogEventOutcome outcome = reader.readEvent( event );
	if( outcome != ULOG_NO_EVENT ) {
		return outcome;
	} else {
		if(! following) { return ULOG_NO_EVENT; }

		int result = trigger.wait( timeout );
		switch( result ) {
			case -1: // FIXME: return ULOG errors...
				return ULOG_INVALID;
			case  0:
				return ULOG_NO_EVENT;
			case  1: {
				// If the job event log event was not written atomically, it's
				// possible for reader.readEvent() to return ULOG_NO_EVENT
				// here, so if we just return that, we might break our promise
				// about how long we waited for a new event.

				// struct timeval now; condor_gettimestamp( now );
				// int elapsedMilliseconds = timersub_usec( now, then ) / 1000;
				// return readEvent( event, timeout - elapsedMilliseconds, following );
				//
				// Um, actually, the above code causes hangs if the original timeout
				// was zero, or if the elapsed time was greater than the requested timeout
				// or if the original timeout requested was negative.  Instead of trying
				// to be clever, let's try again with the original timeout, understanding
				// that we may take longer than requested, but that's true for any time
				// based sleep in the whole system.
				return readEvent(event, timeout, following);
				}
			default:
				EXCEPT( "Unknown return value from FileModifiedTrigger::wait(): %d, aborting.\n", result );
		}
	}
}

size_t
WaitForUserLog::getOffset() const {
    return reader.getOffset();
}

void
WaitForUserLog::setOffset( size_t offset ) {
    reader.setOffset(offset);
}
