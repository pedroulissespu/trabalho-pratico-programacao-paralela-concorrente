CC = gcc
CFLAGS = -Wall -pthread -g
OBJS = main.o elevator.o skier.o queue.o metrics.o log.o

all: esquiador

esquiador: $(OBJS)
	$(CC) $(CFLAGS) -o esquiador $(OBJS)

main.o: main.c elevator.h skier.h queue.h metrics.h config.h log.h
elevator.o: elevator.c elevator.h queue.h metrics.h config.h
skier.o: skier.c skier.h queue.h config.h
queue.o: queue.c queue.h config.h
metrics.o: metrics.c metrics.h config.h

clean:
	rm -f *.o esquiador
