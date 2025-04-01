#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "function_declarations.h"

void PageRank_iterations1(int N, double **hyperlink_matrix, double d, double epsilon, double *scores) {

    for (int i = 0; i < N; i++) {
        scores[i] = 1.0 / N;
    }

    double *new_scores = (double *)malloc(N * sizeof(double));

    int *is_dangling = (int *)calloc(N, sizeof(int));
    for (int j = 0; j < N; j++) {
        int has_outlinks = 0;
        for (int i = 0; i < N; i++) {
            if (hyperlink_matrix[i][j] > 0) {
                has_outlinks = 1;
                break;
            }
        }
        if (!has_outlinks) {
            is_dangling[j] = 1;
        }
    }

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

        // Compute the PageRank scores for this iteration
        for (int i = 0; i < N; i++) {
            double sum = 0.0;
            for (int j = 0; j < N; j++) {
                sum += hyperlink_matrix[i][j] * scores[j];
            }
            new_scores[i] = (1 - d + d * W) / N + d * sum;
        }

        // Check for convergence
        max_diff = 0.0;
        for (int i = 0; i < N; i++) {
            double diff = fabs(new_scores[i] - scores[i]);
            if (diff > max_diff) {
                max_diff = diff;
            }
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