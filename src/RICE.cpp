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
RICE::RICE(int nag, int hrzn, int carbontype){
	switch (carbontype){
		case 0:
			carbon = new DICECarbon();
			break;
		case 1:
			carbon = new WITCHCarbon();
			break;
	}
	temp = new WITCHTemp();
	econ = new Econ();
	allocate(nag, hrzn);
	return;
}
// allocates the memory for the different components
void RICE::allocate(int nag, int hrzn){
	agents = nag;
	horizon = hrzn;
	carbon->allocate(hrzn);
	temp->allocate(hrzn);
	econ->allocate(nag, hrzn);
	t = 0;
	return;	
}
// simulates one step of the model
void RICE::nextStep(){
	econ->nextStep();
	carbon->nextStep(econ->e[t]);
	temp->nextStep(carbon->forc[t]);
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
	carbon->carbon_delete();
	temp->temp_delete();
	econ->econ_delete();
	return;
}