#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

enum phil_state
{
    THINK,
    HUNGRY,
    EAT
};

struct philosopher
{
    int id;
    enum phil_state state;
    pthread_mutex_t *mtx1;
    pthread_mutex_t *mtx2;
};

void *philosopher_starvation(void *arg)
{
    struct philosopher *p = arg;

    while (1)
    {
        switch (p->state)
        {
        case THINK:
            printf("Philosopher %d is think\n", p->id);
            usleep(2);
            p->state = HUNGRY;
            break;

        case HUNGRY:
            printf("Philosopher %d is hungry\n", p->id);

            pthread_mutex_lock(p->mtx1);
            printf("P %d: acquired left fork\n", p->id);
            pthread_mutex_lock(p->mtx2);
            printf("P %d: acquired right fork\n", p->id);
            p->state = EAT;
            break;

        case EAT:
            printf("Philosopher %d is eating\n", p->id);
            usleep(5);

            pthread_mutex_unlock(p->mtx2);
            printf("P %d: released right fork\n", p->id);
            pthread_mutex_unlock(p->mtx1);
            printf("P %d: released left fork\n", p->id);
            p->state = THINK;
            break;
        default:
            printf("Unkown state");
            break;
        }
    }

    return NULL;
}

void *philosopher(void *arg)
{
    struct philosopher *p = arg;

    while (1)
    {
        switch (p->state)
        {
        case THINK:
            // printf("Philosopher %d is think\n", p->id);
            usleep(2);
            p->state = HUNGRY;
            break;

        case HUNGRY:
            // printf("Philosopher %d is hungry\n", p->id);

            int result = pthread_mutex_trylock(p->mtx1);
            if (result != 0)
            {
                pthread_mutex_unlock(p->mtx1);
                break;
            }
            // printf("P %d: acquired left fork\n", p->id);

            result = pthread_mutex_trylock(p->mtx2);
            if (result != 0)
            {
                pthread_mutex_unlock(p->mtx1);
                pthread_mutex_unlock(p->mtx2);
                break;
            }
            // printf("P %d: acquired right fork\n", p->id);
            p->state = EAT;

            break;

        case EAT:
            printf("Philosopher %d is eating\n", p->id);
            usleep(5);

            pthread_mutex_unlock(p->mtx2);
            // printf("P %d: released right fork\n", p->id);
            pthread_mutex_unlock(p->mtx1);
            // printf("P %d: released left fork\n", p->id);
            p->state = THINK;
            break;
        default:
            printf("Unkown state");
            break;
        }
    }

    return NULL;
}

int main(void)
{
    int n_phil = 5;
    int n_forks = 5;

    pthread_mutex_t mtxs[n_forks];
    struct philosopher philosophers[5];
    pthread_t threads[n_phil];

    // Initialize forks
    for (int i = 0; i < n_forks; i++)
    {
        pthread_mutex_init(&mtxs[i], NULL);
    }

    // Initialize philosophers
    for (int i = 0; i < n_phil; i++)
    {

        int left = i % n_forks;
        int right = (i + 1) % n_forks;

        philosophers[i].id = i;
        philosophers[i].state = HUNGRY;
        philosophers[i].mtx1 = &mtxs[left];
        philosophers[i].mtx2 = &mtxs[right];

        pthread_create(
            &threads[i],
            NULL,
            philosopher,
            &philosophers[i]);
    }

    // Wait for philosophers
    for (int i = 0; i < n_phil; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Destroy forks
    for (int i = 0; i < n_forks; i++)
    {
        pthread_mutex_destroy(&mtxs[i]);
    }

    return 0;
}