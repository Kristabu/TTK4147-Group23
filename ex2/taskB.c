#include <stdio.h>
#include <stdint.h>
#include <sched.h>
#include "timespec/timespec.h"
#include "busy_wait/busy_wait.h"

uint64_t rdtsc(void)
{
    uint64_t val;
    asm volatile("isb; mrs %0, cntvct_el0; isb; " : "=r"(val)::"memory");
    // You can check the current CPU frequency with $sudo dmesg | grep MHz
    return val;
}

int main(int argc, char **argv)
{
    struct tms now;
    int ns;

    int ns_max = 1500;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int) * ns_max);

    for (int i = 0; i < 10 * 1000 * 1000; i++)
    {

        if (argc != 2)
        {
            printf("Usage: %s rdtsc|clock_gettime|times\n", argv[0]);
            return 1;
        }

        if (strcmp(argv[1], "rdtsc") == 0)
        {
            int cpu_freq_hz = 600117000;
            uint64_t t1 = rdtsc();
            uint64_t t2 = rdtsc();
            ns = (t2 - t1) * 1000000000ULL / cpu_freq_hz;
        }
        else if (strcmp(argv[1], "clock_gettime") == 0)
        {
            struct timespec t1, t2;

            clock_gettime(CLOCK_MONOTONIC, &t1);
	    sched_yield();
            clock_gettime(CLOCK_MONOTONIC, &t2);

            struct timespec diff = timespec_sub(t2, t1);

            ns = diff.tv_sec * 1000000000LL + diff.tv_nsec;
        }
        else if (strcmp(argv[1], "times") == 0)
        {
            clock_t t1 = times(&now);
            clock_t t2 = times(&now);
            ns = (long long)(t2 - t1) * 1000000000LL / sysconf(_SC_CLK_TCK);
        }
        else
        {
            printf("Unknown option\n");
            return 1;
        }

        if (ns >= 0 && ns < ns_max)
        {
            histogram[ns]++;
        }
    }

    for (int i = 0; i < ns_max; i++)
    {
        printf("%d\n", histogram[i]);
    }
    /*
    Pipe the output from the program into gnuplot like this:
    ./program_name | gnuplot -e "set terminal pngcairo size 1280, 720;set output 'plot.png';set title 'My Plot';plot '<cat' with boxes"
    */
    return 0;
}
