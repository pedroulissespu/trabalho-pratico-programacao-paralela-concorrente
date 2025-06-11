/*
 * queue.c
 * Implementa as operações das filas concorrentes de esquiadores.
 * Cada fila é protegida por um mutex para garantir acesso seguro entre threads.
 */

#include <stdio.h>
#include "queue.h"
#include "log.h"

Queue queues[4];

/*
 * Inicializa todas as filas e seus mutexes.
 */
void init_queues() {
    for (int i = 0; i < 4; i++) {
        queues[i].front = queues[i].rear = queues[i].count = 0;
        pthread_mutex_init(&queues[i].mutex, NULL);
    }
}

/*
 * Insere um esquiador na fila q, se houver espaço.
 */
void enqueue(QueueType q, Skier skier) {
    pthread_mutex_lock(&queues[q].mutex);
    if (queues[q].count < MAX_QUEUE) {
        queues[q].queue[queues[q].rear] = skier;
        queues[q].rear = (queues[q].rear + 1) % MAX_QUEUE;
        queues[q].count++;
    }
    pthread_mutex_unlock(&queues[q].mutex);
}

/*
 * Remove e retorna o esquiador do início da fila q.
 */
Skier dequeue(QueueType q) {
    pthread_mutex_lock(&queues[q].mutex);
    Skier s = queues[q].queue[queues[q].front];
    queues[q].front = (queues[q].front + 1) % MAX_QUEUE;
    queues[q].count--;
    pthread_mutex_unlock(&queues[q].mutex);
    return s;
}

/*
 * Retorna o tamanho atual da fila q.
 */
int queue_size(QueueType q) {
    pthread_mutex_lock(&queues[q].mutex);
    int size = queues[q].count;
    pthread_mutex_unlock(&queues[q].mutex);
    return size;
}

/*
 * Imprime o estado atual de todas as filas no log.
 */
void print_filas() {
    const char* names[] = {"LS", "LT", "RT", "RS"};
    log_printf("--- ESTADO DAS FILAS ---\n");
    for (int i = 0; i < 4; i++) {
        pthread_mutex_lock(&queues[i].mutex);
        log_printf("%s [%d]:", names[i], queues[i].count);
        for (int j = 0, idx = queues[i].front; j < queues[i].count; j++) {
            log_printf(" %d", queues[i].queue[idx].id);
            idx = (idx + 1) % MAX_QUEUE;
        }
        log_printf("\n");
        pthread_mutex_unlock(&queues[i].mutex);
    }
    log_printf("-------------------------\n");
}
