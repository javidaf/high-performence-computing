#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "function_declarations.h"

void PageRank_iterations2(int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores) {

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        scores[i] = 1.0 / N; // Initial guess
    }

    double *new_scores = (double *)malloc(N * sizeof(double));
    if (new_scores == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for new_scores\n");
        exit(1);
    }

    int *is_dangling = (int *)calloc(N, sizeof(int));
    if (is_dangling == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for is_dangling\n");
        free(new_scores);
        exit(1);
    }

    // A node is dangling if it has no outbound links
    // In the CRS format, we can check if any column has at least one non-zero entry
    int *has_outlinks = (int *)calloc(N, sizeof(int));
    
    #pragma omp parallel for
    for (int i = 0; i < row_ptr[N]; i++) {
        #pragma omp atomic write
        has_outlinks[col_idx[i]] = 1;
    }
    
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        if (!has_outlinks[i]) {
            is_dangling[i] = 1;
        }
    }
    free(has_outlinks);

    double max_diff;
    int iteration = 0;
    
    // Iterative computation of PageRank
    do {
        // Compute W^(k-1), the sum of dangling node scores
        double W = 0.0;
        
        #pragma omp parallel
        {
            double local_W = 0.0;
            
            #pragma omp for
            for (int j = 0; j < N; j++) {
                if (is_dangling[j]) {
                    local_W += scores[j];
                }
            }
            #pragma omp atomic
            W += local_W;
        }

        // Compute the constant part for all nodes (teleportation probability)
        double teleport = (1 - d + d * W) / N;

        // Initialize new scores with teleportation probability
        #pragma omp parallel for
        for (int i = 0; i < N; i++) {
            new_scores[i] = teleport;
        }

        // Add the contribution from all incoming links
        // Using CRS format for sparse matrix-vector multiplication
        #pragma omp parallel for
        for (int i = 0; i < N; i++) {
            for (int idx = row_ptr[i]; idx < row_ptr[i+1]; idx++) {
                int j = col_idx[idx];
                new_scores[i] += d * val[idx] * scores[j];
            }
        }

        max_diff = 0.0;
        
        #pragma omp parallel
        {
            double local_max_diff = 0.0;
            
            #pragma omp for
            for (int i = 0; i < N; i++) {
                double diff = fabs(new_scores[i] - scores[i]);
                if (diff > local_max_diff) {
                    local_max_diff = diff;
                }
                scores[i] = new_scores[i];
            }
            
            #pragma omp critical
            {
                if (local_max_diff > max_diff) {
                    max_diff = local_max_diff;
                }
            }
        }

        iteration++;
        
        if (iteration % 100 == 0) {
            printf("Iteration %d, max difference: %.10e\n", iteration, max_diff);
        }

    } while (max_diff > epsilon);

    printf("Converged after %d iterations, max difference: %.10e\n", iteration, max_diff);

    free(new_scores);
    free(is_dangling);
}