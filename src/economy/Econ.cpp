#include "EconAgent.h"
#include "Econ.h"

#include<iostream>
#include<fstream> 
#include<string>

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
	std::string sJunk = "";
	in.open("./settings/regionNAMES.txt", std::ios_base::in);
	if (!in){
		std::cout << "The region names file specified could not be found!" << std::endl;
	    exit(1);
	}
	for (int nag=0; nag < agents; nag++){
		std::string name;
		while (sJunk!=std::to_string(nag)){
		in >>sJunk;
		}
		in >> name;
		agents_ptr[nag] = new RICEEconAgent(hrzn, name);
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
	return;
}
// simulates one step
void Econ::nextStep(){
	for (int ag=0; ag < agents; ag++){
		agents_ptr[ag]->nextStep();
		e[t] += agents_ptr[ag]->e[t];
	}
	std::cout << "\t\tHere the economy evolves to next step: " << t+1 << std::endl;
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