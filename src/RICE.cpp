#include "RICE.h"
#include <iostream>
#include <fstream>
#include <string>

// convert input string to enum
ModelType stringToModelType(std::string input){
	if (input == "DICE") return DICE;
	if (input == "WITCH") return WITCH;	
	if (input == "FAIR") return FAIR;
	if (input == "GEOFFROY") return GEOFFROY;
	return ERR;
}
// constructor  
RICE::RICE(){
	std::fstream in;
	std::string line;
	std::string sJunk = "";
	in.open("./settings/generalSettings.txt");
	if (!in){
		std::cerr << 
			"The RICE general file could not be found!" 
			<< std::endl;
	    exit(1);
	}
	while (sJunk!="horizon"){
		in >>sJunk;
	}
	in >> horizon;
	while (sJunk!="carbon_model"){
		in >>sJunk;
	}
	in >> line;
	carbon_model = stringToModelType(line);
	while (sJunk!="climate_model"){
		in >>sJunk;
	}
	in >> line;
	climate_model = stringToModelType(line);
	while (sJunk!="objectives"){
		in >>sJunk;
	}
	in >> objs;
	switch(carbon_model){
		case WITCH:
			carbon = new WITCHCarbon(horizon);
			break;
		case DICE:
			carbon = new DICECarbon(horizon);
			break;
		case FAIR:
			carbon = new FAIRCarbon(horizon);
			break;
		case GEOFFROY:
			std::cerr <<
				"insert an available ModelType for Carbon" 
				<< std::endl;
		case ERR:
			std::cerr << 
				"insert an available ModelType for Carbon" 
				<< std::endl;
	}
	switch(climate_model){
		case WITCH:
			climate = new WITCHClimate(horizon);
			break;
		case DICE:
			climate = new DICEClimate(horizon);
			break;
		case GEOFFROY:
			climate = new GeoffroyClimate(horizon);
			break;
		case FAIR:
			std::cerr << 
				"insert an available ModelType for Climate" 
				<< std::endl;			
		case ERR:
			std::cerr << 
				"insert an available ModelType for Climate" 
				<< std::endl;
	}
	econ = new Econ(horizon);
	if (econ->params.DMType == INPUT_POLICY){
		econ->initializeStates(econ->getNStates() +
			climate->getNStates() + carbon->getNStates());
	}
	t = 0;
}
// destructor
RICE::~RICE(){
	
}
// sets decision variables provided within MOEA framework
void RICE::setVariables(double* vars){
	econ->setEconVariables(vars);
	return;
}
//returns number of objectives
int RICE::getNObjs(){
	return objs;
} 
//returns number of variables
int RICE::getNVars(){
	int nvars;
	if (econ->params.DMType == INPUT_POLICY){
		nvars = econ->getNVars();
	}
	else{
		nvars = horizon*econ->agents * 2;		
	}
	return nvars;
} 
// get global States to be used in control policies as inputs
void RICE::updateGlobalStates(){
	double* econStates = econ->getStates();
	int nEconStates = econ->getNStates();
	double* climateStates = climate->getStates();
	int nClimateStates = climate->getNStates();
	double* carbonStates = carbon->getStates();
	int nCarbonStates = carbon->getNStates();
	for (int n=0; n < nEconStates; n++){
		econ->globalStates[n] = econStates[n];
	}
	int nstart = nEconStates;
	for (int n=nstart; n < nstart + nClimateStates; n++) {
		econ->globalStates[n] = climateStates[n - nstart];
	}
	nstart = nEconStates + nClimateStates;
	for (int n=nstart ; n < nstart + nCarbonStates; n++) {
		econ->globalStates[n] = carbonStates[n - nstart];
	}
	return;
}
// simulates one step of the model
void RICE::nextStep(){
	if (econ->params.DMType == INPUT_POLICY){
		updateGlobalStates();
	}
	econ->nextStep(climate->tatm);
	// if (carbon_model == FAIR){
	// 	carbon->nextStepC(econ->e[t], climate->toCarbon());
	// }
	// else{
	// 	carbon->nextStep(econ->e[t]);		
	// }
	carbon->nextStep(econ->e[t], climate->toCarbon());
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
	// std::cout << "Objective value - utility is equal to: " 
	// 	<< econ->utility << std::endl;;
	return;
}
void RICE::resetTidx(){
	t = 0;
	econ->t = 0;
	switch (econ->params.utilityType){
		case COOP:
			econ->utility = pow(10,-8);	
			break;
		case NON_COOP:
			econ->utility = 0.0;
			break;
		case UTILITYERR:
			std::cerr << 
				"Please insert a valid option for the utility setting" 
				<< std::endl;
			exit(1);
	}
	for (int ag=0; ag < econ->agents; ag++){
		econ->agents_ptr[ag]->t = 0;
		econ->agents_ptr[ag]->utility = 0.0;
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
		std::cerr << "Error: file could not be opened" << std::endl;
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