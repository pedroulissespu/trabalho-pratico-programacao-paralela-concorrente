/*
 * log.h
 * Declaração das funções para logging thread-safe em terminal e arquivo.
 */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>

extern FILE* log_file;

// Inicializa o sistema de log
void init_log(const char* filename);
// Fecha o arquivo de log
void close_log();
// Imprime mensagem no terminal e no arquivo de log
void log_printf(const char* format, ...);

#endif
