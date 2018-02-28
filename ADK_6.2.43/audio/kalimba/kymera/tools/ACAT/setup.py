############################################################################
# CONFIDENTIAL
#
# Copyright (c) 2012 - 2017 Qualcomm Technologies International, Ltd.
#
############################################################################
'''
ACAT's setup file
'''

import distutils
import shlex
import subprocess
import sys

from setuptools import setup, find_packages

PACKAGE_NAME = 'ACAT'

# this will load version.py which should contain a __version__ = 'version'
exec(open('ACAT/_version.py').read())  # @UndefinedVariable pylint: disable=exec-used

VERSION = __version__  # @UndefinedVariable pylint: disable=undefined-variable


class PylintCommand(distutils.cmd.Command):
    '''
    A custom command to run Pylint on all Python source files.
    '''

    description = 'run Pylint on Python source files'
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        '''
        Run command.
        '''
        cmdline = 'pylint --rcfile=.pylintrc ' + PACKAGE_NAME
        cmdline = cmdline if sys.platform == 'win32' else shlex.split(cmdline)
        self.announce('Running command: %s' % str(cmdline), level=distutils.log.INFO)
        subprocess.call(cmdline)


try:
    from sphinx.setup_command import BuildDoc

    class BuildSphinxCommand(BuildDoc):
        '''
        Custom build sphinx documentation command.
        '''

        def run(self):
            """
            Builds sphinx documentation.
            """
            cmdline = 'sphinx-apidoc -F -o doc ' + PACKAGE_NAME
            cmdline = cmdline if sys.platform == 'win32' else shlex.split(cmdline)
            self.announce('Running command: %s' % str(cmdline), level=distutils.log.INFO)
            subprocess.check_call(cmdline)
            BuildDoc.run(self)
except ImportError:
    pass


setup(
    name=PACKAGE_NAME,

    # Versions should comply with PEP440.  For a discussion on single-sourcing
    # the version across setup.py and the project code, see
    # https://packaging.python.org/en/latest/single_source_version.html
    version=VERSION,

    description='Audio Coredump Analysis Tool',

    # author='Qualcomm',
    # author_email='lbarta@qti.qualcomm.com',
    url='http://www.qualcomm.com/',
    # license='MIT',  # FIXME

    classifiers=[

        'Development Status :: 2 - PreAlpha',

        'Environment :: Console',

        'Intended Audience :: Customer Service',
        'Intended Audience :: Developers'
        'Intended Audience :: Information Technology',
        'Intended Audience :: Manufacturing',
        'Intended Audience :: Telecommunications Industry',

        # (should match "license" above)
        # 'License :: OSI Approved :: MIT License',  # FIXME


        'Operating System :: Microsoft :: Windows',
        'Operating System :: POSIX',

        'Programming Language :: Python',
        'Programming Language :: Python :: 2.7',
        'Topic :: Software Development :: Testing'
    ],

    packages=find_packages(exclude=['contrib', 'docs']),

    command_options={
        'build_sphinx': {
            'version': ('setup.py', VERSION),
            'release': ('setup.py', VERSION)}},
)
