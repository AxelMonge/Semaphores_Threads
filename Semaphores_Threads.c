#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_PAJAROS 10
#define MAX_COMEDERO 3

sem_t semComedero;
sem_t semReponer;

void *pajaro(void *arg) {
    int id = (int)(long)arg;

    while (1) {
        sleep(rand() % 5 + 1);

        sem_wait(&semComedero);
        printf("Pájaro %d entra al comedero\n", id);
        fflush(stdout);

        sleep(rand() % 3 + 1);

        printf("Pájaro %d sale del comedero\n", id);
        fflush(stdout);
        sem_post(&semComedero);
    }
}

void *encargado(void *arg) {
    while (1) {
        sleep(rand() % 10 + 5);

        for (int i = 0; i < MAX_COMEDERO; i++) {
            sem_wait(&semComedero);
        }

        printf("El encargado está reponiendo alpiste\n");
        fflush(stdout);

        sleep(rand() % 5 + 1);

        printf("El encargado terminó de reponer alpiste\n");
        fflush(stdout);

        for (int i = 0; i < MAX_COMEDERO; i++) {
            sem_post(&semComedero);
        }
    }
}

int main() {
    pthread_t hiloPajaros[NUM_PAJAROS];
    pthread_t hiloEncargado;

    srand(time(NULL));

    sem_init(&semComedero, 0, MAX_COMEDERO);
    sem_init(&semReponer, 0, 0);

    pthread_create(&hiloEncargado, NULL, encargado, NULL);

    for (int i = 0; i < NUM_PAJAROS; i++) {
        pthread_create(&hiloPajaros[i], NULL, pajaro, (void *)(long)i);
    }

    for (int i = 0; i < NUM_PAJAROS; i++) {
        pthread_join(hiloPajaros[i], NULL);
    }

    return 0;
}

  
