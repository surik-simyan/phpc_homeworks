#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define SENSORS 3
#define PREP_TIME 3
#define TEMP_RANGE_LOW -20
#define TEMP_RANGE_HIGH 40

pthread_barrier_t sensor_barrier;
int temperatures[SENSORS];

void *sensor(void *arg)
{
    int id = *(int *)arg;
    int prep_time = (rand() % PREP_TIME) + 1;
    sleep(prep_time);
    int temperature = rand() % ((TEMP_RANGE_HIGH + 1) - TEMP_RANGE_LOW) + TEMP_RANGE_LOW;
    temperatures[id] = temperature;
    printf("Sensor %d - Temperature is %d\n", id + 1, temperature);
    pthread_barrier_wait(&sensor_barrier);
    if(id == 0) {
        int sum_temp = 0;
        for (int i = 0; i < SENSORS; i++) {
            sum_temp += temperatures[i];
        }
        double avg_temp = (double) sum_temp / SENSORS;
        printf("Average temperature is %.2f\n", avg_temp);
    }
    return NULL;
}

int main()
{
    srand(time(0));
    pthread_t threads[SENSORS];
    int sensor_ids[SENSORS];

    pthread_barrier_init(&sensor_barrier, NULL, SENSORS);

    for (int i = 0; i < SENSORS; i++)
    {
        sensor_ids[i] = i;
        pthread_create(&threads[i], NULL, sensor, &sensor_ids[i]);
    }

    for (int i = 0; i < SENSORS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&sensor_barrier);
    return 0;
}