import subprocess

srcdir = '.'
blddir = 'build'

def set_options(opt):
    opt.tool_options('compiler_cxx')

def configure(conf):
    conf.check_tool('compiler_cxx')
    conf.check_tool('node_addon')
    conf.check(lib=['xml2', 'xslt', 'exslt'], uselib_store='M', mandatory=True)
    conf.env.set_variant('Release')

def build(bld):
    obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
    obj.target = 'node_xslt'
    obj.source = 'node_xslt.cc'
    obj.cxxflags = subprocess.Popen(["xml2-config", "--cflags"], stdout=subprocess.PIPE).communicate()[0].strip().split()
    obj.lib = ['xml2', 'xslt', 'exslt']
    obj.libpath = ['/usr/lib']

