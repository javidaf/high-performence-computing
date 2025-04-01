#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function_declarations.h"

void read_graph_from_file2(char *filename, int *N, int **row_ptr, int **col_idx, double **val) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        exit(1);
    }

    char line[256];
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

    // Skip the fourth line (header)
    fgets(line, sizeof(line), file);

    // Temporary arrays to store edges
    int *from_nodes = (int *)malloc(num_edges * sizeof(int));
    int *to_nodes = (int *)malloc(num_edges * sizeof(int));
    if (from_nodes == NULL || to_nodes == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for temporary edge arrays\n");
        fclose(file);
        exit(1);
    }

    // Read all edges
    int edge_count = 0;
    while (fgets(line, sizeof(line), file) != NULL && edge_count < num_edges) {
        if (sscanf(line, "%d %d", &from_node, &to_node) != 2) {
            continue; // Skip invalid lines
        }

        // Check for valid node indices and not self-links
        if (from_node >= 0 && from_node < num_nodes && 
            to_node >= 0 && to_node < num_nodes && 
            from_node != to_node) {
            from_nodes[edge_count] = from_node;
            to_nodes[edge_count] = to_node;
            edge_count++;
        }
    }

    int *outbound_count = (int *)calloc(num_nodes, sizeof(int));
    if (outbound_count == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for outbound_count\n");
        free(from_nodes);
        free(to_nodes);
        fclose(file);
        exit(1);
    }

    for (int i = 0; i < edge_count; i++) {
        outbound_count[from_nodes[i]]++;
    }

    // Count the number of non-zero entries in the hyperlink matrix
    int nnz = edge_count;

    // CRS arrays
    *row_ptr = (int *)malloc((num_nodes + 1) * sizeof(int));
    *col_idx = (int *)malloc(nnz * sizeof(int));
    *val = (double *)malloc(nnz * sizeof(double));
    if (*row_ptr == NULL || *col_idx == NULL || *val == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for CRS arrays\n");
        free(from_nodes);
        free(to_nodes);
        free(outbound_count);
        fclose(file);
        exit(1);
    }

    // count how many incoming links each node has to fill row_ptr correctly
    int *incoming_count = (int *)calloc(num_nodes, sizeof(int));
    if (incoming_count == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for incoming_count\n");
        free(from_nodes);
        free(to_nodes);
        free(outbound_count);
        free(*row_ptr);
        free(*col_idx);
        free(*val);
        fclose(file);
        exit(1);
    }

    // Count incoming links for each node
    for (int i = 0; i < edge_count; i++) {
        incoming_count[to_nodes[i]]++;
    }

    // Set up row_ptr array
    (*row_ptr)[0] = 0;
    for (int i = 0; i < num_nodes; i++) {
        (*row_ptr)[i+1] = (*row_ptr)[i] + incoming_count[i];
    }

    // Reset incoming_count to be used as counters when filling col_idx and val
    for (int i = 0; i < num_nodes; i++) {
        incoming_count[i] = 0;
    }

    // Fill col_idx and val arrays
    for (int i = 0; i < edge_count; i++) {
        int from = from_nodes[i];
        int to = to_nodes[i];
        int pos = (*row_ptr)[to] + incoming_count[to];
        (*col_idx)[pos] = from;
        (*val)[pos] = 1.0 / outbound_count[from];
        incoming_count[to]++;
    }

    free(from_nodes);
    free(to_nodes);
    free(outbound_count);
    free(incoming_count);
    fclose(file);
}