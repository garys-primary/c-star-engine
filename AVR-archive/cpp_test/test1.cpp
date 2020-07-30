#include <stdio.h>
#include <iostream>
#include <string>

volatile int sat1 = 200;
volatile int sat2 = 100;
volatile int lig1 = 150;
volatile int lig2 = 250;

//LOGIC vvv
double hue2rgb(double p, double q, double t){
	std::cout<<"p:"<<p<<" q:"<<q<<" t:"<<t<<'\n';
	
	double result = p;
	
	if(t<0) t+=1.0;
	if(t>1) t-=1.0;
		
	if(t<1/6){
		result = 1.0*p+(q-p)*6*t;
		std::cout<<"t<1/6\n";
	} else if(t<1/2){ 
		result = 1.0*q;
		std::cout<<"t<1/2\n";
	} else if(t<2/3){ 
		result = 1.0*(p+1.0*(q-p)*(2/3-t)*6.0);
		std::cout<<"t<2/3\n";
	}
	
	return result;
}
//LOGIC ^^^

void setRGB(double hue, double valueMod){
	//int sat,lig;
	double r1, g1, b1;
	
	int bank = 1;
	int mode = 1;
	
//LOGIC vvv
	
	/*if(bank){
		if(mode){
			sat = valueMod;
			lig = lig2;
		}else{
			sat = sat2;
			lig = valueMod;
		}
	} else {
		if(mode){
			sat = valueMod;
			lig = lig1;
		}else{
			sat = sat1;
			lig = valueMod;
		}
	}*/
	//logic replacement
	int lig = valueMod;
	int sat = 128;
	
	
	double ligD = lig/256.0;
	double satD = sat/256.0;
	double hueD = hue/256.0;
	
	
	std::cout<<"-- input hue:"<<hue<<'\t'<<"double hue:"<<hueD<<'\n';
	
	double q = 1.0*ligD<0.5 ? 1.0*ligD*(1+satD) : 1.0*ligD+satD-1.0*ligD*satD;
	double p = 2.0*ligD-q;

	r1 = hue2rgb(p, q, hueD + 1/3);
	g1 = hue2rgb(p, q, hueD);
	b1 = hue2rgb(p, q, hueD - 1/3);

//LOGIC ^^^


	std::string term = "\033[0m";
	std::string rcol = r1 > 255*255 ? "\033[30m" : "";
	std::string gcol = g1 > 255*255 ? "\033[30m" : "";
	std::string bcol = b1 > 255 ? "\033[30m" : "";
	
	std::cout<<rcol<<r1<<term<<'\t'<<gcol<<g1<<term<<'\t'
		<<bcol<<b1<<term
		<<'\t'<<'\t'<<hueD<<'\t'<<'\t'<<satD<<'\t'<<ligD<<"\n";
}

int main()
{
	//int hue = 0;
	int value = 128;
	
	//std::cout << "Max values:\n";
	//std::cout << 256*256-1 << '\t' << 256*256-1 << '\t' << 256-1 << '\n';
	
	/*while(1){
  	setRGB(hue, value);
  	std::cin >> hue;
  	std::cout << "val: ";
  	std::cin >> value;
  }*/
  
  //for(int val=0; val<256; val+=8){
  	for(int h=0; h<=256; h+=4){
  		setRGB(h, value);
  	}
  //}
  
  return 0;
}