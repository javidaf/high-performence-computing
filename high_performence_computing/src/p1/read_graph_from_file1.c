#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function_declarations.h"

void read_graph_from_file1(char *filename, int *N, double ***hyperlink_matrix) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        exit(1);
    }

    char line[256]; // Buffer to read lines from the file
    int num_nodes = 0, num_edges = 0;
    int from_node, to_node;

    // Skip first two comment lines
    fgets(line, sizeof(line), file);
    fgets(line, sizeof(line), file);

    // Read the third line containing the number of nodes and edges
    fgets(line, sizeof(line), file);
    if (sscanf(line, "# Nodes: %d Edges: %d", &num_nodes, &num_edges) != 2) {
        fprintf(stderr, "Error: Invalid format in file header\n");
        fclose(file);
        exit(1);
    }

    *N = num_nodes;

    *hyperlink_matrix = (double **)malloc(num_nodes * sizeof(double *));
    if (*hyperlink_matrix == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for hyperlink_matrix\n");
        fclose(file);
        exit(1);
    }
    for (int i = 0; i < num_nodes; i++) {
        (*hyperlink_matrix)[i] = (double *)calloc(num_nodes, sizeof(double));
        if ((*hyperlink_matrix)[i] == NULL) {
            fprintf(stderr, "Error: Memory allocation failed for hyperlink_matrix[%d]\n", i);
            for (int j = 0; j < i; j++) {
                free((*hyperlink_matrix)[j]);
            }
            free(*hyperlink_matrix);
            fclose(file);
            exit(1);
        }
    }

    // Skip the fourth line (header)
    fgets(line, sizeof(line), file);

    // Count outbound links for each node first to compute normalization factors
    int *outbound_count = (int *)calloc(num_nodes, sizeof(int));
    if (outbound_count == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for outbound_count\n");
        for (int i = 0; i < num_nodes; i++) {
            free((*hyperlink_matrix)[i]);
        }
        free(*hyperlink_matrix);
        fclose(file);
        exit(1);
    }

    // First read: count outbound links
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d %d", &from_node, &to_node) != 2) {
            continue;
        }

        // Check for valid node indices and not self-links
        if (from_node >= 0 && from_node < num_nodes && 
            to_node >= 0 && to_node < num_nodes && 
            from_node != to_node) {
            outbound_count[from_node]++;
        }
    }

    // Reset file pointer for second read
    fseek(file, 0, SEEK_SET);
    
    // Skip the first four lines again
    for (int i = 0; i < 4; i++) {
        fgets(line, sizeof(line), file);
    }

    // Second read: build the hyperlink matrix
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d %d", &from_node, &to_node) != 2) {
            continue; 
        }

        // Check for valid node indices and not self-links
        if (from_node >= 0 && from_node < num_nodes && 
            to_node >= 0 && to_node < num_nodes && 
            from_node != to_node) {
            // The hyperlink matrix A has A_ij = 1/L_j if there is a link from j to i
            if (outbound_count[from_node] > 0) {
                (*hyperlink_matrix)[to_node][from_node] = 1.0 / outbound_count[from_node];
            }
        }
    }

    free(outbound_count);
    fclose(file);
}