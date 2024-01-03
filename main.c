#include <stdio.h>
#include <stdbool.h>

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

void philosopher(int i)
{
    while (1)
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

void put_forks(i)
{
    down(&mutex);
    state[i] = THIKING;
    test(LEFT);
    test(RIGHT);
    up(&mutex);
}

void test(i)
{
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[i] = EATING;
        up(&s[i]);
    }
}

void think()
{
    printf('Filósofo está pensando...\n');
}

void eat()
{
    printf('Filósofo está comendo...\n');
}

void down(semaphore *s)
{
    // decrementar semáforo
}

void up(semaphore *s)
{
    // incrementar semáforo
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        state[i] = THIKING;
    }

    for (int i = 0; i < N; i++)
    {
        // thread/fork
    }

    for (int i = 0; i < N; i++)
    {
        // esperar thread/fork
    }

    return 0;
}