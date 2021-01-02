#include "Climate.h"

#include <iostream>
#include <fstream>

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
	std::fstream in;
	std::string sJunk = "";
	in.open("./settings/WITCHclimateParams.txt", std::ios_base::in);
	if (!in){
		std::cout << "The WITCH climate settings file could not be found!" << std::endl;
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
	return;
}
// simulates one time step
void WITCHClimate::nextStep(double forc){
	tatm[t+1] = tatm[t] +
		params.sigma1 * (forc - params.lambda * tatm[t] +
						 - params.sigma2 * (tatm[t] - tocean[t]));
	tocean[t+1] = tocean[t] + params.heat_ocean * (tatm[t] - tocean[t]);
	std::cout << "\t\tWITCH climate evolves to next step:" << std::endl;
	std::cout << "\t\t" << tatm[t] << "\t" << tocean[t] << std::endl;
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
	std::fstream in;
	std::string sJunk = "";
	in.open("./settings/DICEclimateParams.txt", std::ios_base::in);
	if (!in){
		std::cout << "The DICE climate settings file could not be found!" << std::endl;
	    exit(1);
	}
	while (sJunk!="c1"){
		in >>sJunk;
	}
	in >> params.c1;
	while (sJunk!="c3"){
		in >>sJunk;
	}
	in >> params.c3;
	while (sJunk!="c4"){
		in >>sJunk;
	}
	in >> params.c4;
	while (sJunk!="fco22x"){
		in >>sJunk;
	}
	in >> params.fco22x;
	while (sJunk!="t2xco2"){
		in >>sJunk;
	}
	in >> params.t2xco2;
	while (sJunk!="tatm0"){
		in >>sJunk;
	}
	in >> tatm[0];
	while (sJunk!="tocean0"){
		in >>sJunk;
	}
	in >> tocean[0];
	in.close();
	return;
}
// simulates one time step
void DICEClimate::nextStep(double forc){
	tatm[t+1] = tatm[t] +
		params.c1 * (forc - params.fco22x / params.t2xco2 * tatm[t] +
						 - params.c3 * (tatm[t] - tocean[t]));
	tocean[t+1] = tocean[t] + params.c4 * (tatm[t] - tocean[t]);
	std::cout << "\t\tDICE climate evolves to next step:" << std::endl;
	std::cout << "\t\t" << tatm[t] << "\t" << tocean[t] << std::endl;
	t++;
	return;
}
// frees allocated memory
void DICEClimate::climateDelete(){
	delete[] tatm;
	delete[] tocean;
	return;
}
