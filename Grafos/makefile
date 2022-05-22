CC = gcc
CFLAGS = -g -Wall -Werror -Wextra -pedantic -std=c99 -O3 
TARGET = test
OBJS = GrafoSt21.o main.o uh.o queue.o list.o
SRCS = GrafoSt21.c main.c uh.c queue.c list.c

all : $(OBJS) 
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(OBJS) : $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)

clean : 
	-rm $(OBJS) $(TARGET)

valgrind:
	cat q10.txt | valgrind --show-reachable=yes --track-origins=yes --leak-check=full ./test
	
testear:
	cat q10.txt | ./test



