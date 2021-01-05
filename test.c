#include <iostream>
#include <stdlib.h>
using namespace std;
int generateNoise();

int main(){
    int result = 0;

    char MIDI_char1 = 0b01100101;
    char MIDI_char2 = 0b00001011;

    cout << (MIDI_char1>>4 == 0b0110) << endl;

    cout << MIDI_char1 << endl;
    MIDI_char2 = '\0';
    cout << MIDI_char1 << endl;

    return 0;
}

int main2(){
    float a1 = -1.287;
    float a2 = 0.8282;
    int inputBuffer[3] = {0,0,0};
    int outputBuffer[3] = {0,0,0};
    int level = 0;

    for(int i=0; i<10; i++){
        level = generateNoise();
        
        for(int i=2; i>0; i--){
            outputBuffer[i]=outputBuffer[i-1];
        }
        outputBuffer[0]=level;

        //level = level - a1*levels[1] - a2*levels[2]; 

        cout << level << "\t|\t" << outputBuffer[0] << '\t' << outputBuffer[1] << '\t' << outputBuffer[2] << '\n';
    }
    return 0;
}


int generateNoise(){
	return rand() % 1024;
}