CC=g++
CFLAGS=-c -Wall -O2 -std=c++11
LFLAGS= -lavcodec-ffmpeg -lavformat-ffmpeg -lavutil -lswresample
PROGS= test
OBJS= test.o

# executable targets
all: test

test: test.o
	$(CC) test.o -o test $(LFLAGS)

# object targets
test.o: test.cpp audio_file.h FFT.h
	$(CC) test.cpp -o test.o $(CFLAGS)

clean:
	@rm $(OBJS) $(PROGS)
