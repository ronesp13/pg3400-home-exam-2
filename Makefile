CC = gcc
FLAGS = -Wall -std=c11
PARANOIA = -Wall -Werror -Wextra -std=c11
TARGET = program

all: $(TARGET)

$(TARGET): main.o secretCoder.o queue.o util.o
	$(CC) $(FLAGS) util.c queue.c secretCoder.c main.c -o $(TARGET)

main.o: main.c
	$(CC) $(FLAGS) -c -o main.o main.c
secretCoder.o: secretCoder.c
	$(CC) $(FLAGS) -c -o secretCoder.o secretCoder.c
queue.o: queue.c
	$(CC) $(FLAGS) -c -o queue.o queue.c
util.o: util.c
	$(CC) $(FLAGS) -c -o util.o util.c
paranoia:
	$(CC) $(PARANOIA) -o $(TARGET) util.c queue.c secretCoder.c main.c
clean:
	$(RM) $(TARGET) *.o