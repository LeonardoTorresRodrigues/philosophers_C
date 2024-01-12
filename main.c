#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#define N 5
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N
#define THIKING 0
#define HUNGRY 1
#define EATING 2

typedef int semaphore;

int state[N];
semaphore mutex = 1;
semaphore s[N];
bool shouldTerminate = false;

void philosopher(int i)
{
    while (!shouldTerminate)
    {
        think();
        take_forks(i);
        eat();
        put_forks(i);
    }
}

void take_forks(int i)
{
    down(&mutex);
    state[i] = HUNGRY;
    test(i);
    up(&mutex);
    down(&s[i]);
}

void put_forks(int i)
{
    down(&mutex);
    state[i] = THIKING;
    test(LEFT);
    test(RIGHT);
    up(&mutex);
}

void test(int i)
{
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[i] = EATING;
        up(&s[i]);
    }
}

void think()
{
    printf("Filósofo está pensando...\n");
    usleep(rand() % 1000000);
}

void eat()
{
    printf("Filósofo está comendo...\n");
    usleep(rand() % 1000000);
}

void down(semaphore *s)
{
    pthread_mutex_lock(s);
}

void up(semaphore *s)
{
    pthread_mutex_unlock(s);
}

int main()
{
    srand(time(NULL));

    for (int i = 0; i < N; i++)
    {
        state[i] = THIKING;
        pthread_mutex_init(&s[i], NULL);
    }

    pthread_t philosophers[N];
    for (int i = 0; i < N; i++)
    {
        pthread_create(&philosophers[i], NULL, (void *)philosopher, (void *)i);
    }

    usleep(5000000);
    shouldTerminate = true;

    for (int i = 0; i < N; i++)
    {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}