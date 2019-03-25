#include <stdlib.h>
#include <stdio.h>
#include <sndfile.h>

// using namespace std;

int main(){

  // cout << "debug1\n";
  printf("debug1\n");
  SNDFILE *SoundFile;
  printf("debug2\n");
  SF_INFO SoundFileInfo;
  SF_INFO *SoundFilePtr = &SoundFileInfo;  
  printf("debug3\n");
  // double *Samples;
  // SoundFileInfo.frames = 
  // SoundFileInfo.samplerate = 44100;
  // SoundFileInfo.channels = 2;
  // SoundFileInfo.format = 0x010000; //WAV file format
  SoundFileInfo.format = 0;
   // typedef struct
   //    {    sf_count_t  frames ;     /* Used to be called samples. */
   //         int         samplerate ;
   //         int         channels ;
   //         int         format ;
   //         int         sections ;
   //         int         seekable ;
   //     } SF_INFO ;

  // /media/sf_CMPUT_275/Assignments/CMPUT275_Final_Project/Test1/
  SoundFile = sf_open("ass2.wav", SFM_READ, SoundFilePtr);
  printf("debug4\n");
  //open a file and put it's info into a struct "SoundFileInfo"

  printf("channels: ");
  printf(SoundFileInfo.channels);
  printf("\n");
  double *Samples = malloc(SoundFileInfo.channels * SoundFileInfo.frames + 1);
  printf("debug5\n");
  //allocate an array to hold the samples
 

  //  sf_count_t  sf_readf_double  (SNDFILE *sndfile, double *ptr, sf_count_t frames) ;
  sf_count_t readDoub = sf_readf_double(SoundFile, Samples, SoundFileInfo.frames);
  printf("debug6\n");
    //fill the array with sample values, a frame equals on sample per channel 
   
  // ...

  /*take note that the left and right values are interleaved.  So Samples[0] 
is the first sample for the left channel, and Samples[1], is the first sample
 for the right  together they make one frame, this is why you need to allocate number 
of frames times channels.

you can also read sample values as short, int, or float.  If you read as 
short, the range will be -32767 to -32768, which is the range of the 
type short, and in and int, then the values will be represented from the 
range of an int.  using sf_readf_double or float, your values will be between -1, and 1.

libsndfile also can write an array of sample values, to a new wav 
file.  To do this, you first have to create an 
*/

SF_INFO SoundFileInfoOut; 
//then fill in the information manually.

SoundFileInfoOut.channels=SoundFileInfo.channels;
SoundFileInfoOut.frames=SoundFileInfo.frames;
//etc. for all of the members of the struct

/*then you can use the sf write function with the struct containing
 the info as a parameter, the array of sample values, and the path to write it to. 

because the samples are interleaved, you might want to deinterleave
 so that you have two arrays each of length SoundFileInfo.frames.  Use
 can use a loop to put  put Samples[i], for 0 and all even i in one, and all odd i in another, or something to that effect.

before writing to a file, you need to reinterleave the samples.  
...
*/

 }