#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_WORKERS 4
#define NUM_STAGES 3

pthread_barrier_t stage_barriers[NUM_STAGES];

void simulate_work(int worker_id, int stage) {
    int work_time = (rand() % 5) + 1;
    printf("Worker %d performing stage %d\n", worker_id + 1, stage + 1);
    sleep(work_time);
}

void* worker(void* arg) {
    int id = *(int*)arg;
    
    simulate_work(id, 0);
    printf("Worker %d completed stage 1\n", id + 1);
    pthread_barrier_wait(&stage_barriers[0]);
    
    simulate_work(id, 1);
    printf("Worker %d completed stage 2\n", id + 1);
    pthread_barrier_wait(&stage_barriers[1]);
    
    simulate_work(id, 2);
    printf("Worker %d completed stage 3\n", id + 1);
    pthread_barrier_wait(&stage_barriers[2]);
    
    printf("Worker %d: Pipeline completed!\n", id + 1);
    return NULL;
}

int main() {
    srand(time(0));

    pthread_t threads[NUM_WORKERS];
    int worker_ids[NUM_WORKERS];    
    
    for (int i = 0; i < NUM_STAGES; i++) {
        pthread_barrier_init(&stage_barriers[i], NULL, NUM_WORKERS);
    }
    
    printf("Starting %d-stage pipeline with %d workers...\n\n", 
           NUM_STAGES, NUM_WORKERS);
    
    for (int i = 0; i < NUM_WORKERS; i++) {
        worker_ids[i] = i;
        pthread_create(&threads[i], NULL, worker, &worker_ids[i]);
    }
    
    for (int i = 0; i < NUM_WORKERS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    for (int i = 0; i < NUM_STAGES; i++) {
        pthread_barrier_destroy(&stage_barriers[i]);
    }
    
    printf("\nAll workers have completed the pipeline!\n");
    return 0;
}