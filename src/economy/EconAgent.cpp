#include "EconAgent.h"

#include <iostream>

using namespace std;
// constructor
EconAgent::EconAgent(){

}
// destructor
EconAgent::~EconAgent(){

}
// allocates the memory for an agent
void EconAgent::allocate(int hrzn){
	e = new double[hrzn + 1];
	t = 0;
	return;
}
// simulates one time step
void EconAgent::nextStep(){
	e[t] = 20.0 * max(0.0,min(1.0, 1.0 - ((double)t) / 5.0));
	cout << "\t\tHere an agent evolves to the step " << t+1 << " emitting (GtCO2):" << e[t] << endl;
	t++;
	return;
}
// frees allocated memory
void EconAgent::econAgentDelete(){
	delete[] e;
	return;
}