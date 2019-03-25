readwav: readwav.c#, test.wav
# 	gcc readwav.c -o readwav $(pkg-config sndfile --cflags --libs)
	gcc readwav.c `pkg-config --cflags sndfile` `pkg-config --libs sndfile` -o readwav -Wall

readwav2: readwav2.c#, test.wav
# 	gcc readwav.c -o readwav $(pkg-config sndfile --cflags --libs)
	gcc readwav2.c `pkg-config --cflags sndfile` `pkg-config --libs sndfile` -o readwav -Wall

tester1: tester1.cpp#, test.wav
# 	gcc readwav.c -o readwav $(pkg-config sndfile --cflags --libs)
	g++ tester1.cpp `pkg-config --cflags sndfile` `pkg-config --libs sndfile` -o tester1 -Wall

clean: 
	rm -f *.o ./readwav ./tester1
# readwav.o: readwav.c
# 	gcc readwav.c -o readwav.o $(pkg-config sndfile --cflags --libs)
# 	gcc readwav.c -o readwav $(pkg-config sndfile --cflags --libs)
#	gcc readwav.c -o readwav $(pkg-config sndfile --cflags --libs) && ./readwav
