#include "EconAgent.h"

#include <iostream>

using namespace std;

EconAgent::EconAgent(){

}

void EconAgent::allocate(int hrzn){
	k = new double[hrzn];
	y = new double[hrzn];
	l = new double[hrzn];
	tfp = new double[hrzn];
	t = 0;
	return;
}

void EconAgent::nextStep(){
	cout << "Here an agent evolves to next step: " << t+1 << endl;
	t++;
	return;
}

void EconAgent::econagent_delete(){
	delete[] k;
	delete[] y;
	delete[] l;
	delete[] tfp;
	return;
}