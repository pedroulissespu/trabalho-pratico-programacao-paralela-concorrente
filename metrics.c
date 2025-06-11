/*
 * metrics.c
 * Implementa a coleta e exibição das métricas de desempenho da simulação.
 * Todas as operações são protegidas por mutex para garantir consistência.
 */

#include <stdio.h>
#include <pthread.h>
#include "metrics.h"
#include "log.h"

int total_seats_occupied = 0;
int total_seats_passed = 0;
int total_skier_served[4] = {0};
long total_wait[4] = {0};

pthread_mutex_t metrics_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Atualiza as métricas ao embarcar um esquiador:
 * - Incrementa lugares ocupados, esquiadores atendidos e soma o tempo de espera.
 */
void update_metrics(QueueType q, long wait) {
    pthread_mutex_lock(&metrics_mutex);
    total_seats_occupied++;
    total_skier_served[q]++;
    total_wait[q] += wait;
    pthread_mutex_unlock(&metrics_mutex);
}

/*
 * Incrementa o total de lugares disponíveis (passados) a cada partida do elevador.
 */
void increment_passed() {
    pthread_mutex_lock(&metrics_mutex);
    total_seats_passed += SEATS;
    pthread_mutex_unlock(&metrics_mutex);
}

/*
 * Imprime as métricas finais da simulação no log.
 */
void print_final_metrics() {
    int total = 0;
    long wait_all = 0;
    double full_use = (total_seats_occupied*100.0) / (NUM_SKIERS);
    log_printf("\n===== MÉTRICAS FINAIS =====\n");
    for (int i = 0; i < 4; i++) {
        total += total_skier_served[i];
        wait_all += total_wait[i];
        log_printf("Fila %d: Tempo médio = %.2f s (%d esquiadores)\n",
               i, total_skier_served[i] ? (double)total_wait[i] / total_skier_served[i] : 0.0,
               total_skier_served[i]);
    }
    log_printf("Taxa de aproveitamento: %.3f%%\n",
            full_use);
    log_printf("Tempo médio total de espera: %.2f s\n",
           total ? (double)wait_all / total : 0.0);
    log_printf("===========================\n");
}
