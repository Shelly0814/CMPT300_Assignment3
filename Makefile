CC = gcc
CCFLAGS = -std=gnu99 -O0 -pthread -lrt -g -Wall -Werror


assn3:
	$(CC) $(CCFLAGS) -o assignment3 assignment3.c


clean:
rm -f core *.o assignment3
