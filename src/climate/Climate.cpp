#include "Climate.h"

#include <iostream>
#include <fstream>
using namespace std;
// constructor
Climate::Climate(){

}
// destructor
Climate::~Climate(){

}

////// DESCRIPTION OF THE WITCH ClimateERATURE MODEL //////
// constructor
WITCHClimate::WITCHClimate(){

}
// destructor
WITCHClimate::~WITCHClimate(){

}// allocates memory for the climate component
WITCHClimate::WITCHClimate(int hrzn){
	tatm = new double[hrzn + 1];
	tocean = new double[hrzn + 1];
	t = 0;
	readParams();
}
// read parameters from text file
// and stores them in the params struct
// and setting initial conditions
void WITCHClimate::readParams(){
	fstream in;
	string sJunk = "";
	in.open("./settings/WITCHclimateParams.txt", ios_base::in);
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
void WITCHClimate::nextStep(double forc){
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
void WITCHClimate::climateDelete(){
	delete[] tatm;
	delete[] tocean;
	return;
}


////// DESCRIPTION OF THE DICE ClimateERATURE MODEL //////
// constructor
DICEClimate::DICEClimate(){

}
// destructor
DICEClimate::~DICEClimate(){

}// allocates memory for the climate component
DICEClimate::DICEClimate(int hrzn){
	tatm = new double[hrzn + 1];
	tocean = new double[hrzn + 1];
	t = 0;
	readParams();
}
// read parameters from text file
// and stores them in the params struct
// and setting initial conditions
void DICEClimate::readParams(){
	fstream in;
	string sJunk = "";
	in.open("./settings/DICEclimateParams.txt", ios_base::in);
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
void DICEClimate::nextStep(double forc){
	tatm[t+1] = tatm[t] +
		params.sigma1 * (forc - params.lambda * tatm[t] +
						 - params.sigma2 * (tatm[t] - tocean[t]));
	tocean[t+1] = tocean[t] + params.heat_ocean * (tatm[t] - tocean[t]);
	cout << "Here the climate evolves to next step: tatm becomes " << tatm[t] << endl;
	t++;
	return;
}
// frees allocated memory
void DICEClimate::climateDelete(){
	delete[] tatm;
	delete[] tocean;
	return;
}
