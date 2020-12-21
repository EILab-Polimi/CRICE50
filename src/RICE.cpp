#include "RICE.h"
#include <iostream>

using namespace std;
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
	climate = new WITCHClimate(hrzn);
	econ = new Econ(hrzn);
	t = 0;
	return;	
}
// // allocates the memory for the different components
// void RICE::allocate(int hrzn){
// 	horizon = hrzn;
// 	carbon->allocate(hrzn);
// 	climate->allocate(hrzn);
// 	econ->allocate(hrzn);
// 	t = 0;
// 	return;	
// }
// simulates one step of the model
void RICE::nextStep(){
	econ->nextStep();
	carbon->nextStep(econ->e[t]);
	climate->nextStep(carbon->forc[t]);
	t++;
	return;
}
// simulates all the horizon
void RICE::simulate(){
	for (int time=0 ; time < horizon; time++){
		cout << "\tSimulation time step " << t << ", year " << 2015+t*5 << endl;
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