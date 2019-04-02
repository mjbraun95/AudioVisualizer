CC=g++
SDL_LIB = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I/usr/loacl/include
CFLAGS=-c -Wall -O2 -std=c++11 $(SDL_INCLUDE)
LFLAGS= $(SDL_LIB) -lavcodec-ffmpeg -lavformat-ffmpeg -lavutil -lswresample
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
