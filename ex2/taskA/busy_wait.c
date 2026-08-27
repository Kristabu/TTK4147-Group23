#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/times.h>
#include "timespec.c"

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

    while (1)
    {
        times(&start);

        clock_t elapsed = (now.tms_utime + now.tms_stime) - (start.tms_utime + start.tms_stime);

        if (elapsed >= ticks_per_second)
            break;
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s sleep|busy\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "clock") == 0)
    {
        busy_wait_clock((struct timespec){1, 0});
    }
    else if (strcmp(argv[1], "times") == 0)
    {
        busy_wait_times();
    }
    else
    {
        printf("Unknown option\n");
        return 1;
    }

    return 0;
}