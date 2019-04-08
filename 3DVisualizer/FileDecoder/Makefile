CC=g++
CFLAGS=-c -Wall -O2 -std=c++11
LFLAGS= -lavcodec-ffmpeg -lavformat-ffmpeg -lavutil -lswresample
PROGS= main
OBJS= main.o

# executable targets
all: main

main: main.o
	$(CC) main.o -o main $(LFLAGS)

# object targets
main.o: main.cpp audio_file.h FFT.h
	$(CC) main.cpp -o main.o $(CFLAGS)

clean:
	@rm $(OBJS) $(PROGS)
