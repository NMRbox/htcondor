#!/usr/bin/env pytest

import htcondor
import logging
import os
from pathlib import Path
import subprocess

from ornithology import *


logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)


@action
def invoke_no_args(default_condor):
    p = default_condor.run_command(["htcondor"])
    return p


@action
def submit_failure(default_condor):
    p = default_condor.run_command(["htcondor", "job", "submit", "does_not_exist.sub"])
    return p


@action
def submit_success(default_condor, test_dir):
    submit_file = open(test_dir / "helloworld.sub", "w")
    submit_file.write("""executable = /bin/echo
arguments = Hello, World!
log = helloworld.log
output = helloworld.out
request_disk = 10M

queue""")
    submit_file.close()
    p = default_condor.run_command(["htcondor", "job", "submit", test_dir / "helloworld.sub"])
    return p


@action
def resource_missing_args(default_condor, test_dir, submit_success):
    p = default_condor.run_command(["htcondor", "job", "submit", test_dir / "helloworld.sub", "--resource", "ec2"])
    return p

@action
def slurm_not_setup(default_condor, test_dir):
    p = default_condor.run_command(["htcondor", "job", "submit", test_dir / "helloworld.sub", "--resource", "slurm", "--runtime", "1", "--node_count", "1"])
    return p


class TestCurlPlugin:
    def test_invoke_no_args(self, invoke_no_args):
        assert invoke_no_args.stderr[0:60] == "htcondor is a tool for managing HTCondor jobs and resources."
        assert "Traceback (most recent call last)" not in invoke_no_args.stdout
        assert "Traceback (most recent call last)" not in invoke_no_args.stderr

    def test_submit_fail(self, submit_failure):
        assert submit_failure.stdout == "Error: could not read file does_not_exist.sub"

    def test_submit_success(self, test_dir, submit_success):
        assert submit_success.stdout == "Job 1 was submitted."
        jel = htcondor.JobEventLog((test_dir / "helloworld.log").as_posix())
        # Wait for the job to finish by watching its event log
        for event in jel.events(stop_after=None):
            if event.type == htcondor.JobEventType.JOB_TERMINATED:
                break
        assert Path(test_dir / "helloworld.out").read_text() == "Hello, World!\n"

    def test_resource_missing_args(self, test_dir, resource_missing_args):
        assert resource_missing_args.stdout == "Error: EC2 resources must specify a --runtime argument"

    def test_slurm_not_setup(self, test_dir, slurm_not_setup):
        assert slurm_not_setup.stdout[0:65] == "You need to install support software to access the Slurm cluster."
