#include "busy_wait.h"

void busy_wait_clock(struct timespec t)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    struct timespec then = timespec_add(now, t);

    while (timespec_cmp(now, then) < 0)
    {
        for (int i = 0; i < 10000; i++)
        {
        }
        clock_gettime(CLOCK_MONOTONIC, &now);
    }
}

void busy_wait_times(void)
{
    struct tms start;
    struct tms now;

    long ticks_per_second = sysconf(_SC_CLK_TCK);

    times(&start);

    while (1)
    {
        times(&now);

        clock_t elapsed =
            (now.tms_utime + now.tms_stime) -
            (start.tms_utime + start.tms_stime);

        if (elapsed >= ticks_per_second)
            break;
    }
}