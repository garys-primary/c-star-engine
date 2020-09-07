#include <iostream>
#include <stdlib.h>

#include "./RtAudio.h"
#include <windows.h>

#include <chrono>
#include <ctime>
#include <time.h>

using namespace std;




int generateNext();
int saw( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData );
int diff(timespec start, timespec end);

void output(int level);
int generateNoise();


int NUM_FRAMES = 100;

int main(){
	//setup and stuff
	double fd = 44100.0;
	double T_ns = (1.0/fd)*1000000000.0;
	int runFramer = 1;
	int idle = 1;
	unsigned long duration = 0;
	timespec time1, time2;

	int temp = 0;

	//init generators
	time_t t;
	srand((unsigned) time(&t));

	cout << " ===== Starting emulation ===== \n";

	RtAudio audio;
	// Determine the number of devices available
	unsigned int devices = audio.getDeviceCount();
	// Scan through devices for various capabilities
	RtAudio::DeviceInfo info;
	for ( unsigned int i=0; i<devices; i++ ) {
		info = audio.getDeviceInfo( i );
		if ( info.probed == true ) {
		// Print, for example, the maximum number of output channels for each device
		std::cout << "device = " << i;
		std::cout << ": maximum output channels = " << info.outputChannels << "\n";
		}
	}
	return 0;

	//actual synth variables
	int counter = 0;

	int level = 0;




	while(runFramer){
		clock_gettime(CLOCK_MONOTONIC, &time1);
		counter++;
		
		//frame stated generation
		output(level);
		

		level = generateNoise();
		

		cout << "\n";

		//frame generated
		idle = 1;
		while(idle){
			//http://www.catb.org/esr/time-programming/
			clock_gettime(CLOCK_MONOTONIC, &time2);
			if(diff(time1,time2) >= T_ns){
				idle = 0;
				//cout << diff(time1,time2) << "\n";
			}
		}

		if(counter >= NUM_FRAMES)
			runFramer = 0;
	}
	cout << "[FG] generated " << counter << " frames of audio\n";
	cout << " =====   Emulation done   =====";
}

//1596657151
//1596657028263054000
int diff(timespec start, timespec end){
    timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp.tv_sec*1000000000 + temp.tv_nsec ;
}


int generateNext(){	
	return 0;
};

void output(int level){
	//1024 is max, 1024/8 = 128, will allow overamp till 200

	int HAT = 100;
	int RESOLUTION_DIVIDER = 16;

	int maxVolume = 1024/RESOLUTION_DIVIDER;
	int displayLevel = level/RESOLUTION_DIVIDER;

	string bar = "[";

	if(displayLevel > 200)
		cout << "AMP TOO HIGH! " << level;
	else{
		for(int i=0; i<(maxVolume+HAT); i++){
			if(i<displayLevel || (i>displayLevel && i<maxVolume))
				bar+=' ';
			
			if(i==displayLevel)
				bar+='*';

			if(i==maxVolume)
				bar+="|";	
		}

		bar+=']';
	}

	cout << bar << '\t';

}

int generateNoise(){
	return rand() % 1024;
}

int saw( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData )
{
  unsigned int i, j;
  double *buffer = (double *) outputBuffer;
  double *lastValues = (double *) userData;
  if ( status )
    std::cout << "Stream underflow detected!" << std::endl;
  // Write interleaved audio data.
  for ( i=0; i<nBufferFrames; i++ ) {
    for ( j=0; j<2; j++ ) {
      *buffer++ = lastValues[j];
      lastValues[j] += 0.005 * (j+1+(j*0.1));
      if ( lastValues[j] >= 1.0 ) lastValues[j] -= 2.0;
    }
  }
  return 0;
}