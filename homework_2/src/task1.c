#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PLAYERS 3
#define ROUNDS 2

pthread_barrier_t barrier;
int rolls[PLAYERS];
int wins[PLAYERS];
int current_round = 0;

void* player(void* arg) {
    int id = *(int*)arg;
    
    for (int round = 0; round < ROUNDS; round++) {
        rolls[id] = (rand() % 6) + 1;
        printf("Player %d rolled: %d\n", id + 1, rolls[id]);
        
        pthread_barrier_wait(&barrier);
        
        if (id == 0) {
            int max_roll = 0;
            for (int i = 0; i < PLAYERS; i++) {
                if (rolls[i] > max_roll) {
                    max_roll = rolls[i];
                }
            }
            
            for (int i = 0; i < PLAYERS; i++) {
                if (rolls[i] == max_roll) {
                    wins[i]++;
                    printf("Player %d wins round %d!\n", i + 1, round + 1);
                }
            }
            printf("\n");
            current_round++;
        }
        
        pthread_barrier_wait(&barrier);
    }
    return NULL;
}

int main() {
    srand(time(0));
    pthread_t threads[PLAYERS];
    int player_ids[PLAYERS];
    
    pthread_barrier_init(&barrier, NULL, PLAYERS);
    
    for (int i = 0; i < PLAYERS; i++) {
        player_ids[i] = i;
        pthread_create(&threads[i], NULL, player, &player_ids[i]);
    }
    
    for (int i = 0; i < PLAYERS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    int max_wins = 0;
    for (int i = 0; i < PLAYERS; i++) {
        printf("Player %d: %d wins\n", i + 1, wins[i]);
        if (wins[i] > max_wins) max_wins = wins[i];
    }
    
    printf("\nWinner(s):\n");
    for (int i = 0; i < PLAYERS; i++) {
        if (wins[i] == max_wins) {
            printf("Player %d\n", i + 1);
        }
    }
    
    pthread_barrier_destroy(&barrier);
    return 0;
}