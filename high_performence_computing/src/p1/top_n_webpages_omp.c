#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "function_declarations.h"

typedef struct {
    int index;
    double score;
} WebPage;

int compare_webpages_omp(const void *a, const void *b) {
    WebPage *wp_a = (WebPage *)a;
    WebPage *wp_b = (WebPage *)b;
    // For descending order (highest score first)
    if (wp_b->score > wp_a->score) return 1;
    if (wp_b->score < wp_a->score) return -1;
    // If scores are equal, sort by index
    return wp_a->index - wp_b->index;
}

void top_n_webpages(int N, double *scores, int n) {
    if (n <= 0 || n > N) {
        n = N; // If n is invalid, show all webpages
    }

    WebPage *webpages = (WebPage *)malloc(N * sizeof(WebPage));
    if (webpages == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for webpages\n");
        return;
    }

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        webpages[i].index = i;
        webpages[i].score = scores[i];
    }

    // Sort the array based on scores in descending order
    // Note: qsort not parallelized, but the preparation was
    qsort(webpages, N, sizeof(WebPage), compare_webpages_omp);

    printf("Rank\tWebpage\tScore\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%f\n", i + 1, webpages[i].index, webpages[i].score);
    }

    free(webpages);
}