#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

#define N 5
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N
#define THIKING 0
#define HUNGRY 1
#define EATING 2

typedef struct
{
    sem_t sem;
} semaphore;

int state[N];
semaphore mutex;
semaphore s[N];
bool shouldTerminate = false;

void take_forks(int i);
void put_forks(int i);
void test(int i);
void think(int i);
void eat(int i);
void down(semaphore *s);
void up(semaphore *s);

void philosopher(void *arg)
{
    int i = *((int *)arg);

    while (!shouldTerminate)
    {
        think(i);
        take_forks(i);
        eat(i);
        put_forks(i);
    }
}

void take_forks(int i)
{
    down(&mutex);
    state[i] = HUNGRY;
    printf("Filósofo %d está faminto. \n", i);
    test(i);
    up(&mutex);
    down(&s[i]);
}

void put_forks(int i)
{
    down(&mutex);
    state[i] = THIKING;
    printf("Filósofo %d terminou de comer e está pensando. \n", i);
    test(LEFT);
    test(RIGHT);
    up(&mutex);
}

void test(int i)
{
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[i] = EATING;
        printf("Filósofo %d está comendo. \n", i);
        up(&s[i]);
    }
}

void think(int i)
{
    printf("Filósofo %d está pensando... \n", i);
    usleep(rand() % 1000000);
}

void eat(int i)
{
    printf("Filósofo %d está comendo... \n", i);
    usleep(rand() % 1000000);
}

void down(semaphore *s)
{
    sem_wait(&(s->sem));
}

void up(semaphore *s)
{
    sem_post(&(s->sem));
}

int main()
{
    srand(time(NULL));

    for (int i = 0; i < N; i++)
    {
        state[i] = THIKING;
        sem_init(&(s[i].sem), 0, 0);
    }

    sem_init(&(mutex.sem), 0, 1);

    pthread_t philosophers[N];
    int philosopher_ids[N];

    for (int i = 0; i < N; i++)
    {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, (void *)philosopher, &philosopher_ids[i]);
    }

    usleep(5000000);
    shouldTerminate = true;

    for (int i = 0; i < N; i++)
    {
        pthread_join(philosophers[i], NULL);
    }

    for (int i = 0; i < N; i++)
    {
        sem_destroy(&(s[i].sem));
    }

    sem_destroy(&(mutex.sem));

    return 0;
}