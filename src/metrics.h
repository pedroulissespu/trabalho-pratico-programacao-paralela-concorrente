/*
 * metrics.h
 * Declaração das funções para coleta e exibição das métricas de desempenho.
 */

#ifndef METRICS_H
#define METRICS_H

#include "config.h"

// Atualiza métricas ao embarcar esquiador
void update_metrics(QueueType q, long wait_time);
// Incrementa total de lugares disponíveis
void increment_passed();
// Imprime métricas finais
void print_final_metrics();

#endif
