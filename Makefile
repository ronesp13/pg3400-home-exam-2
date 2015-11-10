CC = gcc
FLAGS = -Wall -std=c11
PARANOIA = -Wall -Werror -Wextra -std=c11
TARGET = program

all: $(TARGET)

$(TARGET): main.o secretCoder.o queue.o
	$(CC) $(FLAGS) queue.c secretCoder.c main.c -o $(TARGET)

main.o: main.c
	$(CC) $(FLAGS) -c -o main.o main.c
secretCoder.o: secretCoder.c
	$(CC) $(FLAGS) -c -o secretCoder.o secretCoder.c
queue.o: queue.c
	$(CC) $(FLAGS) -c -o queue.o queue.c
paranoia:
	$(CC) $(PARANOIA) -o $(TARGET) queue.c secretCoder.c main.c
clean:
	$(RM) $(TARGET) *.o