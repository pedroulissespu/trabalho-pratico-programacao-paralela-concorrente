/*
 * main.c
 * Arquivo principal da simulação do elevador de esquiadores.
 * Responsável por inicializar o sistema, criar as threads dos esquiadores e do elevador,
 * aguardar o término e exibir as métricas finais.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "elevator.h"
#include "skier.h"
#include "queue.h"
#include "metrics.h"
#include "config.h"
#include "log.h"

// Variáveis globais para controle de término da criação dos esquiadores
extern int all_skiers_created;
extern pthread_mutex_t flag_mutex;

int main() {
    // Inicializa o sistema de log (arquivo e terminal)
    init_log("ski_lift.log");
    log_printf("Iniciando simulação do elevador...\n");

    // Inicializa as filas concorrentes
    init_queues();

    // Registra o tempo de início da simulação
    time_t start_time = time(NULL);

    pthread_t lift_thread;
    pthread_t skier_threads[NUM_SKIERS];

    // Cria a thread do elevador, passando o tempo de início
    pthread_create(&lift_thread, NULL, elevator_thread, &start_time);

    // Cria as threads dos esquiadores, um a cada SKIER_INTERVAL segundos
    for (int i = 0; i < NUM_SKIERS; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&skier_threads[i], NULL, skier_thread, id);
        sleep(SKIER_INTERVAL);
    }

    // Aguarda todas as threads dos esquiadores terminarem
    for (int i = 0; i < NUM_SKIERS; i++) pthread_join(skier_threads[i], NULL);

    // Sinaliza para o elevador que todos os esquiadores já foram criados
    pthread_mutex_lock(&flag_mutex);
    all_skiers_created = 1;
    pthread_mutex_unlock(&flag_mutex);

    // Aguarda o término da thread do elevador
    pthread_join(lift_thread, NULL);

    // Exibe as métricas finais da simulação
    print_final_metrics();
    close_log();
    return 0;
}
