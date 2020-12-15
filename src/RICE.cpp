#include "RICE.h"
#include <iostream>

using namespace std;
// constructor
RICE::RICE(){
	
}
// constructor with inputs 
RICE::RICE(int nag, int hrzn){
	allocate(nag, hrzn);
	return;
}
// allocates the memory for the different components
void RICE::allocate(int nag, int hrzn){
	agents = nag;
	horizon = hrzn;
	carbon.allocate(hrzn);
	temp.allocate(hrzn);
	econ.allocate(nag, hrzn);
	t = 0;
	return;	
}
// simulates one step of the model
void RICE::nextStep(){
	carbon.nextStep();
	temp.nextStep(carbon.forc[t]);
	econ.nextStep();
	t++;
	return;
}
// simulates all the horizon
void RICE::simulate(){
	for (int time=0 ; time < horizon; time++){
		cout << "\tSimulation time step: " << t << endl;
		nextStep();
	} 
	return;
}
// frees allocated memory
void RICE::RICE_delete(){
	carbon.carbon_delete();
	temp.temp_delete();
	econ.econ_delete();
	return;
}