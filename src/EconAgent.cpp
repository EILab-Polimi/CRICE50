#include "EconAgent.h"

#include <iostream>

using namespace std;
//constructor
EconAgent::EconAgent(){

}
// allocates the memory for an agent
void EconAgent::allocate(int hrzn){
	k = new double[hrzn + 1];
	y = new double[hrzn + 1];
	l = new double[hrzn + 1];
	tfp = new double[hrzn + 1];
	t = 0;
	return;
}
// simulates one time step
void EconAgent::nextStep(){
	cout << "Here an agent evolves to next step: " << t+1 << endl;
	t++;
	return;
}
// frees allocated memory
void EconAgent::econagent_delete(){
	delete[] k;
	delete[] y;
	delete[] l;
	delete[] tfp;
	return;
}