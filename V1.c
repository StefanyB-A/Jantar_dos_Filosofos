#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t *forks;
int N; // Numero de filósofos/garfos

void *philosopher(void *arg) {
    int id = *(int*)arg; // Pega o ID do filósofo
    int left = id;
    int right = (id + 1) % N; // Lógica circular para o garfo da direita

    while (1) {
        // Pensar
        printf("Filósofo %d está pensando.\n", id);
        // Sleep aleatório para variar o tempo de pensamento
        usleep(rand() % 500000);

        // Área crítica ->
        // Pegar garfos (pode causar deadlock)
        printf("Filósofo %d está pegando os garfos.\n", id);
        pthread_mutex_lock(&forks[left]);
        sleep(1);
        pthread_mutex_lock(&forks[right]);

        // Comer
        printf("Filósofo %d está comendo.\n", id);
        sleep(1);

        // Liberar garfos
        printf("Filósofo %d está soltando os garfos.\n", id);
        pthread_mutex_unlock(&forks[right]);
        pthread_mutex_unlock(&forks[left]);
    }
    return NULL;
}

int main(int argc, char *argv[]){
    printf("Digite o numero de filosofos/garfos na mesa:");
    scanf("%d",&N);
    if (N < 2) {
        printf("São necessários pelo menos 2 filósofos para haver deadlock.\n");
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

    // Cria os filósofos
    for (int i = 0; i < N; i++) {
        ids[i] = i; // Atribui o ID
        // Passamos o endereço de ids[i] para garantir que o valor não mude
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    free(forks);
    free(threads);
    free(ids);
    return 0;
}
