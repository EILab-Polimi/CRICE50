#include "RICE.h"
#include <numeric>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

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
	in >> nobjs;
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
	return nobjs;
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
	std::vector<double> allobjs;
	std::vector<double> welfare;
	std::vector<double> y15c;
	std::vector<double> ineq;
	std::vector<double> net;
	std::fstream outputScenario;
	outputScenario.open("./outputScenario.txt", std::ios_base::out);
	for (int ssp = 1; ssp <= 5; ssp++){
		// set ssp
		setSsp(ssp);
		for (int damages = BURKESR; damages < DAMAGEERR; damages++){
			// set damages
			setDamages(damages);

			double y15C = 0.0;
			simulate();
			welfare.push_back(-econ->utility);
			for (int tidx = 0; tidx < horizon; tidx++){
				if (climate->tatm[tidx] > 1.5){
					y15C += 5.0;
				}
			}
			y15c.push_back(y15C);
			ineq.push_back(econ->computePrctiles7525());
			net.push_back(econ->computeNET());

			// for (int adapteff = 0; adapteff <= 2; adapteff++){
			// 	double y15C = 0.0;
			// 	setAdaptEff(adapteff*0.5);
			// 	simulate();
			// 	welfare.push_back(-econ->utility);
			// 	for (int tidx = 0; tidx < horizon; tidx++){
			// 		if (climate->tatm[tidx] > 1.5){
			// 			y15C += 5.0;
			// 		}
			// 	}
			// 	y15c.push_back(y15C);
			// 	ineq.push_back(econ->computePrctiles7525());
			// 	net.push_back(econ->computeNET());
				outputScenario << ssp << "\t" << damages << "\t"
					<< - econ->utility << "\t" << y15C << "\t" << econ->computePrctiles7525() << std::endl;
			// }
		}
	}
	double sum = std::accumulate(std::begin(welfare), std::end(welfare), 0.0);
	allobjs.push_back(sum / welfare.size());
	allobjs.push_back(*std::max_element(welfare.begin(), welfare.end()));
	sum = std::accumulate(std::begin(y15c), std::end(y15c), 0.0);
	allobjs.push_back(sum / y15c.size());
	allobjs.push_back(*std::max_element(y15c.begin(), y15c.end()));
	sum = std::accumulate(std::begin(ineq), std::end(ineq), 0.0);
	allobjs.push_back(sum / ineq.size());
	allobjs.push_back(*std::max_element(ineq.begin(), ineq.end()));
	for (int obj = 0; obj < nobjs; obj++){
		objs[obj] = allobjs[obj];
	}
	// objs[6] = *std::max_element(net.begin(), net.end());
	// sum = std::accumulate(std::begin(net), std::end(net), 0.0);
	// objs[7] =  sum / net.size();
	outputScenario.close();
	return;
}
// frees allocated memory
void RICE::RICE_delete(){
	carbon->carbonDelete();
	climate->climateDelete();
	econ->econDelete();
	return;
}