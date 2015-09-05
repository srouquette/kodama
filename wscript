#!/usr/bin/env python
# encoding: utf-8

VERSION='0.0.1'
APPNAME='kodama'

top = '.'
out = '.build'


import os, sys
import platform
from waflib import Configure
from waflib import Logs
from waflib import Utils
from waflib.Tools import waf_unit_test
from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext
from wafcfg import common


''' Define Waf contexts for debug and release builds. '''
CONTEXTS = { BuildContext, CleanContext, InstallContext, UninstallContext }
for var in 'debug release'.split():
    for ctx in CONTEXTS:
        name = ctx.__name__.replace('Context', '').lower()

        class tmp(ctx):
            cmd = name + '_' + var
            variant = var


def options(opt):
    opt.load('cpplint waf_unit_test compiler_cxx boost use_config')


def configure(conf):
    conf.env.TARGET = platform.system().lower() + '_' + platform.machine().lower()
    conf.msg('Setting target to', conf.env.TARGET)

    conf.set_env()

    conf.load('cpplint waf_unit_test compiler_cxx boost')

    env = conf.env.derive()
    conf.set_variant('release', env)
    conf.check_libs()
    conf.check_cpp14()
    conf.check_filesystem()

    conf.set_variant('debug', env)
    conf.check_libs()
    conf.check_cpp14()
    conf.check_filesystem()


def build(bld):
    if not bld.variant:
        bld.fatal('call "waf build_debug", "waf build_release"\nwaf --help')

    bld.env.BINDIR          = 'bin/' + bld.variant
    bld.env.CPPLINT_FILTERS = ','.join(['-build/include_what_you_use',
                                        '-build/include_order',
                                        '-runtime/references',
                                        '-whitespace/braces',
                                        '-whitespace/line_length',
                                        '-whitespace/newline' if bld.env.platform == 'posix' else ''])
    # bld.env.CXXFLAGS      += ['/analyze']

    bld(features = 'cpplint', source = bld.path.ant_glob('include/**/*.h'))

    bld.stlib(
        target      = 'kodama_filesystem',
        features    = 'cpplint',
        includes    = 'include',
        cxxflags    = '-Weffc++' if bld.env.cxx == 'g++' else '',
        source      = bld.path.ant_glob('src/filesystem/*.cpp'),
        use         = 'GCOV')

    bld.recurse('test')

    bld.program(
        target      = 'kodama',
        features    = 'cpplint',
        includes    = 'include',
        subsystem   = 'console',
        source      = bld.path.ant_glob('''
                                        src/app/*
                                        res/{platform}.rc
                                        '''.format(**bld.env)),
        use         = 'BOOST kodama_filesystem')
