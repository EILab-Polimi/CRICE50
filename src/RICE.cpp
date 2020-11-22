#include "RICE.h"
#include "Carbon.h"
#include "Climate.h"
#include "Econ.h"

#include <iostream>


using namespace std;

RICE::RICE(){
	
}

RICE::RICE(int nag, int hrzn){
	allocate(nag, hrzn);
	return;
}

void RICE::allocate(int nag, int hrzn){
	agents = nag;
	horizon = hrzn;
	carbon.allocate(hrzn);
	climate.allocate(hrzn);
	econ.allocate(nag, hrzn);
	t = 0;
	return;	
}

void RICE::nextStep(){
	carbon.nextStep();
	climate.nextStep();
	econ.nextStep();
	t++;
	return;
}

void RICE::simulate(){
	for (int time=0 ; time < horizon; time++){
		cout << "\tSimulation time step: " << t << endl;
		nextStep();
	} 
	return;
}

void RICE::RICE_delete(){
	carbon.carbon_delete();
	climate.climate_delete();
	econ.econ_delete();
	return;
}