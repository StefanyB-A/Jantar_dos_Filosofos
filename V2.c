#include <pthread.h>     // Biblioteca para threads (mutex, criação de threads)
#include <semaphore.h>   // Biblioteca de Semaforos
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int N; // Numero de filósofos/garfos
sem_t limitante;          // Semáforo para limitar filósofos
pthread_mutex_t *forks;  // Ponteiro para os mutexes (garfos)

void *philosopher(void *arg) {
    int id = *(int*)arg;
    int left = id;
    int right = (id + 1) % N;

    while (1) {
        // Pensar
        printf("Filósofo %d está pensando.\n", id);
        usleep(rand() % 500000); // Tempo aleatório

        // Espera receber o sinal de controle do semaforo 
        sem_wait(&limitante);

        // Pegar garfos
        printf("Filósofo %d está pegando os garfos.\n", id);
        pthread_mutex_lock(&forks[left]);
        pthread_mutex_lock(&forks[right]);

        // Comer
        printf("Filósofo %d está comendo.\n", id);
        sleep(1);

        // Liberar garfos
        printf("Filósofo %d está soltando os garfos.\n", id);
        pthread_mutex_unlock(&forks[right]);
        pthread_mutex_unlock(&forks[left]);
        sem_post(&limitante);
    }
    return NULL;
}

int main(int argc, char *argv[]){
    printf("Digite o numero de filosofos/garfos na mesa:");
    scanf("%d",&N);
    if (N < 2) {
        printf("São necessários pelo menos 2 filósofos.\n");
        return 1;
    }

    // Alocação dinâmica dos garfos (mutexes) e threads
    forks = malloc(sizeof(pthread_mutex_t) * N);
    pthread_t *threads = malloc(sizeof(pthread_t) * N);
    int *ids = malloc(sizeof(int) * N); // Vetor para guardar os IDs

    // Inicializa os mutexes (garfos)
    for (int i = 0; i < N; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Inicializa o semáforo
    sem_init(&limitante, 0, N - 1);

    // Cria os filósofos
    for (int i = 0; i < N; i++) {
        ids[i] = i; // Atribui o ID
        // Passamos o endereço de ids[i] para garantir que o valor não mude
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&limitante);
    for(int i=0; i<N; i++) pthread_mutex_destroy(&forks[i]);
    free(forks);
    free(threads);
    free(ids);
    return 0;
}
