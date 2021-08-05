#include "EconAgent.h"
#include "Econ.h"

#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <cmath>

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
DecisionMakers stringToDecisionMakers(std::string input){
	if (input == "BAU") return BAU;
	if (input == "INPUT_STATIC") return INPUT_STATIC;	
	if (input == "INPUT_POLICY") return INPUT_POLICY;	
	return DMERR;
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
	horizon = hrzn;
	e = new double[horizon+1];
	cemutotper = new double[horizon + 1];
	RPCutoff = new double[horizon + 1];
	agents_ptr = new EconAgent * [agents];
	toCarbon = new double[1];
	prctiles = new double[4];
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
			agents_ptr[nag] = new RICEEconAgent(horizon, splitline[0], params.DMType);
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
	params.RPCutoffMetric = stringToRPMetricType(line);
	while (sJunk!="utility"){
		in >>sJunk;
	}
	in >> line;
	params.utilityType = stringToUtilityType(line);
	while (sJunk!="DecisionMakers"){
		in >>sJunk;
	}
	in >> line;
	params.DMType = stringToDecisionMakers(line);
	in.close();
	return;
}
// set decision variables using MOEA framework
void Econ::setEconVariables(double* vars){
	if (params.DMType == INPUT_STATIC){
		for (int ag=0; ag < agents; ag++){
			agents_ptr[ag]->setAgentVariables(vars + ag * agents_ptr[ag]->getNVars());
		}
	}
	else if (params.DMType == INPUT_POLICY){
		for (int ag=0; ag < agents; ag++){
			agents_ptr[ag]->setAgentVariables(vars);
		}
	}
	return;
}
// initializes vector of global states
void Econ::initializeStates(int numGlobalStates){
	nGlobalStates = numGlobalStates;
	globalStates = new double[numGlobalStates];
	for (int ag = 0; ag < agents; ag++){
		agents_ptr[ag]->globalStates = globalStates;
		agents_ptr[ag]->nGlobalStates = nGlobalStates;
	}
} 
// returns the pointer to a vector of global economic state variables
double* Econ::getStates(){
	return econStates;
}
// returns the pointer to a vector of global economic state variables
int Econ::getNStates(){
	return 0;
}
// passes the variables needed to nextStep in carbon cycle
void Econ::updateLinks(){
	toCarbon[0] = e[t];
	return;
}
// return number of variables for policy
int Econ::getNVars(){
	return agents_ptr[0]->getNVars();
}
// simulates one step
void Econ::nextStep(){
	// compute Rich Poor Cutoff
	// get value for cutoff from every agent
	double* tatm = fromClimate;
	double RPCutoffValues[agents];
	for (int ag=0; ag < agents; ag++){
		RPCutoffValues[ag] = agents_ptr[ag]->getValueForRPCutoff();
	}
	// sort values and compute cutoff
	std::sort(RPCutoffValues,RPCutoffValues+agents);
	// double RPCutoff = 0.0;
	RPCutoff[t] = 0.0;
	switch (params.RPCutoffMetric){
		case MEAN:
			for (int ag=0; ag < agents; ag++){
				RPCutoff[t] += RPCutoffValues[ag];
			}
			RPCutoff[t] = RPCutoff[t] / agents;
			break;
		case MEDIAN:
			RPCutoff[t] = RPCutoffValues[agents/2];
			break;
		case METRICERR:
			std::cerr <<
				 "Please insert an available metric for RPCutoff" 
				 << std::endl;
	}
	// nextStep in each agent
	e[t] = 0.0;
	cemutotper[t] = 0.0;
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
	updateLinks();
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
// computes Gini coefficient for 2100
double Econ::computeGini(){
	double num = 0.0, denum= 0.0;
	for (int i=0; i < agents; i++){
		for (int j=0; j < agents; j++){
			num += std::abs(agents_ptr[i]->getGDPpc(17) - agents_ptr[j]->getGDPpc(17));
			denum += agents_ptr[j]->getGDPpc(17);
		}
	}
	return num/(2*denum);
}
// computes GDPpc percentiles for 2100
double* Econ::computePrctiles(){
	std::vector<double> GDPpcDist;
	for (int ag=0; ag < agents; ag++){
		int bins = round(agents_ptr[ag]->getPop(17));
		for (int n=0; n < bins; n++){
			GDPpcDist.push_back(agents_ptr[ag]->getGDPpc(17));
		}
	}
	std::sort(GDPpcDist.begin(), GDPpcDist.end());
	*(prctiles) = GDPpcDist[round(GDPpcDist.size()/100*90)] * 1000.0;
	*(prctiles+1) = GDPpcDist[round(GDPpcDist.size()/100*80)] * 1000.0;
	*(prctiles+2) = GDPpcDist[round(GDPpcDist.size()/100*20)] * 1000.0;
	*(prctiles+3) = GDPpcDist[round(GDPpcDist.size()/100*10)] * 1000.0;
	return prctiles;
}
// computes GDPpc percentiles for 2100
double Econ::computePrctiles7525(){
	double ineq = 0.0;
	*(prctiles) = 0.0;
	*(prctiles+1) = 0.0;
	*(prctiles+2) = 0.0;
	*(prctiles+3) = 0.0;
	for (int tidx=12; tidx<23; tidx++){
		std::vector<double> GDPpcDist;
		for (int ag=0; ag < agents; ag++){
			int bins = round(agents_ptr[ag]->getPop(tidx));
			for (int n=0; n < bins; n++){
				GDPpcDist.push_back(agents_ptr[ag]->getGDPpc(tidx));
			}
		}
		std::sort(GDPpcDist.begin(), GDPpcDist.end());
		*(prctiles) = GDPpcDist[round(GDPpcDist.size()/100*90)] * 1000.0 / 11.0;
		// *(prctiles+1) += GDPpcDist[round(GDPpcDist.size()/100*80)] * 1000.0 / 11.0;
		// *(prctiles+2) += GDPpcDist[round(GDPpcDist.size()/100*20)] * 1000.0 / 11.0;
		*(prctiles+3) = GDPpcDist[round(GDPpcDist.size()/100*10)] * 1000.0 / 11.0 ;
		ineq += prctiles[0]/prctiles[3] / 11.0;
	}
	return ineq;
}
double Econ::computeNET(){
	double NET = 0.0;
	for (int tidx=0; tidx <= 27; tidx++){
		for (int ag=0; ag < agents; ag++){
			NET += std::max(0.0, - agents_ptr[ag]->getEmissions(tidx));
		}
	}
	return NET;
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
	delete[] RPCutoff;
	delete[] globalStates;
	delete[] toCarbon;
	delete[] prctiles;
	return;
}