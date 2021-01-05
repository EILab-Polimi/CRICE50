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
	for (int time=0 ; time < horizon; time++){
		std::cout << "\tSimulation time step " << t << ", year " << 2015+t*5 << std::endl;
		nextStep();
	} 
	return;
}
// writes simulation trajectories to file
void RICE::writeSimulation(){
	std::fstream output;
	output.open("./simulationOutput.txt", std::ios_base::out);
	if (!output) {
		std::cout << "Error: file could not be opened" << std::endl;
    	exit(1);
    }
	output << "YEAR\t";
	econ->writeHeader(output);
	carbon->writeHeader(output);
	climate->writeHeader(output);
	output << std::endl;
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