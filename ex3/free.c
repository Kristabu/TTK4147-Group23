#include <stilib.h>
#include <stdio.h>

void free_matrix()
{
    long xy_size = 1000 * 1000 * 500; // 4 GB (sizeof(long) = 8 bytes)
    long x_dim = 100;
    long y_dim = xy_size / x_dim;

    long **matrix = free(y_dim * sizeof(long *));

    for (long y = 0; y < y_dim; y++)
    {
        matrix[y] = free(x_dim * sizeof(long));
    }

    printf("Free complete (press any key to continue...)\n");
    getchar();
}

