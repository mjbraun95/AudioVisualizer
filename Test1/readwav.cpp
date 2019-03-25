#include <stdio.h>
#include <sndfile.h>

int main(){

    SNDFILE *SoundFile;
    SF_INFO SoundFileInfo;  
    double *Samples;
 
    // /media/sf_CMPUT_275/Assignments/CMPUT275_Final_Project/Test1/
    SoundFile=sf_open("test.wav", SFM_READ, &SoundFileInfo);
    //open a file and put it's info into a struct "SoundFileInfo"

   double *Samples[SoundFileInfo.channels * SoundFileInfo.frames];
   //allocate an array to hold the samples
   
    sf_readf_double(SoundFile, Samples, SoundFileInfo.frames);
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