/*
 * log.c
 * Implementa funções de logging thread-safe para terminal e arquivo.
 */

#include "log.h"
#include <stdarg.h>

FILE* log_file = NULL;

// Inicializa o arquivo de log
void init_log(const char* filename) {
    log_file = fopen(filename, "w");
    if (!log_file) {
        perror("Erro ao abrir o arquivo de log");
    }
}

// Fecha o arquivo de log
void close_log() {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

// Imprime mensagem formatada no terminal e no arquivo de log
void log_printf(const char* format, ...) {
    va_list args;

    // Terminal
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // Arquivo
    if (log_file) {
        va_start(args, format);
        vfprintf(log_file, format, args);
        fflush(log_file);
        va_end(args);
    }
}
