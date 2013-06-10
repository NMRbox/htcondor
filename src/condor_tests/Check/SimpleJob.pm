##**************************************************************
##
## Copyright (C) 1990-2007, Condor Team, Computer Sciences Department,
## University of Wisconsin-Madison, WI.
## 
## Licensed under the Apache License, Version 2.0 (the "License"); you
## may not use this file except in compliance with the License.  You may
## obtain a copy of the License at
## 
##    http://www.apache.org/licenses/LICENSE-2.0
## 
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.
##
##**************************************************************

package SimpleJob;

use CondorTest;
my $timeout = 0;
my $defaulttimeout = 60;
$timeout = $defaulttimeout;

$timed_callback = sub
{
	print "SimpleJob timeout expired!\n";
	CondorTest::RegisterResult( 0, %args );
    return $result;
};

$submitted = sub
{
	CondorTest::debug("Job submitted\n",1);
};

$aborted = sub 
{
	die "Abort event NOT expected\n";
};

$dummy = sub
{
};

$ExitSuccess = sub {
	CondorTest::debug("Job completed\n",1);
};

sub RunCheck
{
    my %args = @_;
    my $testname = $args{test_name} || CondorTest::GetDefaultTestName();
    my $universe = $args{universe} || "vanilla";
    my $user_log = $args{user_log} || CondorTest::TempFileName("$testname.user_log");
    my $append_submit_commands = $args{append_submit_commands} || "";
    my $grid_resource = $args{grid_resource} || "";
    my $should_transfer_files = $args{should_transfer_files} || "";
    my $when_to_transfer_output = $args{when_to_transfer_output} || "";
    my $duration = "1";
	if(exists $args{duration}) {
		$duration = $args{duration};
	}

	if(exists $args{timeout}){
		$timeout = $args{timeout};
		print "Test getting requested timeout of <$timeout> seconds\n";
	}
	 
	#print "Checking duration being passsed to RunCheck <$args{duration}>\n";
    my $execute_fn = $args{on_execute} || $dummy;
    my $suspended_fn = $args{on_suspended} || $dummy;
    my $unsuspended_fn = $args{on_unsuspended} || $dummy;
    my $disconnected_fn = $args{on_disconnected} || $dummy;
    my $reconnected_fn = $args{on_reconnected} || $dummy;
    my $submit_fn = $args{on_submit} || $submitted;
    my $ulog_fn = $args{on_ulog} || $dummy;
    my $abort_fn = $args{on_abort} || $aborted;
	my $donewithsuccess_fn = $args{on_success} || $ExitSuccess;


	if(exists $args{timeout}){
		CondorTest::RegisterTimed( $testname, $timed_callback, $timeout);
	}
    CondorTest::RegisterAbort( $testname, $abort_fn );
    CondorTest::RegisterExitedSuccess( $testname, $donewithsuccess_fn );
    CondorTest::RegisterExecute($testname, $execute_fn);
    CondorTest::RegisterULog($testname, $ulog_fn);
    CondorTest::RegisterSubmit( $testname, $submit_fn );

	#If we register thees to dummy, then we don't get
	#the error function registered which says this is bad

	if( exists $args{on_disconnected} ) {
    	CondorTest::RegisterDisconnected( $testname, $disconnected_fn );
	}
	if( exists $args{on_reconnected} ) {
    	CondorTest::RegisterReconnected( $testname, $reconnected_fn );
	}
	if( exists $args{on_suspended} ) {
    	CondorTest::RegisterSuspended( $testname, $suspended_fn );
	}
	if( exists $args{on_unsuspended} ) {
    	CondorTest::RegisterUnsuspended( $testname, $unsuspended_fn );
	}
	if(exists $args{on_failure}) {
		CondorTest::RegisterExitedFailure( $testname, $args{on_failure} );
	}

	my $program = $args{runthis} || "x_sleep.pl";
    my $submit_fname = CondorTest::TempFileName("$testname.submit");

    open( SUBMIT, ">$submit_fname" ) || die "error writing to $submit_fname: $!\n";
    print SUBMIT "universe = $universe\n";
    print SUBMIT "executable = $program\n";
    print SUBMIT "log = $user_log\n";
    print SUBMIT "arguments = $duration\n";
    print SUBMIT "notification = never\n";
    if( $grid_resource ne "" ) {
	print SUBMIT "GridResource = $grid_resource\n"
    }
    if( $should_transfer_files ne "" ) {
	print SUBMIT "ShouldTransferFiles = $should_transfer_files\n";
    }
    if( $when_to_transfer_output ne "" ) {
	print SUBMIT "WhenToTransferOutput = $when_to_transfer_output\n";
    }
    if( $append_submit_commands ne "" ) {
        print SUBMIT "\n" . $append_submit_commands . "\n";
    }
    print SUBMIT "queue $args{queue_sz}\n";
    close( SUBMIT );

    my $result = 0;
	if (defined $args{GetClusterId}) {
    	$result = CondorTest::RunTest($testname, $submit_fname, 0, $args{GetClusterId});
	} else {
    	$result = CondorTest::RunTest($testname, $submit_fname, 0);
	}
    CondorTest::RegisterResult( $result, %args );
    return $result;
}

1;
