CFLAGS=-c -Wall -std=c++11 -O2

parser: parser.o
	g++ parser.o -o parser

parser.o: parser.cpp
	g++ parser.cpp $(CFLAGS)

#parser: parser.o#, test.wav
# 	gcc parser.c -o parser $(pkg-config sndfile --cflags --libs)
	#g++ parser.o -o parser $(CFLAGS)

# parser.o: parser.cpp#, test.wav
# 	gcc parser.c -o parser $(pkg-config sndfile --cflags --libs)
	#g++ parser.cpp 
	#$(CFLAGS)

#clean: 
	#rm -f *.o ./parser ./parser
# parser.o: parser.c
# 	gcc parser.c -o parser.o $(pkg-config sndfile --cflags --libs)
# 	gcc parser.c -o parser $(pkg-config sndfile --cflags --libs)
#	gcc parser.c -o parser $(pkg-config sndfile --cflags --libs) && ./parser
