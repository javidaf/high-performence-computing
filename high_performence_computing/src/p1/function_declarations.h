#ifndef FUNCTION_DECLARATIONS_H
#define FUNCTION_DECLARATIONS_H

/**
 * Reads a web graph from file and builds a hyperlink matrix
 * 
 * @param filename The name of the file containing the web graph
 * @param N Pointer to store the number of webpages
 * @param hyperlink_matrix Pointer to store the hyperlink matrix (will be allocated inside the function)
 */
void read_graph_from_file1(char *filename, int *N, double ***hyperlink_matrix);

/**
 * Reads a web graph from file and builds a hyperlink matrix in CRS format
 * 
 * @param filename The name of the file containing the web graph
 * @param N Pointer to store the number of webpages
 * @param row_ptr Pointer to store the row pointers (will be allocated inside the function)
 * @param col_idx Pointer to store the column indices (will be allocated inside the function)
 * @param val Pointer to store the values (will be allocated inside the function)
 */
void read_graph_from_file2(char *filename, int *N, int **row_ptr, int **col_idx, double **val);

/**
 * Performs PageRank iterations using a dense hyperlink matrix
 * 
 * @param N Number of webpages
 * @param hyperlink_matrix The dense hyperlink matrix
 * @param d Damping factor
 * @param epsilon Convergence threshold
 * @param scores Array to store the computed PageRank scores (pre-allocated)
 */
void PageRank_iterations1(int N, double **hyperlink_matrix, double d, double epsilon, double *scores);

/**
 * Performs PageRank iterations using a hyperlink matrix in CRS format
 * 
 * @param N Number of webpages
 * @param row_ptr Row pointers array of the CRS format
 * @param col_idx Column indices array of the CRS format
 * @param val Values array of the CRS format
 * @param d Damping factor
 * @param epsilon Convergence threshold
 * @param scores Array to store the computed PageRank scores (pre-allocated)
 */
void PageRank_iterations2(int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores);

/**
 * Prints the top n webpages with the highest PageRank scores
 * 
 * @param N Number of webpages
 * @param scores Array of PageRank scores
 * @param n Number of top webpages to print
 */
void top_n_webpages(int N, double *scores, int n);

#endif /* FUNCTION_DECLARATIONS_H */