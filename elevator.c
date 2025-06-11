/*
 * elevator.c
 * Implementa a lógica do elevador: embarque de esquiadores, alternância entre filas triplas e simples,
 * transferência de esquiadores remanescentes e controle de término da simulação.
 */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "elevator.h"
#include "queue.h"
#include "metrics.h"
#include "config.h"
#include "log.h"

// Variáveis globais para alternância de atendimento das filas
int last_served_triple = -1;
int last_served_single = -1;

// Controle de término da criação dos esquiadores
int all_skiers_created = 0;
pthread_mutex_t flag_mutex = PTHREAD_MUTEX_INITIALIZER;

// Adicione variável global para tempo de início
time_t simulation_start_time = 0;

/*
 * Transfere esquiadores remanescentes das filas triplas (LT, RT) para as filas simples (LS, RS)
 * caso não seja possível formar um grupo de 3 para embarque.
 * Isso ocorre apenas após todos os esquiadores terem sido criados.
 */
void transfer_remaining_from_triples() {
    for (QueueType q = LT; q <= RT; q++) {
        pthread_mutex_lock(&queues[q].mutex);
        while (queues[q].count > 0 && queues[q].count < 3) {
            Skier s = queues[q].queue[queues[q].front];
            queues[q].front = (queues[q].front + 1) % MAX_QUEUE;
            queues[q].count--;
            pthread_mutex_unlock(&queues[q].mutex);

            QueueType dest = (queue_size(LS) <= queue_size(RS)) ? LS : RS;
            enqueue(dest, s);
            log_printf("[TRANSFERÊNCIA] %d de %d para %d\n", s.id, q, dest);
            pthread_mutex_lock(&queues[q].mutex);
        }
        pthread_mutex_unlock(&queues[q].mutex);
    }
}

/*
 * Embarca um esquiador da fila q, atualiza as métricas e armazena o id no vetor ids.
 */
void board(QueueType q, int* ids, int* count) {
    Skier s = dequeue(q);
    time_t now = time(NULL);
    update_metrics(q, now - s.arrival_time);
    ids[(*count)++] = s.id;
}

/*
 * Lógica de embarque do elevador:
 * - Prioriza grupos de 3 das filas triplas (LT, RT), alternando entre elas.
 * - Preenche lugares restantes com esquiadores das filas simples (LS, RS), alternando.
 * - Atualiza as métricas e imprime o estado das filas.
 */
void serve_lift() {
    int filled = 0, ids[4] = {-1};
    int lt = queue_size(LT), rt = queue_size(RT);
    int ls = queue_size(LS), rs = queue_size(RS);
    int serve = -1;

    // Alternância entre filas triplas
    if (lt >= 3 && rt >= 3)
        serve = (last_served_triple == LT) ? RT : LT;
    else if (lt >= 3) serve = LT;
    else if (rt >= 3) serve = RT;

    int needed_simples = SEATS;
    if (serve != -1) needed_simples -= 3;

    // Verifica se há pessoas suficientes nas filas simples para preencher os lugares restantes
    int simples_disponiveis = ls + rs;
    if (needed_simples > simples_disponiveis) {
        // Não há pessoas suficientes para preencher o elevador
        log_printf("[ELEVADOR] Não há pessoas suficientes para preencher o elevador neste ciclo. Aguardando próximo ciclo.\n");
        return;
    }

    // Embarca grupo triplo se possível
    if (serve != -1) {
        for (int i = 0; i < 3; i++) board(serve, ids, &filled);
        last_served_triple = serve;
    }

    // Preenche lugares restantes com filas simples
    while (filled < SEATS) {
        int next = (last_served_single == RS) ? LS : RS;
        if (queue_size(next) > 0) {
            board(next, ids, &filled);
            last_served_single = next;
        } else if (queue_size(1 - next) > 0) {
            board(1 - next, ids, &filled);
            last_served_single = 1 - next;
        }
    }

    increment_passed();
    log_printf("[ELEVADOR] Partiu com:");
    for (int i = 0; i < filled; i++) log_printf(" %d", ids[i]);
    log_printf("\n");
    print_filas();
}

/*
 * Thread principal do elevador.
 * Executa a cada LIFT_INTERVAL segundos, embarcando esquiadores conforme a lógica.
 * Após todos os esquiadores serem criados, transfere remanescentes das filas triplas.
 * Encerra quando todas as filas estiverem vazias.
 */
void* elevator_thread(void* arg) {
    // Recebe o tempo de início da simulação via argumento
    simulation_start_time = *(time_t*)arg;
    while (1) {
        sleep(LIFT_INTERVAL);
        serve_lift();

        // Verifica se atingiu o tempo limite
        time_t now = time(NULL);
        if (now - simulation_start_time >= TIME_LIMIT) {
            log_printf("[ELEVADOR] Tempo limite de simulação atingido (%d segundos).\n", TIME_LIMIT);
            break;
        }

        pthread_mutex_lock(&flag_mutex);
        int ended = all_skiers_created;
        pthread_mutex_unlock(&flag_mutex);

        if (ended) transfer_remaining_from_triples();

        int total = 0;
        for (int i = 0; i < 4; i++) total += queue_size(i);
        if (ended && total == 0) break;
    }
    return NULL;
}
