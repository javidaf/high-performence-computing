import os 
from SCons.Script import Environment, Glob, VariantDir, Default

env = Environment(
    tools=['mingw'], 
    CC='gcc',
    CCFLAGS=['-Wall', '-Wextra', '-g', '-O3', '-std=c99'],
    CPPPATH=['#include', '#src']
)

# OpenMP environment for parallel version
omp_env = env.Clone()
omp_env.Append(CCFLAGS=['-fopenmp'], LINKFLAGS=['-fopenmp'])

build_dir = 'build'

# Add p1 project files with separate build directories for serial and parallel versions
p1_src_dir = os.path.join('src', 'p1')
p1_serial_build_dir = os.path.join(build_dir, 'p1', 'serial')
p1_omp_build_dir = os.path.join(build_dir, 'p1', 'omp')

# Create separate build directories for serial and parallel versions
VariantDir(p1_serial_build_dir, p1_src_dir, duplicate=0)
VariantDir(p1_omp_build_dir, p1_src_dir, duplicate=0)


# Serial PageRank
p1_serial_sources = [
    os.path.join(p1_serial_build_dir, 'read_graph_from_file1.c'),
    os.path.join(p1_serial_build_dir, 'read_graph_from_file2.c'),
    os.path.join(p1_serial_build_dir, 'PageRank_iterations1.c'),
    os.path.join(p1_serial_build_dir, 'PageRank_iterations2.c'),
    os.path.join(p1_serial_build_dir, 'top_n_webpages.c'),
    os.path.join(p1_serial_build_dir, 'main.c')
]

pagerank_serial = env.Program(
    target=os.path.join(build_dir, 'p1', 'pagerank_serial'),
    source=p1_serial_sources
)

# Parallel PageRank
p1_omp_sources = [
    os.path.join(p1_omp_build_dir, 'read_graph_from_file2.c'),
    os.path.join(p1_omp_build_dir, 'PageRank_iterations2_omp.c'),
    os.path.join(p1_omp_build_dir, 'top_n_webpages_omp.c'),
    os.path.join(p1_omp_build_dir, 'main_omp.c')
]

pagerank_omp = omp_env.Program(
    target=os.path.join(build_dir, 'p1', 'pagerank_omp'),
    source=p1_omp_sources
)

Default(pagerank_serial, pagerank_omp)