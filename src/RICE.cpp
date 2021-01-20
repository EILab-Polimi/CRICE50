#include "RICE.h"
#include<iostream>
#include<fstream>
// constructor
RICE::RICE(){
	
}
// destructor
RICE::~RICE(){
	
}
// constructor with inputs 
RICE::RICE(int hrzn, int carbontype){
	horizon = hrzn;
	// Module selection accoding to settings
	switch (carbontype){
		case 0:
			carbon = new DICECarbon(hrzn);
			climate = new DICEClimate(hrzn);
			break;
		case 1:
			carbon = new WITCHCarbon(hrzn);
			climate = new WITCHClimate(hrzn);
			break;
	}
	econ = new Econ(hrzn);
	t = 0;
}
// simulates one step of the model
void RICE::nextStep(){
	econ->nextStep(climate->tatm);
	carbon->nextStep(econ->e[t]);
	climate->nextStep(carbon->forc[t]);
	t++;
	return;
}
// simulates all the horizon
void RICE::simulate(){
	resetTidx();
	for (int time=0 ; time < horizon; time++){
		// std::cout << "\tSimulation time step " << t << ", year " << 2015+t*5 << std::endl;
		nextStep();
	} 
	return;
}
void RICE::resetTidx(){
	t = 0;
	econ->t = 0;
	for (int ag=0; ag < econ->agents; ag++){
		econ->agents_ptr[ag]->t = 0;
	}
	climate->t = 0;
	carbon->t = 0;
}
// writes simulation trajectories to file
void RICE::writeSimulation(){
	// === Open output file =====
	std::fstream output;
	output.open("./simulationOutput.txt", std::ios_base::out);
	if (!output) {
		std::cout << "Error: file could not be opened" << std::endl;
    	exit(1);
    }
	// === Write header ======
	output.precision(10);
	output << std::scientific;
	output << "YEAR\t";
	econ->writeHeader(output);
	carbon->writeHeader(output);
	climate->writeHeader(output);
	output << std::endl;
	//=== Write time-related output =====
	for (int t=0 ; t < horizon; t++){
		output << 2015+t*5 << "\t" ;
		econ->writeStep(output);
		carbon->writeStep(output);
		climate->writeStep(output);
		output << std::endl;
	} 
	output.close();
	return;	
}
// frees allocated memory
void RICE::RICE_delete(){
	carbon->carbonDelete();
	climate->climateDelete();
	econ->econDelete();
	return;
}