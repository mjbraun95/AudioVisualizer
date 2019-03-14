#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "FFT.h"

using namespace std ;
using namespace sf ;

int main()
{
	RenderWindow window(VideoMode(900,900,32),"Window");

	string path ;
	int bufferSize ;
	cout<<"Put an audio file in the Ressources folder (.wav will work, mp3 wont)"<<endl;
	cout<<"Enter the file name (example.wav) : " ;
	cin>>path ;
	cout<<"Enter the buffer size treated by the fft (powers of two works best like 16384)"<<endl;
	cin>>bufferSize ;

	FFT fft("Ressources/"+path,bufferSize) ;

	Event event ;

	while(window.isOpen())
	{
		while(window.pollEvent(event)) {}

		fft.update() ;

		window.clear() ;
		fft.draw(window) ;
		window.display() ;
	}

	return 0;
}