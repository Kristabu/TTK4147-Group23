#include "busy_wait/busy_wait.h"
#include "timespec/timespec.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s clock|times\n", argv[0]);
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