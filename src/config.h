/*
 * config.h
 * Parâmetros globais de configuração da simulação.
 */

#ifndef CONFIG_H
#define CONFIG_H

#define MAX_QUEUE 20      // Tamanho máximo de cada fila
#define NUM_SKIERS 120    // Número total de esquiadores
#define SKIER_INTERVAL 1  // Intervalo entre chegadas de esquiadores (segundos)
#define LIFT_INTERVAL 5   // Intervalo entre partidas do elevador (segundos)
#define SEATS 4           // Número de lugares no elevador
#define TIME_LIMIT 150    // Tempo máximo de simulação (segundos), no caso 2 minutos e meio como declarado no PDF

// Enumeração dos tipos de fila
typedef enum { LS = 0, LT, RT, RS } QueueType;

#endif
