import os
from build import CPPOptionsContext
from waflib import Scripting, Options

VERSION = '3.0-dev'
APPNAME = 'CODA-OSS'
top  = '.'
out  = 'target'

DIRS = 'modules'

TOOLS = 'build pythontool swig'

def options(opt):
    opt.load(TOOLS, tooldir='./build/')
    # always set_options on all
    opt.recurse(DIRS)

def configure(conf):
    conf.load(TOOLS, tooldir='./build/')
    conf.recurse(DIRS)

def build(bld):
    bld.recurse(DIRS)

def distclean(ctxt):
    ctxt.recurse(DIRS)
    Scripting.distclean(ctxt)
