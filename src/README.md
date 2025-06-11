# Código Fonte do Problema da Simulação de Elevador de Esquiadores

## Funcionamento Geral

- Esquiadores chegam em intervalos regulares e escolhem uma das filas disponíveis (simples ou triplas) conforme uma estratégia.
- O elevador parte em intervalos fixos, embarcando grupos de esquiadores das filas conforme regras de prioridade e alternância.
- O sistema coleta métricas como tempo médio de espera, taxa de ocupação do elevador e quantidade de esquiadores atendidos por fila.
- Toda a lógica é concorrente, utilizando mutexes para garantir a integridade dos dados compartilhados.

## Arquivos do Projeto

- [`main.c`](./main.c): Função principal da simulação. Inicializa o sistema, cria as threads dos esquiadores e do elevador, e exibe as métricas finais.
- [`elevator.c`](./elevator.c) / [`elevator.h`](./elevator.h): Implementação e declaração da lógica do elevador, incluindo embarque, alternância entre filas e transferência de esquiadores.
- [`skier.c`](./skier.c) / [`skier.h`](./skier.h): Implementação e declaração da lógica de cada esquiador, incluindo chegada, escolha de fila e entrada na fila.
- [`queue.c`](./queue.c) / [`queue.h`](./queue.h): Implementação e declaração das filas concorrentes de esquiadores, protegidas por mutex.
- [`metrics.c`](./metrics.c) / [`metrics.h`](./metrics.h): Coleta e exibição das métricas de desempenho da simulação.
- [`log.c`](./log.c) / [`log.h`](./log.h): Sistema de logging thread-safe para terminal e arquivo.
- [`config.h`](./config.h): Parâmetros globais de configuração da simulação (número de esquiadores, intervalos, tamanhos de fila, etc).
- [`Makefile`](./Makefile): Script para compilação do projeto.

## Resumo dos Arquivos

- [`main.c`](./main.c): Gerencia o ciclo de vida da simulação, criação e sincronização das threads.
- [`elevator.c`](./elevator.c): Controla o embarque dos esquiadores no elevador, alternância entre filas e término da simulação.
- [`skier.c`](./skier.c): Define o comportamento de cada esquiador, desde a chegada até a entrada na fila.
- [`queue.c`](./queue.c): Gerencia as operações de enfileiramento e desenfileiramento de esquiadores de forma thread-safe.
- [`metrics.c`](./metrics.c): Calcula e imprime métricas como tempo médio de espera e taxa de ocupação.
- [`log.c`](./log.c): Permite registrar eventos da simulação tanto no terminal quanto em arquivo.
- [`config.h`](./config.h): Centraliza as constantes e enums de configuração.
- [`Makefile`](./Makefile): Facilita a compilação e limpeza dos arquivos objeto.

## Como Compilar e Executar

```sh
make
./esquiador
```

## Para remover os binários após a execução
```sh
make clean
```

O log detalhado da simulação será salvo em `ski_lift.log`.

---
