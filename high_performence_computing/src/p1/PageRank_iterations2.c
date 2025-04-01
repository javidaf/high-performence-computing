#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "function_declarations.h"

void PageRank_iterations2(int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores) {
    for (int i = 0; i < N; i++) {
        scores[i] = 1.0 / N;
    }

    double *new_scores = (double *)malloc(N * sizeof(double));
    if (new_scores == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for new_scores\n");
        exit(1);
    }

    // Identify dangling node
    int *is_dangling = (int *)calloc(N, sizeof(int));
    if (is_dangling == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for is_dangling\n");
        free(new_scores);
        exit(1);
    }

    // In the CRS format, we can check if any column has at least one non-zero entry
    int *has_outlinks = (int *)calloc(N, sizeof(int));
    for (int i = 0; i < row_ptr[N]; i++) {
        has_outlinks[col_idx[i]] = 1;
    }
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
        for (int j = 0; j < N; j++) {
            if (is_dangling[j]) {
                W += scores[j];
            }
        }

        // Compute the constant part for all nodes (teleportation probability)
        double teleport = (1 - d + d * W) / N;

        // Initialize new scores with teleportation probability
        for (int i = 0; i < N; i++) {
            new_scores[i] = teleport;
        }

        // Add the contribution from all incoming links
        // Using CRS format for sparse matrix-vector multiplication
        for (int i = 0; i < N; i++) {
            for (int idx = row_ptr[i]; idx < row_ptr[i+1]; idx++) {
                int j = col_idx[idx];
                new_scores[i] += d * val[idx] * scores[j];
            }
        }

        // Check for convergence
        max_diff = 0.0;
        for (int i = 0; i < N; i++) {
            double diff = fabs(new_scores[i] - scores[i]);
            if (diff > max_diff) {
                max_diff = diff;
            }
            // Update scores for the next iteration
            scores[i] = new_scores[i];
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