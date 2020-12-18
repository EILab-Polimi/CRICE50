#include "Temp.h"

#include <iostream>
#include <fstream>
using namespace std;
// constructor
Temp::Temp(){

}
// destructor
Temp::~Temp(){

}

////// DESCRIPTION OF THE WITCH TEMPERATURE MODEL //////
// constructor
WITCHTemp::WITCHTemp(){

}
// destructor
WITCHTemp::~WITCHTemp(){

}// allocates memory for the climate component
void WITCHTemp::allocate(int hrzn){
	tatm = new double[hrzn + 1];
	tocean = new double[hrzn + 1];
	t = 0;
	readParams();
	return;
}
// read parameters from text file
// and stores them in the params struct
// and setting initial conditions
void WITCHTemp::readParams(){
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
void WITCHTemp::nextStep(double forc){
	tatm[t+1] = tatm[t] +
		params.sigma1 * (forc - params.lambda * tatm[t] +
						 - params.sigma2 * (tatm[t] - tocean[t]));
	tocean[t+1] = tocean[t] + params.heat_ocean * (tatm[t] - tocean[t]);
	cout << "\t\tWITCH climate evolves to next step:" << endl;
	cout << "\t\t" << tatm[t] << "\t" << tocean[t] << endl;
	t++;
	return;
}
// frees allocated memory
void WITCHTemp::temp_delete(){
	delete[] tatm;
	delete[] tocean;
	return;
}


////// DESCRIPTION OF THE DICE TEMPERATURE MODEL //////
// constructor
DICETemp::DICETemp(){

}
// destructor
DICETemp::~DICETemp(){

}// allocates memory for the climate component
void DICETemp::allocate(int hrzn){
	tatm = new double[hrzn + 1];
	tocean = new double[hrzn + 1];
	t = 0;
	readParams();
	return;
}
// read parameters from text file
// and stores them in the params struct
// and setting initial conditions
void DICETemp::readParams(){
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
void DICETemp::nextStep(double forc){
	tatm[t+1] = tatm[t] +
		params.sigma1 * (forc - params.lambda * tatm[t] +
						 - params.sigma2 * (tatm[t] - tocean[t]));
	tocean[t+1] = tocean[t] + params.heat_ocean * (tatm[t] - tocean[t]);
	cout << "Here the climate evolves to next step: tatm becomes " << tatm[t] << endl;
	t++;
	return;
}
// frees allocated memory
void DICETemp::temp_delete(){
	delete[] tatm;
	delete[] tocean;
	return;
}
