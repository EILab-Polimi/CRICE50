#include "EconAgent.h"
#include "Econ.h"

#include<iostream>
#include<fstream> 
#include<string>
#include<sstream>
#include<algorithm>
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
	agents_ptr = new EconAgent * [agents];
	t = 0;
	std::fstream in;
	std::string line;
	std::string sJunk = "";
	int nag = 0;
	in.open("./data_ed57/data_economy/k0.csv");
	if (!in){
		std::cout << "The k0 file could not be found!" << std::endl;
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
	in.open("./settings/globalEconParams.txt", std::ios_base::in);
	if (!in){
		std::cout << "The general economic settings file specified could not be found!" << std::endl;
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
	in.close();
	params.RPCutoffMetric = 0;
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
	// sort values
	std::sort(RPCutoffValues,RPCutoffValues+agents);
	double RPCutoff = 0.0;
	if (params.RPCutoffMetric==0){ // mean
		for (int ag=0; ag < agents; ag++){
			RPCutoff += RPCutoffValues[ag];
		}
		RPCutoff = RPCutoff / agents;
	}
	else if (params.RPCutoffMetric==1){ //median
		RPCutoff = RPCutoffValues[agents/2];
	}
	// nextStep in each agent
	e[t] = 0.0;
	for (int ag=0; ag < agents; ag++){
		agents_ptr[ag]->nextStep(tatm, RPCutoff);
		e[t] += agents_ptr[ag]->e[t];
	}
	// std::cout << "\t\tHere the economy evolves to next step: " << t+1 << std::endl;
	t++;
	return;
}
//writes header of file
void Econ::writeHeader(std::fstream& output){
	output << "E\t";
	for (int ag=0; ag < agents; ag++){
		agents_ptr[ag]->writeHeader(output);
	}
	t = 0;
	return;
}
//writes steps to file
void Econ::writeStep(std::fstream& output){
	output << e[t] << "\t";
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
	return;
}