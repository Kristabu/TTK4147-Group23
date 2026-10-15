#define _GNU_SOURCE

#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <wiringPi.h>

#define TEST_1 8
#define RESPONSE_1 9

#define TEST_2 7
#define RESPONSE_2 15

#define TEST_3 16
#define RESPONSE_3 1

int set_cpu(int cpu_id)
{
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (cpu_id < 0 || cpu_id >= num_cores)
        return EINVAL;

    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_id, &cpu);
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

void sleep_realtime(long nanoseconds)
{
	struct timespec ts;
	ts.tv_sec = nanoseconds / 1e9;
	ts.tv_nsec = nanoseconds % (long)1e9;
	clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL);
}

struct responseTaskArgs
{
    struct
    {
        uint32_t test;
        uint32_t response;
    } pin;

    uint32_t work_time;
};

static void busy_delay_ms(uint32_t ms)
{
    struct timespec start, now;

    clock_gettime(CLOCK_MONOTONIC, &start);

    do
    {
        clock_gettime(CLOCK_MONOTONIC, &now);

        uint64_t elapsed_ns =
            (uint64_t)(now.tv_sec - start.tv_sec) * 1000000000ULL;

        if (now.tv_nsec >= start.tv_nsec)
        {
            elapsed_ns += (uint64_t)(now.tv_nsec - start.tv_nsec);
        }
        else
        {
            elapsed_ns -= (uint64_t)(start.tv_nsec - now.tv_nsec);
        }

        if (elapsed_ns >= (uint64_t)ms * 1000000ULL)
        {
            break;
        }

    } while (1);
}

static void *disturbTask(void *args)
{
	int ret = set_cpu(0);

	if (ret != 0)
    	{
        	fprintf(stderr, "Failed to set CPU affinity: %s\n",
                strerror(ret));
        	return NULL;
    	}
	while(1){
		asm volatile("" ::: "memory");
	}
}

static void *responseTask(void *args)
{
    int ret = set_cpu(0);

    if (ret != 0)
    {
        fprintf(stderr, "Failed to set CPU affinity: %s\n",
                strerror(ret));
        return NULL;
    }

    struct responseTaskArgs *a = args;

    while (1)
    {
        if (digitalRead(a->pin.test) == LOW)
        {
            if (a->work_time > 0)
            {
                busy_delay_ms(a->work_time);
            }

            digitalWrite(a->pin.response, LOW);
        }
        else
        {
            digitalWrite(a->pin.response, HIGH); 
	}
	sleep_realtime(2000000L);
    }

    return NULL;
}

static int io_init(void)
{
    if (wiringPiSetup() == -1)
    {
        fprintf(stderr, "Failed to initialize WiringPi\n");
        return -1;
    }

    pinMode(TEST_1, INPUT);
    pinMode(RESPONSE_1, OUTPUT);

    pinMode(TEST_2, INPUT);
    pinMode(RESPONSE_2, OUTPUT);

    pinMode(TEST_3, INPUT);
    pinMode(RESPONSE_3, OUTPUT);

    digitalWrite(RESPONSE_1, HIGH);
    digitalWrite(RESPONSE_2, HIGH);
    digitalWrite(RESPONSE_3, HIGH);

    return 0;
}

int main(void)
{
    if (io_init() != 0)
    {
        return 1;
    }

    struct responseTaskArgs args1 = {{TEST_1, RESPONSE_1}, 0};
    struct responseTaskArgs args2 = {{TEST_2, RESPONSE_2}, 0};
    struct responseTaskArgs args3 = {{TEST_3, RESPONSE_3}, 0};

    pthread_t thread1, thread2, thread3, dthread4, dthread5;

    if (pthread_create(&thread1, NULL, responseTask, &args1) != 0)
    {
        perror("pthread_create thread1");
        return 1;
    }

    if (pthread_create(&thread2, NULL, responseTask, &args2) != 0)
    {
        perror("pthread_create thread2");
        return 1;
    }

    if (pthread_create(&thread3, NULL, responseTask, &args3) != 0)
    {
        perror("pthread_create thread3");
        return 1;
    }
//	pthread_create(&dthread4, NULL, disturbTask, NULL);
//	pthread_create(&dthread5, NULL, disturbTask, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

  //  pthread_join(dthread4, NULL);
//    pthread_join(dthread5, NULL);

    return 0;
}
