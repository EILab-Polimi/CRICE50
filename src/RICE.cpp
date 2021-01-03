#include "RICE.h"
#include <iostream>

// constructor
RICE::RICE(){
	
}
// destructor
RICE::~RICE(){
	
}
// constructor with inputs 
RICE::RICE(int hrzn, int carbontype){
	horizon = hrzn;
	switch (carbontype){
		case 0:
			carbon = new DICECarbon(hrzn);
			break;
		case 1:
			carbon = new WITCHCarbon(hrzn);
			break;
	}
	climate = new DICEClimate(hrzn);
	econ = new Econ(hrzn);
	t = 0;
}
// simulates one step of the model
void RICE::nextStep(){
	econ->nextStep(climate->tatm[t]);
	carbon->nextStep(econ->e[t]);
	climate->nextStep(carbon->forc[t]);
	t++;
	return;
}
// simulates all the horizon
void RICE::simulate(){
	for (int time=0 ; time < horizon; time++){
		std::cout << "\tSimulation time step " << t << ", year " << 2015+t*5 << std::endl;
		nextStep();
	} 
	return;
}
// frees allocated memory
void RICE::RICE_delete(){
	carbon->carbonDelete();
	climate->climateDelete();
	econ->econDelete();
	return;
}