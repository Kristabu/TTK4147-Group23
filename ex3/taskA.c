#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void allocate_matrix(long **matrix, long x_dim, long y_dim)
{
    for (long y = 0; y < y_dim; y++)
    {
        matrix[y] = malloc(x_dim * sizeof(long));
	memset(matrix[y], 0, x_dim*sizeof(long));
    }
}

void free_matrix(long **matrix, long y_dim)
{
    if (matrix == NULL)
	return;

    for (long y = 0; y < y_dim; y++)
    {
        free(matrix[y]);
    }

   free(matrix);

    printf("Free complete (press any key to continue...)\n");
    getchar();
}


void main()
{
	long xy_size = 1000 * 1000 * 500; // 4 GB (sizeof(long) = 8 bytes)
	long x_dim = 100;
	long y_dim = xy_size / x_dim;

	long **matrix = malloc(y_dim * sizeof(long *));


	allocate_matrix(matrix, x_dim, y_dim);
	printf("Allocation complete (press any key to continue...)\n");
	getchar();
	free_matrix(matrix, y_dim);
}
