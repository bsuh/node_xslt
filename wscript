import subprocess

srcdir = '.'
blddir = 'build'
VERSION = '0.1.0'

def set_options(opt):
    opt.tool_options('compiler_cxx')

def configure(conf):
    conf.check_tool('compiler_cxx')
    conf.check_tool('node_addon')
    conf.check(lib=['xml2', 'xslt'], uselib_store='M', mandatory=True)

def build(bld):
    obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
    obj.target = 'node_xslt'
    obj.source = 'node_xslt.cc'
    obj.cxxflags = subprocess.check_output(["xml2-config", "--cflags"]).strip()
    obj.lib = ['xml2', 'xslt']
    obj.libpath = ['/usr/lib']

def clean(ctx):
    print("Cleaning nothing...")
