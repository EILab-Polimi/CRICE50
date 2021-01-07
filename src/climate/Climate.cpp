#include "Climate.h"

#include<iostream>
#include<fstream>
#include<sstream>

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
	std::string line;
	in.open("./data_ed57/data_climate_witch/tempc.csv");
		if (!in){
		std::cout << "The tempc file could not be found!" << std::endl;
	    exit(1);
	}
	while(std::getline(in, line)){
		line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
		std::istringstream s(line);
		std::string field;
		std::string splitline[2];
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[0].compare("heat_ocean")){
			params.heat_ocean = stod(splitline[1]);
		} 
		else if (!splitline[0].compare("lambda")){
			params.lambda = stod(splitline[1]);
		} 
		else if (!splitline[0].compare("sigma1")){
			params.sigma1 = stod(splitline[1]);
		} 
		else if (!splitline[0].compare("sigma2")){
			params.sigma2 = stod(splitline[1]);
		} 
	}
	in.close();
	tatm[0] = 0.85;
	tocean[0] = 0.0068;
	return;
}
// simulates one time step
void WITCHClimate::nextStep(double forc){
	tatm[t+1] = tatm[t] +
		params.sigma1 * (forc - params.lambda * tatm[t]
						 - params.sigma2 * (tatm[t] - tocean[t]));
	tocean[t+1] = tocean[t] + params.heat_ocean * (tatm[t] - tocean[t]);
	// std::cout << "\t\tWITCH climate evolves to next step:" << std::endl;
	// std::cout << "\t\t" << tatm[t] << "\t" << tocean[t] << std::endl;
	t++;
	return;
}
void WITCHClimate::writeHeader(std::fstream& output){
	output << "TATM" << "\t" <<
		"TOCEAN" << "\t";
	t = 0;
}
//writes step to output
void WITCHClimate::writeStep(std::fstream& output){
	output << tatm[t] << "\t" <<
		tocean[t] << "\t" ;
	t++;
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
	// std::cout << "\t\tDICE climate evolves to next step:" << std::endl;
	// std::cout << "\t\t" << tatm[t] << "\t" << tocean[t] << std::endl;
	t++;
	return;
}
void DICEClimate::writeHeader(std::fstream& output){
	output << "TATM" << "\t" <<
		"TOCEAN" << "\t";
	t = 0;
}
//writes step to output
void DICEClimate::writeStep(std::fstream& output){
	output << tatm[t] << "\t" <<
		tocean[t] << "\t" ;
	t++;
}
// frees allocated memory
void DICEClimate::climateDelete(){
	delete[] tatm;
	delete[] tocean;
	return;
}
