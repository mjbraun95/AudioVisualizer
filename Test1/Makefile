readwav: readwav.c#, test.wav
# 	gcc readwav.c -o readwav $(pkg-config sndfile --cflags --libs)
	gcc readwav.c `pkg-config --cflags sndfile` `pkg-config --libs sndfile` -o readwav -Wall

readwav2: readwav2.c#, test.wav
# 	gcc readwav.c -o readwav $(pkg-config sndfile --cflags --libs)
	gcc readwav2.c `pkg-config --cflags sndfile` `pkg-config --libs sndfile` -o readwav -Wall

clean: 
	rm -f *.o ./readwav
# readwav.o: readwav.c
# 	gcc readwav.c -o readwav.o $(pkg-config sndfile --cflags --libs)
# 	gcc readwav.c -o readwav $(pkg-config sndfile --cflags --libs)
#	gcc readwav.c -o readwav $(pkg-config sndfile --cflags --libs) && ./readwav
