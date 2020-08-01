#include <stdio.h>
#include <sys/time.h> //mac

int generateNext();

int main(){
	//setup and stuff
	struct timeval tval_current, tval_test, tval_result;


	int run = 1;
	int counter = 0;
	
	long timestamp = 0;



	while(run){
		counter++;
		gettimeofday(&tval_current, NULL);
	
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
			

			if((long int)tval_result.tv_usec >= (1/44100)/1000000){
				toWait=0;
				printf("%0.20f", (float)tval_result.tv_usec);
				printf("%0.20f", (float)(1.0/44100.0));
				//, "T", (long int)(1/44100)/1000000
				run = 0;
			}

		}			
	}
}

int generateNext(){
	return 0;
};
