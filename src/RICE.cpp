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
	while (sJunk!="robustness"){
		in >> sJunk;
	}
	in >> robustness;
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
	createLinks();

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
// creates links between objects
void RICE::createLinks(){
	econ->fromCarbon = carbon->toEcon;
	econ->fromClimate = climate->toEcon;
	carbon->fromEcon = econ->toCarbon;
	carbon->fromClimate = climate->toCarbon;
	climate->fromCarbon = carbon->toClimate;
	climate->fromEcon = econ->toClimate;
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
		nvars = (horizon - 1) * econ->agents * 2;		
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
	econ->nextStep();
	carbon->nextStep();
	climate->nextStep();
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
// resets indices for new simulation
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
// sets ssp
void RICE::setSsp(int ssp){
	for (int ag=0; ag < econ->agents; ag++){
		econ->agents_ptr[ag]->setSsp(ssp);
	}
	return;
}
// sets damages
void RICE::setDamages(int damages){
	for (int ag=0; ag < econ->agents; ag++){
		econ->agents_ptr[ag]->setDamages(damages);
	}
	return;
}
// sets adaptation efficiency
void RICE::setAdaptEff(double adapteff){
	for (int ag=0; ag < econ->agents; ag++){
		econ->agents_ptr[ag]->setAdaptEff(adapteff);
	}
	return;
}
// report objectives
void RICE::reportObjs(std::string nameSol, int ssp, int damages, std::fstream& robustnessOutput){
	double tatmpeak = 0.0;
	for (int tidx=0; tidx <= 17; tidx++){
		tatmpeak = std::max(climate->tatm[tidx], tatmpeak);
	}
	double gini = 0.0;
	gini = econ->computeGini();
	double* prctiles;
	prctiles = econ->computePrctiles(); 
	robustnessOutput << nameSol << "\t" << ssp  << "\t" << damages << "\t" << 
		econ->utility << "\t" << climate->tatm[17] << "\t" <<
		tatmpeak << "\t" << gini << "\t" <<
		*(prctiles) << "\t" <<  *(prctiles+1) << "\t" << 
		*(prctiles+2) << "\t" <<  *(prctiles+3) << std::endl;
	return;
}
// simulates over uncertainties
void RICE::simulateUnc(double* objs){
	// objs[0] =  - pow(10,10);
	// objs[1] = 0.0;
	// objs[2] = 0.0;
	// double* percentiles;
	std::vector<double> welfare;
	std::vector<double> y15c;
	std::vector<double> ineq;
	std::vector<double> net;
	for (int ssp = 1; ssp <= 5; ssp++){
		// set ssp
		setSsp(ssp);
		for (int damages = BURKESR; damages < DAMAGEERR; damages++){
			// set damages
			setDamages(damages);
			for (int adapteff = 0; adapteff <= 5; adapteff++){
				double y15C = 0.0;
				setAdaptEff(adapteff*0.2);
				simulate();
				welfare.push_back(-econ->utility);
				// objs[0] = std::max(- econ->utility, objs[0]);
				for (int tidx = 0; tidx < horizon; tidx++){
					if (climate->tatm[tidx] > 1.5){
						y15C += 5.0;
					}
				}
				// objs[1] = std::max(y15C, objs[1]);
				y15c.push_back(y15C);
				// percentiles = econ->computePrctiles7525();
				// ineq.push_back(percentiles[0] / percentiles[3]);
				// objs[2] = std::max(econ->computeGini(), objs[2]);
				ineq.push_back(econ->computePrctiles7525());
				net.push_back(econ->computeNET());
				std::cout << ssp << "\t" << damages << "\t" << adapteff << "\t"
					<< - econ->utility << "\t" << y15C << "\t" << econ->computeGini() << std::endl;
			}
		}
	}
	objs[0] = *std::max_element(welfare.begin(), welfare.end());
	std::nth_element(welfare.begin(), welfare.begin() + welfare.size() / 2, welfare.end());
	objs[1] = welfare[welfare.size() / 2];
	objs[2] = *std::max_element(y15c.begin(), y15c.end());
	std::nth_element(y15c.begin(), y15c.begin() + y15c.size() / 2, y15c.end());
	objs[3] = y15c[y15c.size() / 2];
	objs[4] = *std::max_element(ineq.begin(), ineq.end());
	std::nth_element(ineq.begin(), ineq.begin() + ineq.size() / 2, ineq.end());
	objs[5] = ineq[ineq.size() / 2];
	objs[6] = *std::max_element(net.begin(), net.end());
	std::nth_element(net.begin(), net.begin() + net.size() / 2, net.end());
	objs[7] = net[net.size() / 2];
	return;
}
// frees allocated memory
void RICE::RICE_delete(){
	carbon->carbonDelete();
	climate->climateDelete();
	econ->econDelete();
	return;
}