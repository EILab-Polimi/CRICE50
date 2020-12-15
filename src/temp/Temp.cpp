#include "Temp.h"

#include <iostream>
#include <fstream>
using namespace std;
// constructor
Temp::Temp(){

}
// allocates memory for the climate component
void Temp::allocate(int hrzn){
	tatm = new double[hrzn + 1];
	tocean = new double[hrzn + 1];
	t = 0;
	readParams();
	return;
}
// read parameters from text file
// and stores them in the params struct
// and setting initial conditions
void Temp::readParams(){
	fstream in;
	string sJunk = "";
	in.open("./settings/temp_params.txt", ios_base::in);
	if (!in){
		cout << "The general settings file specified could not be found!" << endl;
	    exit(1);
	}
	while (sJunk!="sigma1"){
		in >>sJunk;
	}
	in >> params.sigma1;
	while (sJunk!="sigma2"){
		in >>sJunk;
	}
	in >> params.sigma2;
	while (sJunk!="lambda"){
		in >>sJunk;
	}
	in >> params.lambda;
	while (sJunk!="heat_ocean"){
		in >>sJunk;
	}
	in >> params.heat_ocean;
	while (sJunk!="tatm0"){
		in >>sJunk;
	}
	in >> tatm[0];
	while (sJunk!="tocean0"){
		in >>sJunk;
	}
	in >> tocean[0];
	in.close();
}
// simulates one time step
void Temp::nextStep(double forc){
	tatm[t+1] = tatm[t] +
		params.sigma1 * (forc - params.lambda * tatm[t] +
						 - params.sigma2 * (tatm[t] - tocean[t]));
	tocean[t+1] = tocean[t] + params.heat_ocean * (tatm[t] - tocean[t]);
	cout << "Here the climate evolves to next step: tatm becomes " << tatm[t] << endl;
	t++;
	return;
}
// frees allocated memory
void Temp::temp_delete(){
	delete[] tatm;
	delete[] tocean;
	return;
}