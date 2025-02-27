#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define PLAYERS 3
#define PREP_TIME 3

pthread_barrier_t lobby_barrier;

void *player(void *arg)
{
    int id = *(int *)arg;
    int prep_time = (rand() % PREP_TIME) + 1;
    sleep(prep_time);
    printf("Player %d is ready!\n", id + 1);
    pthread_barrier_wait(&lobby_barrier);
    if(id == 0) {
        printf("Game Started!\n");
    }
    return NULL;
}

int main()
{
    srand(time(0));
    pthread_t threads[PLAYERS];
    int player_ids[PLAYERS];

    pthread_barrier_init(&lobby_barrier, NULL, PLAYERS);

    for (int i = 0; i < PLAYERS; i++)
    {
        player_ids[i] = i;
        pthread_create(&threads[i], NULL, player, &player_ids[i]);
    }

    for (int i = 0; i < PLAYERS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&lobby_barrier);
    return 0;
}