#include <iostream>
#include <stdlib.h>

#include "./lib/rtaudio/RtAudio.h"
#include <windows.h>

#include <chrono>
#include <ctime>
#include <time.h>

#include <inttypes.h>
#include <math.h>


//read keys
#include <conio.h>  //provides non standard getch() function


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
double generateNoiseNormalized();
void processKey(char key);


int NUM_FRAMES = 256;
int MIN_LEVEL = 0;
int MAX_LEVEL = 1024;

int IS_OUTPUT_SOUND = 1; //can be console log of vaweform
int OUTPUT_TO_SPEAKER = 1;
int OUTPUT_TO_SPECTRUM_ANALYZER = 1; //for this you need: 
//a vitrual cable https://vb-audio.com/Cable/index.htm and a 
//spectrum analyzer http://www.sillanumsoft.org/

//control keys
int KEY_Q = 0;


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

	if(IS_OUTPUT_SOUND){
		//init RtAudio
		RtAudio aud1;
		RtAudio aud2;

		//list devices
		unsigned int devices = aud1.getDeviceCount();
		if ( devices < 1 ) {
			std::cout << "\nNo audio devices found!\n";
			exit( 0 );
		}
		RtAudio::DeviceInfo info;
		for ( unsigned int i=0; i<devices; i++ ) {
			info = aud1.getDeviceInfo( i );
			if ( info.probed == true ) {
				// Print, for example, the maximum number of output channels for each device
				std::cout << "device = " << i;
				std::cout << ": maximum output channels = " << info.outputChannels << "\n";
			}
		}

		if(OUTPUT_TO_SPEAKER){
			RtAudio::StreamParameters parameters;
			parameters.deviceId = aud1.getDefaultOutputDevice();
			parameters.nChannels = 1;
			parameters.firstChannel = 0;
			unsigned int sampleRate = (int)fd;
			unsigned int bufferFrames = NUM_FRAMES; // 256 sample frames
			double data[2];

			aud1.openStream( &parameters, NULL, RTAUDIO_FLOAT64, sampleRate, &bufferFrames, &generator, (void *)&data );
			aud1.startStream();
		}
		if(OUTPUT_TO_SPECTRUM_ANALYZER){
			
			RtAudio::StreamParameters parameters2;
			parameters2.deviceId = 2;
			parameters2.nChannels = 1;
			parameters2.firstChannel = 0;
			unsigned int sampleRate = (int)fd;
			unsigned int bufferFrames = NUM_FRAMES; // 256 sample frames
			double data[2];

			aud2.openStream( &parameters2, NULL, RTAUDIO_FLOAT64, sampleRate, &bufferFrames, &generator, (void *)&data );
			aud2.startStream();
		}
		


		cout << "\nPlaying ... press <enter> to quit.\n";


		// Keypress detection for live twiggle

		int keepReading = 1;

		while(keepReading){
			char c = getch();

			if(c == 13 || c == '\n'){
				if(OUTPUT_TO_SPEAKER){
					aud1.stopStream();
					if(aud1.isStreamOpen()) aud1.closeStream();
				}

				if(OUTPUT_TO_SPECTRUM_ANALYZER){
					aud2.stopStream();
					if(aud2.isStreamOpen()) aud2.closeStream();
				}	

				keepReading = 0;	
			}else if(c=='q'){
				cout << "Q: " << KEY_Q << endl;
				KEY_Q = 1 - KEY_Q;
			}else{
				processKey(c);
			}
		}


	}else{
		//UNDEVELOPED. use int generator() for output

		int counter=0;
		int level=0;

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

		cout << "[FG] generated " << counter << " frames of audio\n";
	}
	
	cout << " =====   Emulation done   =====\n\n\n\n"; 
	
	return 0;
}







//this will be called when audio stream needs new chunk.
//chunk size is 256 bytes.
//the function will generate 256 samples and return to

/////////////////////////////////////////////////////////////////////
// optimization: 
/////////////////////////////////////////////////////////////////////
// save cycles on array.push() by overflowing itterator:
// 			https://www.avrfreaks.net/forum/circular-buffer
// doubles to uint32_t (much much more precision)
// 			https://www.avrfreaks.net/forum/floating-point-and-avr?name=PNphpBB2&file=viewtopic&t=113173
/////////////////////////////////////////////////////////////////////




//synth select
// 1: NoiseVoice noise processor
int SYNTH = 1;





/////////////////////////////////////////////////////////////////////
// Common components
/////////////////////////////////////////////////////////////////////
// filter explorer http://jaggedplanet.com/iir/iir-explorer.asp



/////////////////////////////////////////////////////////////////////
// SYNTH 1 variables
/////////////////////////////////////////////////////////////////////
typedef double REAL;


REAL level = 0;
REAL inputValues[3] = {0,0,0};
REAL outputValues[3] = {0,0,0};

//filter  a(1) = -1.287 and a(2)= .8282   | Digital_Resonators.pdf

typedef double REAL;
#define NBQ 1
REAL biquada[]={0.9608788261715387,-0.6823338291115265};
REAL biquadb[]={-1,0};
REAL gain=50.29611161304246;
REAL xyv[]={0,0,0,0,0,0};

int cutoff = 0;
int bandwidth = 0;

REAL a;
REAL th;
REAL poleR;
REAL poleI;

REAL w0;
REAL w1;
REAL w2;
REAL wd;

REAL pwdI;
REAL pwdR;

REAL dR;
REAL dI;

REAL warp(REAL cutoff){
	2 * tan(PI * cutoff / 44100);
}

/////////////////////////////////////////////////////////////////////

int generator( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData ){
	double *RTbuffer = (double *) outputBuffer;
	double *lastValues = (double *) userData;


	//if ( status ) std::cout << "Stream underflow detected!" << std::endl;


	for (int frame=0; frame < nBufferFrames; frame++ ) {

		//*RTbuffer++ = ((level - MAX_LEVEL/2) / (double) MAX_LEVEL); //output(level)
		*RTbuffer++ = level;

		/////////////////////////////////////////////////////////////////////
		////////        SYNTH: NoiseVoice noise manipulator          ////////
		/////////////////////////////////////////////////////////////////////
		
		//level = generateNoise();
		level = generateNoiseNormalized();


		//modify filter props

		//"warp" fx
		//; 2*Math.tan(PI*cutoff/44100)

		a = warp(cutoff)
		//a = 3.234924533245629;
		
		th = 3.141592653589793;
		poleR = cos(th)*a;
		poleI = sin(th)*a;
		
		if(poleI<1E-10 && poleI>-1e-10) 
			poleI=0;

		//"Make bandpass"
		if (bandwidth>cutoff) bandwidth=cutoff;
		if (bandwidth>srate/2-cutoff) bandwidth=srate/2-cutoff;

		w2 = warp(cutoff+bw/2);
		w1 = warp(cutoff-bw/2);
		w0 = sqrt(w2*w1);           //!
		wd = w2-w1;
		
		pwdR = poleR * (wd/w0); //WAS NOT COMPLEX for the equation
		pwdI = poleI * (wd/w0); //(c) => new Complex(this.r*c, this.i*c);
		
		//d=pwd.multiply(pwd).minus(new Complex(4*w0*w0))
		//multiply: (c) => Complex(this.r*c.r - this.i*c.i, this.r*c.i + this.i*c.r)
		//minus: (c) => Complex(this.r-c.r, this.i-c.i)

		dI = 0;	
		dR = -39.56805950939898;





		//biquada[0]=

		int b, xp=0, yp=3, bqp=0;
		REAL out = level / gain;

		for (int i=5; i>0; i--) 
			xyv[i] = xyv[i-1];

		int len=2;
		xyv[xp]=out;
		for(int i=0; i<len; i++) 
			out += xyv[xp+len-i] * biquadb[bqp+i] - xyv[yp+len-i] * biquada[bqp+i];
		bqp+=len;
		xyv[yp]=out;

		if(KEY_Q)
			level = out;





		//insert sample  [1][2][3] => [1][1][2] => [4][1][2]
		/*for(int i=2; i>0; i--){
            inputValues[i]=inputValues[i-1];
        }
        inputValues[0]=level;

		//Y(k) = X(k) - a(1)Y(k-1) - a(2)Y(k-2)   | Digital_Resonators.pdf
		if(KEY_Q)
			level = level - a1*outputValues[1] - a2*outputValues[2]; 
		
		//track output levels
		for(int i=2; i>0; i--){
            outputValues[i]=outputValues[i-1];
        }
        outputValues[0]=level;
		*/
		//cout << level << endl;

		/////////////////////////////////////////////////////
	
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void processKey(char key){
	if(key=='t'){
		cut += 10;
		cout << "Cutoff: " << cut << endl;
	}else if(key=='g'){
		cut -= 10;
		cout << "Cutoff: " << cut << endl;
	}else if(key=='y'){
		reso += 10;
		cout << "Reso: " << reso << endl;
	}else if(key=='h'){
		reso -= 10;
		cout << "Reso: " << reso << endl;
	}
}

int generateNoise(){
	return rand() % MAX_LEVEL;
}
double generateNoiseNormalized(){

	return ((rand() % 1024) - 512 )/ 1024.0;
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
