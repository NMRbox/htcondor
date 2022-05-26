# -*- coding: utf-8 -*-
#
# Read the Docs Template documentation build configuration file, created by
# sphinx-quickstart on Tue Aug 26 14:19:49 2014.
#
# This file is execfile()d with the current directory set to its
# containing dir.
#
# Note that not all possible configuration values are present in this
# autogenerated file.
#
# All configuration values have a default; values that are commented out
# serve to show the default.

import sys
import os

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# If this is not inserted at the beginning of the list, our 'jira' ticket
# extension is hidden by the system 'jira' API, if it exists.
sys.path.insert(0, os.path.abspath('extensions'))
sys.path.append(os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'src', 'condor_tests'))

# -- General configuration ------------------------------------------------

# If your documentation needs a minimal Sphinx version, state it here.
# needs_sphinx = '1.0'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'ticket',
    'macro',
    'macro-def',
    'jira',
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# The suffix of source filenames.
source_suffix = '.rst'

# The encoding of source files.
# source_encoding = 'utf-8-sig'

# The master toctree document.
master_doc = 'index'

# General information about the project.
project = u'HTCondor Manual'
copyright = u'1990-2020, Center for High Throughput Computing, Computer \
Sciences Department, University of Wisconsin-Madison, Madison, WI, US. \
Licensed under the Apache License, Version 2.0.'

# The version info for the project you're documenting, acts as replacement for
# |version| and |release|, also used in various other places throughout the
# built documents.
#
# The short X.Y version.
version = '9.0'
# The full version, including alpha/beta/rc tags.
release = '9.0.13'

rst_epilog = """
.. |release_date| replace:: May 26, 2022
"""

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
# language = None

# There are two options for replacing |today|: either, you set today to some
# non-false value, then it is used:
# today = ''
# Else, today_fmt is used as the format for a strftime call.
# today_fmt = '%B %d, %Y'

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
exclude_patterns = ['_build', 'extensions', 'utils']

# The reST default role (used for this markup: `text`) to use for all
# documents.
# default_role = None

# If true, '()' will be appended to :func: etc. cross-reference text.
# add_function_parentheses = True

# If true, the current module name will be prepended to all description
# unit titles (such as .. function::).
# add_module_names = True

# If true, sectionauthor and moduleauthor directives will be shown in the
# output. They are ignored by default.
# show_authors = False

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'colorful'

# A list of ignored prefixes for module index sorting.
# modindex_common_prefix = []

# If true, keep warnings as "system message" paragraphs in the built documents.
# keep_warnings = False


# -- Options for manual page output ---------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [
    ('man-pages/bosco_cluster', 'bosco_cluster', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/bosco_findplatform', 'bosco_findplatform', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/bosco_install', 'bosco_install', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/bosco_ssh_start', 'bosco_ssh_start', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/bosco_start', 'bosco_start', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/bosco_stop', 'bosco_stop', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/bosco_uninstall', 'bosco_uninstall', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/classad_eval', 'classad_eval', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/classads', 'classads', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_adstash', 'condor_adstash', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_advertise', 'condor_advertise', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_annex', 'condor_annex', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_check_password', 'condor_check_password', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_check_userlogs', 'condor_check_userlogs', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_chirp', 'condor_chirp', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_configure', 'condor_configure', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_config_val', 'condor_config_val', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_continue', 'condor_continue', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_convert_history', 'condor_convert_history', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_dagman', 'condor_dagman', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_drain', 'condor_drain', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_evicted_files', 'condor_evicted_files', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_fetchlog', 'condor_fetchlog', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_findhost', 'condor_findhost', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_gather_info', 'condor_gather_info', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_gpu_discovery', 'condor_gpu_discovery', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_history', 'condor_history', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_hold', 'condor_hold', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_install', 'condor_install', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_job_router_info', 'condor_job_router_info', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_master', 'condor_master', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_now', 'condor_now', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_off', 'condor_off', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_on', 'condor_on', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_ping', 'condor_ping', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_pool_job_report', 'condor_pool_job_report', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_power', 'condor_power', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_preen', 'condor_preen', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_prio', 'condor_prio', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_procd', 'condor_procd', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_q', 'condor_q', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_qedit', 'condor_qedit', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_qsub', 'condor_qsub', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_reconfig', 'condor_reconfig', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_release', 'condor_release', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_reschedule', 'condor_reschedule', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_restart', 'condor_restart', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_rm', 'condor_rm', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_rmdir', 'condor_rmdir', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_router_history', 'condor_router_history', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_router_q', 'condor_router_q', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_router_rm', 'condor_router_rm', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_run', 'condor_run', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_set_shutdown', 'condor_set_shutdown', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_sos', 'condor_sos', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_ssh_to_job', 'condor_ssh_to_job', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_stats', 'condor_stats', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_status', 'condor_status', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_store_cred', 'condor_store_cred', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_submit', 'condor_submit', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_submit_dag', 'condor_submit_dag', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_suspend', 'condor_suspend', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_tail', 'condor_tail', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_token_create', 'condor_token_create', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_token_fetch', 'condor_token_fetch', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_token_list', 'condor_token_list', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_token_request', 'condor_token_request', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_token_request_approve', 'condor_token_request_approve', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_token_request_auto_approve', 'condor_token_request_auto_approve', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_token_request_list', 'condor_token_request_list', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_top', 'condor_top', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_transfer_data', 'condor_transfer_data', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_transform_ads', 'condor_transform_ads', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_update_machine_ad', 'condor_update_machine_ad', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_updates_stats', 'condor_updates_stats', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_urlfetch', 'condor_urlfetch', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_userlog', 'condor_userlog', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_userprio', 'condor_userprio', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_vacate', 'condor_vacate', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_vacate_job', 'condor_vacate_job', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_version', 'condor_version', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_wait', 'condor_wait', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_watch_q', 'condor_watch_q', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/condor_who', 'condor_who', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/get_htcondor', 'get_htcondor', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/gidd_alloc', 'gidd_alloc', u'HTCondor Manual', [u'HTCondor Team'], 1),
    ('man-pages/procd_ctl', 'procd_ctl', u'HTCondor Manual', [u'HTCondor Team'], 1)
]

# If true, show URL addresses after external links.
# man_show_urls = False

# -- conf.py contains common configuration and man pages configuration
# -- full_conf.py contains configuration for the whole manual
sys.path.append(os.path.dirname(__file__))
MANPAGES = os.environ.get('MANPAGES') == 'True'
if not MANPAGES:
    from full_conf import *
