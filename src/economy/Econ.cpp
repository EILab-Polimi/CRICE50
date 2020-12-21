#include "EconAgent.h"
#include "Econ.h"

#include<iostream>
#include<fstream> 

using namespace std;
// constructor
Econ::Econ(){

}
// destructor
Econ::~Econ(){

}
void Econ::readParams(){
	fstream in;
	string sJunk = "";
	in.open("./settings/globalEconParams.txt", ios_base::in);
	if (!in){
		cout << "The general settings file specified could not be found!" << endl;
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
}

// allocates memory for the economic component
Econ::Econ(int hrzn){
	readParams();
	e = new double[hrzn+1];
	agents_ptr = new EconAgent * [agents];
	t = 0;
	for (int nag=0; nag < agents; nag++){
		agents_ptr[nag] = new RICEEconAgent(hrzn);
	}
	return;
}
// simulates one step
void Econ::nextStep(){
	for (int ag=0; ag < agents; ag++){
		agents_ptr[ag]->nextStep();
		e[t] += agents_ptr[ag]->e[t];
	}
	cout << "\t\tHere the economy evolves to next step: " << t+1 << endl;
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