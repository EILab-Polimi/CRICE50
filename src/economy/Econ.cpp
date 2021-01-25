#include "EconAgent.h"
#include "Econ.h"

#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include <algorithm>
#include <math.h>

RPMetricType stringToRPMetricType(std::string input){
	if (input == "MEAN") return MEAN;
	if (input == "MEDIAN") return MEDIAN;	
	return METRICERR;
}
UtilityType stringToUtilityType(std::string input){
	if (input == "COOP") return COOP;
	if (input == "NON-COOP") return NON_COOP;	
	return UTILITYERR;
}
// constructor
Econ::Econ(){

}
// destructor
Econ::~Econ(){

}
// allocates memory for the economic component
Econ::Econ(int hrzn){
	readParams();
	e = new double[hrzn+1];
	cemutotper = new double[hrzn + 1];
	agents_ptr = new EconAgent * [agents];
	t = 0;
	std::fstream in;
	std::string line;
	std::string sJunk = "";
	int nag = 0;
	in.open("./data_ed57/data_economy/k0.csv");
	if (!in){
		std::cerr << "The k0 file could not be found!" << std::endl;
	    exit(1);
	}
	while(std::getline(in, line) && nag < agents){
		line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
		std::istringstream s(line);
		std::string field;
		std::string splitline[2];
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (splitline[0].compare("n")){
			agents_ptr[nag] = new RICEEconAgent(hrzn, splitline[0]);
			nag++;
		}
	}
}
// read general economic parameters
void Econ::readParams(){
	std::fstream in;
	std::string sJunk = "";
	std::string line;
	in.open("./settings/globalEconParams.txt", std::ios_base::in);
	if (!in){
		std::cerr << 
			"The general economic settings file could not be found!" 
			<< std::endl;
	    exit(1);
	}
	while (sJunk!="nagents"){
		in >>sJunk;
	}
	in >> agents;
	while (sJunk!="elasmu"){
		in >>sJunk;
	}
	in >> params.elasmu;
	while (sJunk!="prstp"){
		in >>sJunk;
	}
	in >> params.prstp;
	while (sJunk!="ineqav"){
		in >>sJunk;
	}
	in >> params.ineqav;
	while (sJunk!="RPCutoffMetric"){
		in >>sJunk;
	}
	in >> line;
	// std::cout << line << std::endl;
	params.RPCutoffMetric = stringToRPMetricType(line);
	while (sJunk!="utility"){
		in >>sJunk;
	}
	in >> line;
	// std::cout << line << std::endl;
	params.utilityType = stringToUtilityType(line);
	in.close();
	return;
}
// set decision variables using MOEA framework
void Econ::setEconVariables(double* vars){
	for (int ag=0; ag < agents; ag++){
		agents_ptr[ag]->setAgentVariables(vars + ag*58*2);
	}
	return;
}
// simulates one step
void Econ::nextStep(double* tatm){
	// compute Rich Poor Cutoff
	// get value for cutoff from every agent
	double RPCutoffValues[agents];
	for (int ag=0; ag < agents; ag++){
		RPCutoffValues[ag] = agents_ptr[ag]->getValueForRPCutoff();
	}
	// sort values and compute cutoff
	std::sort(RPCutoffValues,RPCutoffValues+agents);
	double RPCutoff = 0.0;
	switch (params.RPCutoffMetric){
		case MEAN:
			for (int ag=0; ag < agents; ag++){
				RPCutoff += RPCutoffValues[ag];
			}
			RPCutoff = RPCutoff / agents;
			break;
		case MEDIAN:
			RPCutoff = RPCutoffValues[agents/2];
			break;
		case METRICERR:
			std::cerr <<
				 "Please insert an available metric for RPCutoff" 
				 << std::endl;
	}
	// nextStep in each agent
	e[t] = 0.0;
	double pop[agents];
	double sum_pop = 0.0, sum_cpc = 0.0;
	double cpc[agents];
	for (int ag=0; ag < agents; ag++){
		// compute next step for the agent
		agents_ptr[ag]->nextStep(tatm, RPCutoff);
		// retrieve relevant data from the agent
		e[t] += agents_ptr[ag]->getEmissions(t);
		pop[ag] = agents_ptr[ag]->getPop(t);
		sum_pop += pop[ag];
		cpc[ag] = agents_ptr[ag]->getCPC(t);
		sum_cpc += cpc[ag];
	}
	// compute period utility and update utiltiy
	if (params.utilityType == COOP){
		for (int ag=0; ag < agents; ag++){
			cemutotper[t] += pop[ag]/sum_pop * 							// pop weights
				pow(cpc[ag], 1.0 - params.ineqav);						// cpc & ineqav
		}
		cemutotper[t] = pow((1 + params.prstp), -5.0*t) *				// rr(t)
			(pow(cemutotper[t], (1.0 - params.elasmu)/					// cpc & elasmu
				(1.0 - params.ineqav))									//     + ineqav
			/ (1.0 - params.elasmu) - 1.0);								// elasmu
		utility += cemutotper[t] * 5 * pow(10,-8);						// update utility
	}
	else if (params.utilityType == NON_COOP){
		for (int ag=0; ag < agents; ag++){
			cemutotper[t] = 
				pow(cpc[ag], params.elasmu) / 							// NEGISHI
				pow(sum_cpc, params.elasmu) *							// NEGISHI
				pop[ag] / pow(1 + params.prstp, 5*t) *					// POP and rr(t)
				((pow(cpc[ag], 1.0 - params.elasmu) - 1.0) / 			// PERIODU
				(1.0 - params.elasmu) - 1.0);							// PERIODU
		}
		utility += 5 * 0.0001 * cemutotper[t];							// update utility
	}
	// std::cout << "\t\tHere the economy evolves to next step: " << t+1 << std::endl;
	t++;
	return;
}
//writes header of file
void Econ::writeHeader(std::fstream& output){
	output << "E\tCEMUTOTPER\t";
	for (int ag=0; ag < agents; ag++){
		agents_ptr[ag]->writeHeader(output);
	}
	t = 0;
	return;
}
//writes steps to file
void Econ::writeStep(std::fstream& output){
	output << e[t] << "\t"
		<< cemutotper[t] << "\t" ;
	for (int ag=0; ag < agents; ag++){
		agents_ptr[ag]->writeStep(output);
	}
	t++;
	return;
}
// frees allocated memory
void Econ::econDelete(){
	for (int nag=0; nag < agents; nag++){
		agents_ptr[nag]->econAgentDelete();
		delete agents_ptr[nag];
	}
	delete[] agents_ptr;
	delete[] e;
	delete[] cemutotper;
	return;
}