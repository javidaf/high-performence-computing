#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "function_declarations.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <web_graph_file> <damping_factor> <epsilon> <top_n>\n", argv[0]);
        return 1;
    }

    char *graph_file = argv[1];
    double d = atof(argv[2]);
    double epsilon = atof(argv[3]);
    int top_n = atoi(argv[4]);

    double start_time, end_time, elapsed_time;

    int max_threads = omp_get_max_threads();
    printf("Running with %d OpenMP threads\n", max_threads);

    printf("\n--- Processing graph using CRS format (OpenMP version) ---\n");
    int N;
    int *row_ptr = NULL;
    int *col_idx = NULL;
    double *val = NULL;
    double *scores = NULL;

    start_time = omp_get_wtime();
    printf("Reading graph from file: %s\n", graph_file);
    read_graph_from_file2(graph_file, &N, &row_ptr, &col_idx, &val);
    end_time = omp_get_wtime();
    elapsed_time = end_time - start_time;
    printf("Reading completed. Number of webpages: %d\n", N);
    printf("Time taken for reading graph: %f seconds\n", elapsed_time);

    scores = (double *)malloc(N * sizeof(double));
    if (scores == NULL) {
        printf("Memory allocation failed for scores\n");
        return 1;
    }

    start_time = omp_get_wtime();
    printf("Running parallel PageRank iterations with damping factor = %f, epsilon = %e\n", d, epsilon);
    PageRank_iterations2(N, row_ptr, col_idx, val, d, epsilon, scores);
    end_time = omp_get_wtime();
    elapsed_time = end_time - start_time;
    printf("PageRank iterations completed\n");
    printf("Time taken for parallel PageRank iterations: %f seconds\n", elapsed_time);

    printf("Top %d webpages:\n", top_n);
    top_n_webpages(N, scores, top_n);

    free(row_ptr);
    free(col_idx);
    free(val);
    free(scores);

    return 0;
}