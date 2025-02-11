import os 
from SCons.Script import Environment, Glob, VariantDir, Default

env = Environment(
    tools=['mingw'], 
    CC='gcc',
    CCFLAGS=['-Wall', '-Wextra', '-g', '-O3', '-std=c99'],
    CPPPATH=['#include']
)

src_files = Glob('src/*.c')

build_dir = 'build'

VariantDir(build_dir, 'src', duplicate=0)

build_files = [os.path.join(build_dir, os.path.basename(str(f))) for f in src_files]

program = env.Program(
    target=os.path.join(build_dir, 'program'),
    source=build_files
)

Default(program)