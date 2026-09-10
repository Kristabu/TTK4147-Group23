#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include "../timespec/timespec.h"

void busy_wait_clock(struct timespec t);
void busy_wait_times(void);