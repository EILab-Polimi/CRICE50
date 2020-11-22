#include "EconAgent.h"
#include "Econ.h"

#include <iostream>

using namespace std;

Econ::Econ(){

}

void Econ::allocate(int nag, int hrzn){
	agents = nag;
	agents_ptr = new EconAgent[nag];
	t = 0;
	for (int ag=0; ag < nag; ag++){
		agents_ptr[ag].allocate(hrzn);
	}
	return;
}

void Econ::nextStep(){
	for (int ag=0; ag < agents; ag++){
		agents_ptr[ag].nextStep();
	}
	cout << "Here the economy evolves to next step: " << t+1 << endl;
	t++;
	return;
}

void Econ::econ_delete(){
	for (int nag=0; nag < agents; nag++){
		agents_ptr[nag].econagent_delete();
	}
	delete[] agents_ptr;
	return;
}