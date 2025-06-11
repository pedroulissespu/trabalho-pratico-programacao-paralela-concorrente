/*
 * queue.h
 * Define as estruturas e funções para manipulação das filas concorrentes de esquiadores.
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <time.h>
#include "config.h"

// Estrutura que representa um esquiador
typedef struct {
    int id;
    time_t arrival_time;
} Skier;

// Estrutura de fila circular protegida por mutex
typedef struct {
    Skier queue[MAX_QUEUE];
    int front, rear, count;
    pthread_mutex_t mutex;
} Queue;

// Vetor global de filas (4 filas: LS, LT, RT, RS)
extern Queue queues[4];

// Inicializa todas as filas
void init_queues();
// Insere esquiador na fila q
void enqueue(QueueType q, Skier skier);
// Remove esquiador da fila q
Skier dequeue(QueueType q);
// Retorna tamanho da fila q
int queue_size(QueueType q);
// Imprime estado das filas
void print_filas();

#endif
