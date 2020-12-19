#include "EconAgent.h"
#include "Econ.h"

#include <iostream>

using namespace std;
// constructor
Econ::Econ(){

}
// destructor
Econ::~Econ(){

}
// allocates memory for the economic component
void Econ::allocate(int nag, int hrzn){
	agents = nag;
	agents_ptr = new EconAgent[nag];
	t = 0;
	for (int nag=0; nag < agents; nag++){
		agents_ptr[nag].allocate(hrzn);
	}
	params.elasmu = 1.45;
	params.prstp = 0.015;
	params.ineqav = 1.0;
	e = new double[hrzn+1];
	return;
}
// simulates one step
void Econ::nextStep(){
	for (int ag=0; ag < agents; ag++){
		agents_ptr[ag].nextStep();
		e[t] += agents_ptr[ag].e[t];
	}
	cout << "\t\tHere the economy evolves to next step: " << t+1 << endl;
	t++;
	return;
}
// frees allocated memory
void Econ::econDelete(){
	for (int nag=0; nag < agents; nag++){
		agents_ptr[nag].econAgentDelete();
	}
	delete[] agents_ptr;
	delete[] e;
	return;
}