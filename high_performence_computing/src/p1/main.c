#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "function_declarations.h"

int main(int argc, char *argv[]) {
    if (argc != 6) {
        printf("Usage: %s <small_graph_file> <large_graph_file> <damping_factor> <epsilon> <top_n>\n", argv[0]);
        return 1;
    }

    char *small_graph_file = argv[1];
    char *large_graph_file = argv[2];
    double d = atof(argv[3]);
    double epsilon = atof(argv[4]);
    int top_n = atoi(argv[5]);

    clock_t start, end;
    double cpu_time_used;

    printf("\n--- Processing small graph using dense matrix representation ---\n");
    int N1;
    double **hyperlink_matrix = NULL;
    double *scores1 = NULL;

    start = clock();
    printf("Reading graph from file: %s\n", small_graph_file);
    read_graph_from_file1(small_graph_file, &N1, &hyperlink_matrix);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Reading completed. Number of webpages: %d\n", N1);
    printf("Time taken for reading graph: %f seconds\n", cpu_time_used);

    scores1 = (double *)malloc(N1 * sizeof(double));

    start = clock();
    printf("Running PageRank iterations with damping factor = %f, epsilon = %e\n", d, epsilon);
    PageRank_iterations1(N1, hyperlink_matrix, d, epsilon, scores1);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("PageRank iterations completed\n");
    printf("Time taken for PageRank iterations: %f seconds\n", cpu_time_used);

    printf("Top %d webpages:\n", top_n);
    top_n_webpages(N1, scores1, top_n);

    for (int i = 0; i < N1; i++) {
        free(hyperlink_matrix[i]);
    }
    free(hyperlink_matrix);
    free(scores1);

    printf("\n--- Processing large graph using CRS format ---\n");
    int N2;
    int *row_ptr = NULL;
    int *col_idx = NULL;
    double *val = NULL;
    double *scores2 = NULL;

    start = clock();
    printf("Reading graph from file: %s\n", large_graph_file);
    read_graph_from_file2(large_graph_file, &N2, &row_ptr, &col_idx, &val);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Reading completed. Number of webpages: %d\n", N2);
    printf("Time taken for reading graph: %f seconds\n", cpu_time_used);

    scores2 = (double *)malloc(N2 * sizeof(double));
    if (scores2 == NULL) {
        printf("Memory allocation failed for scores\n");
        return 1;
    }

    start = clock();
    printf("Running PageRank iterations with damping factor = %f, epsilon = %e\n", d, epsilon);
    PageRank_iterations2(N2, row_ptr, col_idx, val, d, epsilon, scores2);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("PageRank iterations completed\n");
    printf("Time taken for PageRank iterations: %f seconds\n", cpu_time_used);

    printf("Top %d webpages:\n", top_n);
    top_n_webpages(N2, scores2, top_n);

    free(row_ptr);
    free(col_idx);
    free(val);
    free(scores2);

    return 0;
}