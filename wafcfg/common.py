#!/usr/bin/env python
# encoding: utf-8

import platform
from waflib.Configure import conf
from waflib import Logs

TARGETS = {
    'linux_i686': {
        'arch': 32,
        'suffix': '-i686',
        'platform': 'posix',
        'interface': 'x11',
    },
    'linux_x86_64': {
        'arch': 64,
        'suffix': '-x64',
        'platform': 'posix',
        'interface': 'x11',
    },
    'windows_x86': {
        'arch': 32,
        'suffix': '-x86',
        'platform': 'win32',
        'interface': 'win32',
        'LIB': ['Shell32', 'User32']
    },
    'windows_amd64': {
        'arch': 64,
        'suffix': '-x64',
        'platform': 'win32',
        'interface': 'win32',
        'LIB': ['Shell32', 'User32']
    }
}


GCC_FLAGS = {
    'boost': {
        'lib'  : 'system locale date_time thread filesystem'},
    'common': [
        '-std=c++1y',
        '-fPIC',        # required by Qt
        '-Wdouble-promotion',
        '-Wall',
        '-Wextra',
        # '-Weffc++',   # doesn't work well with Qt
        '-pedantic'],
    'debug': [
        '-DDEBUG',
        '-g',
        '-ggdb',
        '-O0',
        '-fno-omit-frame-pointer'],
    'release': ['-O2', '-DNDEBUG'],
    'link_debug': ['-pthread'],
    'link_release': ['-pthread'],
    'arch_32': { 'cxxflags': ['-m32'] },
    'arch_64': { 'cxxflags': ['-m64'] },
}


MSVC_FLAGS = {
    'boost': { 'lib': 'system locale date_time chrono thread filesystem' },
    'common': [
        '/nologo',
        '/EHsc',    # enable exception
        '/FS',      # unlock write to PDB file
        '/fp:fast', # fast floating point instead of precise
        '/W4',      # warning level 4
        '/WX',      # warning as error
        '/DBOOST_ALL_DYN_LINK', # link all boost libs dynamically
        '/D_WIN32_WINNT=0x0501'],
    'debug': [
        '/MDd',     # multi-threading
        '/Od',      # no optimization
        '/Zi',      # full debug info
        '/DDEBUG',
        '/D_SCL_SECURE_NO_WARNINGS'],   # disable a boost error
    'release': [
        '/MD',      # multi-threading
        '/Qpar',    # execute loop in parallel when possible
        '/O2',      # optimize for speed
        '/GL',      # global optimization (used by /LTCG)
        '/GA',      # optimized for Windows
        '/Gy',      # bundle function in COMDAT (used by /OPT)
        '/Gw',      # bundle global data in COMDAT (used by /OPT)
        '/DNDEBUG'], # no debug (like assert)
    'link_debug': [
        '/DEBUG',
        '/WX'],     # warning as error
    'link_release': [
        '/nologo',
        '/WX',              # warning as error
        '/OPT:REF',         # remove unreferenced code
        '/OPT:ICF=5',       # remove unreferenced code
        '/LTCG'],           # optimize code at link-time (use /GL)
    'arch_32': { 'env': { 'MSVC_MANIFEST': False } },
    'arch_64': { 'env': { 'MSVC_MANIFEST': False } },
}


FLAGS = {
    'clang++': GCC_FLAGS,
    'g++': GCC_FLAGS,
    'gcc': GCC_FLAGS,
    'msvc': MSVC_FLAGS,
}


@conf
def set_env(conf):
    conf.define('BOOSTTHREAD_USE_LIB', 1)
    conf.define('BOOSTTHREAD_VERSION', 4)
    conf.define('GTEST_LANG_CXX11', 1)
    for key, value in TARGETS[conf.env.TARGET].items():
        conf.env[key] = value

@conf
def check_gtest(conf):
    conf.check(lib='gtest' if conf.variant == 'release' else 'gtestd',
               header_name='gtest/gtest.h', uselib_store='GTEST', mandatory=False)
    conf.check(lib='gmock' if conf.variant == 'release' else 'gmockd', use='GTEST',
               header_name='gmock/gmock.h', uselib_store='GMOCK', mandatory=False)

@conf
def check_gcov(conf):
    if conf.variant == 'debug':
        conf.check(lib='gcov', uselib_store='GCOV', mandatory=False)
    if (conf.env.LIB_GCOV):
        conf.env.CXXFLAGS  += ['--coverage', '-fprofile-arcs', '-ftest-coverage']
        conf.env.LINKFLAGS += ['--coverage', '-fprofile-arcs']

@conf
def check_libs(conf):
    check_gtest(conf)
    check_gcov(conf)
    boost_params = dict(dict(mt = True, abi = 'gd' if conf.variant == 'debug' else ''),
                        **FLAGS[conf.env.COMPILER_CXX]['boost'])
    conf.check_boost(**boost_params)

@conf
def check_cpp14(conf):
    conf.start_msg('Checking C++14 support')
    try:
        conf.check_cxx(
         fragment='''
#include <memory>
class test {
    test()              = default;
    test(const test&)   = delete;
    test(test&&)        = delete;
    ~test()             = default;
};
int main() {
    auto ptr = std::make_unique<int>();
    return 0;
}''',
         execute=False,
        )
        conf.end_msg('ok')
    except:
        conf.fatal('failed')


@conf
def check_filesystem(conf):
    conf.start_msg('Checking filesystem support')
    try:
        conf.check_cxx(
         fragment='\n'.join([
          '#include <filesystem>',
          'int main() { std::tr2::sys::path path; }',
         ]),
         execute=False,
        )
        conf.define('STL_FILESYSTEM_ENABLED', 1)
        conf.end_msg('<filesystem>')
    except:
        conf.end_msg('<boost/filesystem.hpp>')


@conf
def set_variant(conf, variant, env):
    conf.msg('----------------------------------------', '----{ %s' % variant)
    conf.setenv(variant, env=env)
    compiler = FLAGS[conf.env.COMPILER_CXX]
    arch = 'arch_%d' % TARGETS[conf.env.TARGET]['arch']
    compiler_target = compiler[arch] if arch in compiler else None

    # Compiler options
    conf.env.CXXFLAGS = compiler['common'] + compiler[variant]
    if compiler_target and 'cxxflags' in compiler_target:
        conf.env.CXXFLAGS += compiler_target['cxxflags']

    # Linker options
    conf.env.LINKFLAGS = compiler['link_' + variant]
    if compiler_target and 'linkflags' in compiler_target:
        conf.env.LINKFLAGS += compiler_target['linkflags']

    # Includes and defines
    if conf.env.TARGET == 'windows_x86':
        conf.define('WIN32_LEAN_AND_MEAN', 1)
    if compiler_target and 'env' in compiler_target:
        for key, value in compiler_target['env'].items():
            conf.env[key] = value
