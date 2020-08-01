#include <stdio.h>
//#include <sys/time.h> //mac
#include "lib/nanotime.c"

int generateNext();

int main(){
	//setup and stuff
	double fd = 44100.0;
	double T_ns = (1.0/fd)*1000000000.0;
	int run = 1;
	int counter = 0;
	
	unsigned long timestamp_ns;
	unsigned long time_check_ns = 0;


	while(run){
		nano_second(&timestamp_ns);

		counter++;


		//read keypresses
		//params 1-6
		//buttons 1,2

		//import generator file


		//output to speakers
		
		
		//printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);

		//test/dev
		//printf("%ld", timestamp);

		

		int toWait = 1;
		while(toWait){
			//http://www.catb.org/esr/time-programming/
			
			
			nano_second(&time_check_ns);
			//(1/44100)*1000000000
			//T 44100 = 22675.736961451246 ns

			printf("%f\t", (float)timestamp_ns);
			printf("%f\t", (float)T_ns);
			printf("%f\n", (float)time_check_ns);
				
			if(time_check_ns >= timestamp_ns + T_ns){
				toWait=0;

				//printf("%0.20Ld", (float)((1.0/fd)*1000000000.0));
				//, "T", (long int)(1/44100)/1000000
				run = 0;
			}

		}			
	}
}

int generateNext(){
	return 0;
};
