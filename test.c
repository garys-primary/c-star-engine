#include <iostream>
#include <stdlib.h>
using namespace std;

int generateNoise();
void isr_rx_vector(int MIDI_char);


//-----------------------------------------------------
//TEST FUNCTION, not to be included in final program
volatile int cab_tick = 0;
volatile int cab_bitNo = 1;
volatile int cab_byte = 0;

int outputCharsAsBits(int bytes[], int size){

// tick 000001111111111122222222333333333344444444
// 01234567890123456789023456789012345678901234567
// |||||||||||||||||||||||||||||||||||||||||||||||
// -',---__-__-``-__-``-``------__-__-``-__-``-``---
// ---- | - output bits - | -- | - output bits - | --

//                    __   __                       __            __
//1023       ||      |  | |  |                     |  |          |  |
// 500 ------'|,-----`  |_|  |_    _    _   ...   _|  |_---------'  |_   ... 
//   0        ||               |__| |__| |                       .  . |_ 
//      |<----st---->|                                           .  . .       
//      |<-st+bitNo*byteNo*(w+spc)->|                            .  . .     
//      |<-st+bitNo*byteNo*(w+spc)+w-->|                         .  . .  
//      |<-st+bitNo*byteNo*(w+spc)+w+spc>|<--[next bit]          .  . .
//      |<-st+bitNo*byteNo*(w+spc)---------------->|             .  . .
//      |<-st+bitNo*byteNo*(w+spc)+w+spc-------------->|<--[next byte].
// c1-> |<----st*cab_byteNo+cab_bitNo*cab_byteNo*(w+spc)-------->|  . .
// c2-> |<----st*cab_byteNo+cab_bitNo*cab_byteNo*(w+spc)+w--------->| .
// c3-> |<----st*cab_byteNo+cab_bitNo*cab_byteNo*(w+spc)+w+spc------->|


    int st = 7;
    int w = 5;
    int spc = 1;

	int level = 2;
	
    int expression = st+st*cab_byte+(cab_byte*8+cab_bitNo)*(w+spc);

    
    cout << "-tick: " << cab_tick << ", bitNo: " << cab_bitNo << ", byte: " << cab_byte << ", ";
    cout << "\tcases: " << expression << " + " << w+spc << "\t";
   
    //output levels accordingly tu time computatuion
    if(cab_byte <= size*8){

        // st phase
        if(cab_tick == 2)
            level = 1023;
        else if(cab_tick == 4)
            level = 0;
        else if(cab_tick < st) 
            level = 500;

        else if(cab_tick < expression)
            level = 450;
        else if(cab_tick < expression+w)
            level = 1023 * ( 1 & (bytes[cab_byte] >> (8-cab_bitNo)) );
        else if(cab_tick < expression+w+spc)
            level = 400;
        else
            level = 600;
    }

    cab_tick++;

    //calculate bit and byte number
    if(cab_tick == expression+w+spc){
        if(cab_bitNo>=8){
            cab_bitNo=1;
            cab_byte++;
        }else
            cab_bitNo++;
    }


    //cout << "-------------I " << cab_pause + (cab_bitWidth + cab_bitSpacing) * 8 * cab_byteNo << ", count: " << cab_outCounter << endl;

    //start sequence
/*
    if(cab_outCounter==2)
        level = 1023;
    else if(cab_outCounter==4)
        level = 0;

	else if(cab_outCounter<=6){  
		level = 500;


    //output bits one by one with pauses
    }else if(cab_outCounter < cab_pause + (cab_bitWidth + cab_bitSpacing) * 8 * cab_byteNo){
        

        cout << "------------II " << (cab_pause + (cab_bitWidth+cab_bitSpacing)*cab_bitCounter + cab_bitWidth*(1+cab_bitCounter))*cab_byteNo << endl;
        //output a bit cab_bitWidth times
        if(cab_outCounter < (cab_pause + (cab_bitWidth+cab_bitSpacing)*cab_bitCounter + 
            cab_bitWidth*(1+cab_bitCounter))*cab_byteNo){
            
            //output 1023 if bit is 1 or 0 for 0
            level = 1023 * (1 & (bytes[cab_byteNo-1] >> (7-cab_bitCounter) ));
        
        }

        //output a bit divider
        if( (cab_outCounter == cab_pause + (cab_bitWidth+cab_bitSpacing) * (1+cab_bitCounter)) * cab_byteNo )  {
            level = 450;
            cab_bitCounter++;
        }

    //otherwice divide chars and check for next byte
	}else if(cab_outCounter < ( cab_pause + (cab_bitWidth + cab_bitSpacing + cab_tail) * 8 * cab_byteNo )){
        cab_bitCounter = 0;
        if(cab_byteNo <= size)
            cab_byteNo++;
		level = 600;
    }else
		cab_outCounter = 0;
	*/

    return level;
}

int main(){

    int bytes[3];

    bytes[0] = 0b01101101;
    bytes[1] = 0b11110000;
    bytes[2] = 0b00110011;

    //reverses order
    cout << "Alg 1: \t"
        << (1 & (bytes[0] >> 7-0))
        << (1 & (bytes[0]  >> 7-1))
        << (1 & (bytes[0]  >> 7-2))
        << (1 & (bytes[0]  >> 7-3))
        << (1 & (bytes[0]  >> 7-4))
        << (1 & (bytes[0]  >> 7-5))
        << (1 & (bytes[0]  >> 7-6))
        << (1 & (bytes[0]  >> 7-7)) << endl;
        


    int level = 0;

    for(int i=0; i<150; i++){

        level = outputCharsAsBits(bytes, 3);

        cout << level << endl;
    }

    return 0;
}

int main3(){
    int result = 0;

    int MIDI_comm1 = 0b10000000;
    int MIDI_data1 = 0b01100101;
    int MIDI_data2 = 0b00001011;
    int MIDI_comm2 = 0b10010000;
    int MIDI_data3 = 0b01101111;
    int MIDI_comm3 = 0b11111111;

    cout << (MIDI_comm1>>4 == 0b0110) << endl;

    isr_rx_vector(MIDI_comm1);
    isr_rx_vector(MIDI_data1);
    isr_rx_vector(MIDI_data2);
    isr_rx_vector(MIDI_comm2);
    isr_rx_vector(MIDI_data3);
    isr_rx_vector(MIDI_comm3);
    isr_rx_vector(MIDI_data1);
    isr_rx_vector(MIDI_comm3);
    isr_rx_vector(MIDI_data3);
    isr_rx_vector(MIDI_data1);

    return 0;
}

volatile int MIDI_message[3] = {0,0,0};
volatile int MIDI_chars_received = 0;

volatile int pitch;

void isr_rx_vector(int UDR){
    
    if(UDR >= 0b10000000) //is command   
        MIDI_chars_received = 0;

    MIDI_message[MIDI_chars_received++] = UDR;




    cout << "Midi message: [ " << MIDI_message[0] << " | " 
         << MIDI_message[1] << " | " 
         << MIDI_message[2] << " ]" << endl;

    if(MIDI_chars_received==2){ 
        if(MIDI_message[0]>>4 == 0b1001){ //note on
            pitch = MIDI_message[1];
        }
    }

    cout << "note:" << pitch << endl;
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