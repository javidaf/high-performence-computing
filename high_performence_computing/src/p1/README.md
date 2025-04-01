# PageRank Implementation

This project implements the PageRank algorithm for computing importance scores of webpages in a web graph. The implementation includes both serial and parallel (OpenMP) versions.

## Compilation

This project uses SCons for building. To compile the code, run:

```bash
scons
```

This will build both the serial and parallel versions of the PageRank program.

## Usage

### Serial Version

The serial version processes two web graph files - one small using the dense matrix representation and one large using the CRS format:

```bash
./build/p1/pagerank_serial <small_graph_file> <large_graph_file> <damping_factor> <epsilon> <top_n>
```

Parameters:

- `small_graph_file`: Path to a small web graph file
- `large_graph_file`: Path to a large web graph file
- `damping_factor`: The damping factor (typically 0.85)
- `epsilon`: The convergence threshold (typically 1e-6)
- `top_n`: Number of top-ranked webpages to display

Example:

```bash
./build/p1/pagerank_serial simple-webgraph.txt web-stanford.txt 0.85 1e-6 10
```

### Parallel Version (OpenMP)

The parallel version processes a single web graph file using the CRS format with OpenMP parallelization:

```bash
./build/p1/pagerank_omp <web_graph_file> <damping_factor> <epsilon> <top_n>
```

Parameters:

- `web_graph_file`: Path to a web graph file
- `damping_factor`: The damping factor (typically 0.85)
- `epsilon`: The convergence threshold (typically 1e-6)
- `top_n`: Number of top-ranked webpages to display

Example:

```bash
./build/p1/pagerank_omp web-stanford.txt 0.85 1e-6 10
```

To control the number of OpenMP threads, set the OMP_NUM_THREADS environment variable:

```bash
export OMP_NUM_THREADS=4
./build/p1/pagerank_omp web-stanford.txt 0.85 1e-6 10
```
