####################################################
# SConstruct script
####################################################
# about Scons:
# get source file from http://www.scons.org/
# and install with 
# > python setup.py install
# (you need python installed)
# then just use 'scons' instead of 'make'
####################################################

import os, sys, glob

env = Environment(CC="gcc", CXX="g++", 
                  CCFLAGS="-Wall -Ofast -DNDEBUG",
                  CPPPATH = ['./',"/opt/local/include"],
                  LIBPATH = ['./']
)
envDebug = Environment(CC="gcc", CXX="g++", 
                  CCFLAGS="-g -Wall",
                  CPPPATH = ['./',"/opt/local/include"],
                  LIBPATH = ['./'],
)

sources_common = [ 'divsufsort.c','osus.cpp' ]
objects_common = env.Object(sources_common)
debug_objects_common = [envDebug.Object(src+'.debug.o', src) for src in sources_common]

def makeprog(onlyfor_sources, progname):
    objects_onlyfor = env.Object(onlyfor_sources)
    env.Program(progname, objects_common + objects_onlyfor)

def makeprogDebug(onlyfor_sources, progname):
    objects_onlyfor = envDebug.Object(onlyfor_sources+'.debug.o', onlyfor_sources)
    env.Program(progname + '.debug', debug_objects_common + objects_onlyfor)

progs = [
    ['main.cpp', 'osus'],
    ]
progsDebug = progs + [
    ]

for fin, fout in progs:
    makeprog([fin], fout)

for fin, fout in progsDebug:
    makeprogDebug(fin, fout)
