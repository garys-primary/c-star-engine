#include <iostream>
//#include <sys/time.h> //mac
#include "lib/nanotime.c"
#include "lib/rtaudio-5.1.0/RtAudio.h"
#include <chrono>
#include <ctime>
#include <time.h>

using namespace std;




int generateNext();
int diff(timespec start, timespec end);

int main(){
	//setup and stuff
	double fd = 44100.0;
	double T_ns = (1.0/fd)*1000000000.0;
	int run = 1;
	int idle = 1;
	int counter = 0;
	
	unsigned long duration = 0;

	timespec time1, time2;


	int temp = 0;

	cout << " ===== Starting emulation ===== \n";
	cout << "T_ns: " << T_ns << '\n';

	while(run){
		clock_gettime(CLOCK_MONOTONIC, &time1);

		counter++;
		
		//load
		for (int i = 0; i< 2420; i++)
			temp+=temp;

		//read keypresses
		//params 1-6
		//buttons 1,2

		//import generator file


		//output to speakers
		//Rtaudio
		
		
		//printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);

		//test/dev
		//printf("%ld", timestamp);

		
		

		
		
		

		//(1/44100)*1000000000
		//T 44100 = 22675.736961451246 ns

		idle = 1;
		while(idle){
			//http://www.catb.org/esr/time-programming/
			clock_gettime(CLOCK_MONOTONIC, &time2);
			if(diff(time1,time2) >= T_ns){
				idle = 0;
				cout << diff(time1,time2) << "\n";
			}
		}

		if(counter >=10)
			run = 0;
	}
	cout << " ===== emulation terminated =====";
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
