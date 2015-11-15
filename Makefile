CC = gcc
FLAGS = -Wall -std=c11
PARANOIA = -Wall -Werror -Wextra -std=c11
TARGET = westergate

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
lib: secretCoder.o queue.o util.o
	ar rcs libsecret.a util.o queue.o secretCoder.o
libexe:
	$(CC) -L. main.c -o $(TARGET) -lsecret
clean:
	$(RM) $(TARGET) *.o