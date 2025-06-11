/*
 * skier.c
 * Implementa a lógica de cada esquiador: chegada, escolha de fila e entrada na fila.
 * Cada esquiador é executado em uma thread separada.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "skier.h"
#include "queue.h"
#include "config.h"
#include "log.h"

extern const char* queue_names[];

/*
 * Estratégia de escolha de fila:
 * - Prefere filas simples (LS, RS) se estiverem significativamente menores que as triplas.
 * - Caso contrário, escolhe a fila tripla (LT ou RT) com menor tamanho.
 */
QueueType choose_queue() {
    int sz[4];
    for (int i = 0; i < 4; i++) sz[i] = queue_size(i);

    if (sz[LS] < 2*sz[LT] && sz[LS] < 2*sz[RT] && sz[LS] < sz[RS]) return LS;
    if (sz[RS] < 2*sz[LT] && sz[RS] < 2*sz[RT] && sz[RS] <= sz[LS]) return RS;
    return (sz[LT] <= sz[RT]) ? LT : RT;
}

/*
 * Função da thread do esquiador:
 * - Registra chegada.
 * - Escolhe a fila de acordo com a estratégia.
 * - Entra na fila escolhida.
 * - Imprime o estado das filas.
 */
void* skier_thread(void* arg) {
    int id = *(int*)arg;
    free(arg);

    Skier skier = {.id = id, .arrival_time = time(NULL)};
    log_printf("[ESQUIADOR %d] chegou.\n", id);

    QueueType q = choose_queue();
    enqueue(q, skier);
    log_printf("[ESQUIADOR %d] entrou na fila %d.\n", id, q);
    print_filas();
    return NULL;
}
