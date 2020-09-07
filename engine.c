#include <iostream>
#include <stdlib.h>

#include "./lib/rtaudio/RtAudio.h"
#include <windows.h>

#include <chrono>
#include <ctime>
#include <time.h>

using namespace std;


int generateNext();
void outputRt(int level, double *buffer);
int saw( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData );

int generator( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData );

int diff(timespec start, timespec end);

void outputVisual(int level);
int generateNoise();


int NUM_FRAMES = 256;
int IS_OUTPUT_SOUND = 1;
int MIN_LEVEL = 0;
int MAX_LEVEL = 1024;

int main(){
	//setup and stuff
	double fd = 44100.0;
	double T_ns = (1.0/fd)*1000000000.0;
	int runFramer = 1;
	int idle = 1;
	unsigned long duration = 0;
	timespec time1, time2;

	int temp = 0;


	//actual synth variables
	int counter = 0;

	int level = 0;

	//init generators
	time_t t;
	srand((unsigned) time(&t));

	cout << " ===== Starting emulation ===== \n";

	if(IS_OUTPUT_SOUND){
		//init RtAudio
		RtAudio dac;
		if ( dac.getDeviceCount() < 1 ) {
			std::cout << "\nNo audio devices found!\n";
			exit( 0 );
		}

		RtAudio::StreamParameters parameters;
		parameters.deviceId = dac.getDefaultOutputDevice();
		parameters.nChannels = 1;
		parameters.firstChannel = 0;
		unsigned int sampleRate = (int)fd;
		unsigned int bufferFrames = NUM_FRAMES; // 256 sample frames
		double data[2];
		
		dac.openStream( &parameters, NULL, RTAUDIO_FLOAT64, sampleRate, &bufferFrames, &generator, (void *)&data );
		dac.startStream();

		std::cout << "\nPlaying ... press <enter> to quit.\n";
		std::cin.get();

		dac.stopStream();
		if(dac.isStreamOpen()) dac.closeStream();

	}else{
		while(runFramer){
			clock_gettime(CLOCK_MONOTONIC, &time1);
			counter++;
			
			//frame stated generation
			outputVisual(level);

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
	}
	cout << "[FG] generated " << counter << " frames of audio\n";
	cout << " =====   Emulation done   =====\n\n\n\n"; 

	
	return 0;
}






//this will be called when audio stream needs new chunk.
//cunk size is 256 bytes.
//the function will generate 256 samples and return to
int generator( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData ){
	double *buffer = (double *) outputBuffer;
	double *lastValues = (double *) userData;
	if ( status ) std::cout << "Stream underflow detected!" << std::endl;

	//synth variables
	int level = 0;



	for (int i=0; i<nBufferFrames; i++ ) {

		*buffer++ = (generateNoise() - MAX_LEVEL/2) / (double)MAX_LEVEL;
		
		//lastValues[0] += 0.005;

		//if ( lastValues[0] >= 1.0 ) lastValues[0] -= 2.0;
	}
	/*
	for (int i=0; i<nBufferFrames; i++ ) { //buffer framer
		

		level = generateNoise();

	}
	outputRt(level, buffer, lastValues);
	*/
	return 0;
}





void outputRt(int level, double *buffer){
	//max NV = 1024  min NV = 0
	//max RT = 1.0  min RT = -1.0	

	*buffer++ = (level - (level/2.0)) / (level/2.0);
}

int generateNoise(){
	return rand() % 1024;
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

void outputVisual(int level){
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


int saw( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData 
	   ){

	unsigned int i, j;

	double *buffer = (double *) outputBuffer;
	double *lastValues = (double *) userData;
	
	if ( status )
		std::cout << "Stream underflow detected!" << std::endl;

	for ( i=0; i<nBufferFrames; i++ ) {

		*buffer++ = lastValues[0];

		lastValues[0] += 0.005;

		if ( lastValues[0] >= 1.0 ) lastValues[0] -= 2.0;
	}
	return 0;
}
