#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int sleep_test(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s sleep|usleep|nanosleep\n", argv[0]);
        return 1;
    }

    printf("Starting...\n");

    if (strcmp(argv[1], "sleep") == 0)
    {
        sleep(1);
    }
    else if (strcmp(argv[1], "usleep") == 0)
    {
        usleep(1000000); // 1 second = 1,000,000 microseconds
    }
    else if (strcmp(argv[1], "nanosleep") == 0)
    {
        struct timespec ts = {
            .tv_sec = 1,
            .tv_nsec = 0};

        nanosleep(&ts, NULL);
    }
    else
    {
        printf("Unknown method: %s\n", argv[1]);
        return 1;
    }

    printf("Done!\n");

    return 0;
}
