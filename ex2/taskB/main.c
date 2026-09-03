#include <stdio.h>
#include <stdint.h>
#include "../timespec/timespec.h"

// read access latency
int access_latency()
{
    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int) * ns_max);

    for (int i = 0; i < 10 * 1000 * 1000; i++)
    {

        // t1 = timer()
        // t2 = timer()

        int ns = // (t2 - t1) * ??

            if (ns >= 0 && ns < ns_max)
        {
            histogram[ns]++;
        }
    }

    for (int i = 0; i < ns_max; i++)
    {
        printf("%d\n", histogram[i]);
    }
}

uint64_t rdtsc(void)
{
    uint64_t val;
    asm volatile("isb; mrs %0, cntvct_el0; isb; " : "=r"(val)::"memory");
    // You can check the current CPU frequency with $sudo dmesg | grep MHz
    return val;
}

int main()
{
    /*
    Pipe the output from the program into gnuplot like this:
    ./program_name | gnuplot -e "set terminal pngcairo size 1280, 720;set output 'plot.png';set title 'My Plot';plot '<cat' with boxes"
    */
    return 0;
}