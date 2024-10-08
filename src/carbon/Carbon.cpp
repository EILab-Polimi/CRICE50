#include "Carbon.h"

#include <iostream>
#include <sstream>
#include <algorithm>

// constructor
Carbon::Carbon(){

}
// destructor
Carbon::~Carbon(){

}


// =======  DICE-Carbon module =============

// constructor
DICECarbon::DICECarbon(){

}
// destructor
DICECarbon::~DICECarbon(){

}
// allocates the carbon component
DICECarbon::DICECarbon(int hrzn){
	
	mat = new double[hrzn + 1];  // Carbon concentration increase in Atmosphere [GtC from 1750]
	mup = new double[hrzn + 1];  // Carbon concentration increase in Shallow Oceans [GtC from 1750]
	mlo = new double[hrzn + 1];  // Carbon concentration increase in Lower Oceans [GtC from 1750]
	forc = new double[hrzn + 1];  // Increase in Radiative Forcing [W/m2 from 1900]
	forcoth = new double[hrzn + 1];  // Exogenous forcing from other greenhouse gases [W/m2]
	statesVector = new double[3];
	toClimate = new double[1];
	readParams();
	t = 0;
}
// simulates next step
void DICECarbon::nextStep(){

	e = fromEcon[0];
	// OGHG Forcing
	forcoth[t] = params.fex0 + (params.fex1 - params.fex0) * std::min((double)t/17.0,1.0);
	// Carbon concentration increase in Atmosphere 
	mat[t+1] = mat[t] * params.b11 + mup[t] * params.b21 + e * (5/3.666);
	// Carbon concentration increase in Shallow Oceans 
	mup[t+1] = mat[t] * params.b12 + mup[t] * params.b22 + mlo[t] * params.b32;
	// Carbon concentration increase in Lower Oceans 
	mlo[t+1] = mup[t] * params.b23 + mlo[t] * params.b33;
	// Total radiative forcing
	forc[t] = params.fco22x * ((log((mat[t]/588.000))/log(2))) + forcoth[t];

	// std::cout << "\t\tDICE carbon cycle evolves to next step: " << std::endl;
	// std::cout <<  "\t\t" << mat[t] << "\t" << mup[t] << "\t" << mlo[t] << "\t" << forc[t] << "\t" << t+1 << std::endl;
	updateLinks();
	t++;
	return;
}

// read parameters from text file
// and stores them in the params struct
// and setting initial conditions
void DICECarbon::readParams(){
	std::fstream in;
	std::string sJunk = "";
	in.open("./settings/DICEcarbonParams.txt", std::ios_base::in);
	if (!in){
		std::cout << "The DICE carbon settings file could not be found!" << std::endl;
	    exit(1);
	}
	while (sJunk!="b12"){
		in >>sJunk;
	}
	in >> params.b12;
	while (sJunk!="b23"){
		in >>sJunk;
	}
	in >> params.b23;
	while (sJunk!="mateq"){
		in >>sJunk;
	}
	in >> params.mateq;
	while (sJunk!="mupeq"){
		in >>sJunk;
	}
	in >> params.mupeq;
	while (sJunk!="mloeq"){
		in >>sJunk;
	}
	in >> params.mloeq;
	params.b11 = 1.0 - params.b12;
    params.b21 = params.b12 * params.mateq / params.mupeq;
    params.b22 = 1.0 - params.b21 - params.b23;
    params.b32 = params.b23 * params.mupeq/params.mloeq;
    params.b33 = 1.0 - params.b32 ;
	while (sJunk!="fco22x"){
		in >>sJunk;
	}
	in >> params.fco22x;
	while (sJunk!="fex0"){
		in >>sJunk;
	}
	in >> params.fex0;
	while (sJunk!="fex1"){
		in >>sJunk;
	}
	in >> params.fex1;
	while (sJunk!="mat0"){
		in >>sJunk;
	}
	in >> mat[0];
	while (sJunk!="mup0"){
		in >>sJunk;
	}
	in >> mup[0];
	while (sJunk!="mlo0"){
		in >>sJunk;
	}
	in >> mlo[0];
	in.close();
	return;
}
// Writes header for output
void DICECarbon::writeHeader(std::fstream& output){
	output << "MAT" << "\t" <<
		"MUP" << "\t" <<
		"MLO" << "\t" <<
		"FORC" << "\t" <<
		"FORCOTH" << "\t" ;
	t = 0;
}
// Writes step to output
void DICECarbon::writeStep(std::fstream& output){
	output << mat[t] << "\t" <<
		mup[t] << "\t" <<
		mlo[t] << "\t" <<
		forc[t] << "\t" <<
		forcoth[t] << "\t" ;
	t++;
}
//get states
double* DICECarbon::getStates(){
	statesVector[0] = mat[t];
	statesVector[1] = mup[t];
	statesVector[2] = mlo[t];
	return statesVector;
}
// get number of states
int DICECarbon::getNStates(){
	return 3;
}
// update shared info with climate
void DICECarbon::updateLinks(){
	toClimate[0] = forc[t];
	return;
}
// frees allocated memory
void DICECarbon::carbonDelete(){
	delete[] mat;
	delete[] mup;
	delete[] mlo;
	delete[] forc;
	delete[] forcoth;
	delete[] statesVector;
	delete[] toClimate;
	return;
}


void DICECarbon::reset(){
	t = 0;
	return;
}


// ====  WITCH-Carbon module =============


// constructor
WITCHCarbon::WITCHCarbon(){

}
// destructor
WITCHCarbon::~WITCHCarbon(){

}
// allocates the carbon component
WITCHCarbon::WITCHCarbon(int hrzn){
	mat = new double[hrzn + 1];  // Carbon concentration increase in Atmosphere [GtC from 1750]
	mup = new double[hrzn + 1];  // Carbon concentration increase in Shallow Oceans [GtC from 1750]
	mlo = new double[hrzn + 1];  // Carbon concentration increase in Lower Oceans [GtC from 1750]
	forc = new double[hrzn + 1];  // Increase in Radiative Forcing [W/m2 from 1900]
	statesVector = new double[3];
	toClimate = new double[1];
	readParams();
	t = 0;
}
// simulates next step
void WITCHCarbon::nextStep(){

	e = fromEcon[0];
	// Carbon concentration increase in Atmosphere 
	mat[t+1] = mat[t] * params.at2at + mup[t] * params.up2at +
		e * 5 * params.CO2toC;
	// Carbon concentration increase in Shallow Oceans 
	mup[t+1] = mat[t] * params.at2up + mup[t] * params.up2up +
		mlo[t] * params.lo2up;
	// Carbon concentration increase in Lower Oceans 
	mlo[t+1] = mlo[t] * params.lo2lo + mup[t] * params.up2lo;
	// Total radiative forcing
	forc[t] = params.rfoth_I + ( 1 + params.rfoth_a) * 
		(params.rfc_alpha * (log(mat[t]) - log(params.rfc_beta)));


	// std::cout << "\t\tWITCH carbon cycle evolves to next step: " << std::endl;
	// std::cout <<  "\t\t" << mat[t] << "\t" << mup[t] << "\t" << mlo[t] << "\t" << forc[t] << "\t" << t+1 << std::endl;
	updateLinks();
	t++;
	return;
}
// read parameters from text file
// and stores them in the params struct
// and setting initial conditions
void WITCHCarbon::readParams(){
	params.CO2toC = 12.0/44.0;
	std::fstream in;
	std::string line;
	in.open("./data_ed57/data_climate_witch/cmphi.csv");
		if (!in){
		std::cout << "The cmphi file could not be found!" << std::endl;
	    exit(1);
	}
	while(std::getline(in, line)){
		line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
		std::istringstream s(line);
		std::string field;
		std::string splitline[3];
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[0].compare("atm")){
			if (!splitline[1].compare("atm")){
				params.at2at = stod(splitline[2]);
			}
			else if (!splitline[1].compare("upp")){
				params.at2up = stod(splitline[2]);
			}
		} 
		else if (!splitline[0].compare("upp")){
			if (!splitline[1].compare("atm")){
				params.up2at = stod(splitline[2]);
			}
			else if (!splitline[1].compare("upp")){
				params.up2up = stod(splitline[2]);
			}
			else if (!splitline[1].compare("low")){
				params.up2lo = stod(splitline[2]);
			}
		} 
		else if (!splitline[0].compare("low")){
			if (!splitline[1].compare("upp")){
				params.lo2up = stod(splitline[2]);
			}
			else if (!splitline[1].compare("low")){
				params.lo2lo = stod(splitline[2]);
			}
		} 
	}
	in.close();
	in.open("./data_ed57/data_climate_witch/rfc.csv");
		if (!in){
		std::cout << "The rfc file could not be found!" << std::endl;
	    exit(1);
	}
	while(std::getline(in, line)){
		line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
		std::istringstream s(line);
		std::string field;
		std::string splitline[3];
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[0].compare("co2")){
			if (!splitline[1].compare("alpha")){
				params.rfc_alpha = stod(splitline[2]);
			}
			else if (!splitline[1].compare("beta")){
				params.rfc_beta = stod(splitline[2]);
			}
		} 
	}
	in.close();
	in.open("./data_ed57/data_climate_witch/wcum_emi0.csv");
		if (!in){
		std::cout << "The wcum_emi0 file could not be found!" << std::endl;
	    exit(1);
	}
	while(std::getline(in, line)){
		line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
		std::istringstream s(line);
		std::string field;
		std::string splitline[3];
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[0].compare("co2")){
			if (!splitline[1].compare("atm")){
				mat[0] = stod(splitline[2]);
			}
			else if (!splitline[1].compare("upp")){
				mup[0] = stod(splitline[2]);
			}
			else if (!splitline[1].compare("low")){
				mlo[0] = stod(splitline[2]);
			}
		} 
	}
	in.close();
	in.open("./data_ed57/data_climate_witch/oghg_coeff.csv");
		if (!in){
		std::cout << "The oghg_coeff file could not be found!" << std::endl;
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
		if (!splitline[0].compare("intercept")){
			params.rfoth_I = stod(splitline[1]);
		}
		else if (!splitline[0].compare("slope")){
			params.rfoth_a = stod(splitline[1]);
		}
	}
	in.close();
	return;
}
//writes header for output
void WITCHCarbon::writeHeader(std::fstream& output){
	output << "MAT" << "\t" <<
		"MUP" << "\t" <<
		"MLO" << "\t" <<
		"FORC" << "\t" ;
	t = 0;
}
//writes step to output
void WITCHCarbon::writeStep(std::fstream& output){
	output << mat[t] << "\t" <<
		mup[t] << "\t" <<
		mlo[t] << "\t" <<
		forc[t] << "\t" ;
	t++;
}
//get states
double* WITCHCarbon::getStates(){
	statesVector[0] = mat[t];
	statesVector[1] = mup[t];
	statesVector[2] = mlo[t];
	return statesVector;
}
// get number of states
int WITCHCarbon::getNStates(){
	return 3;
}
// update shared info with climate
void WITCHCarbon::updateLinks(){
	toClimate[0] = forc[t];
	return;
}
// frees allocated memory
void WITCHCarbon::carbonDelete(){
	delete[] mat;
	delete[] mup;
	delete[] mlo;
	delete[] forc;
	delete[] statesVector;
	delete[] toClimate;
	return;
}


void WITCHCarbon::reset(){
	t = 0;
	return;
}


// ====   FAIR-Carbon module ========

// constructor
FAIRCarbon::FAIRCarbon(){

}
// destructor
FAIRCarbon::~FAIRCarbon(){

}
// allocates the carbon component
FAIRCarbon::FAIRCarbon(int hrzn){
	params.horizon = hrzn;
	params.r0       = 35.0;
	params.rc       = 0.019;
	params.rt       = 4.165;
	params.iirf_h   = 100.0;
	params.iirf_max = 97.0;
	params.iirf = 0.0;
	alpha = new double[params.horizon+1];
	params.ppm_to_gtc = 2.124;
	params.a[0] = 0.2173;
	params.a[1] = 0.2240;
	params.a[2] = 0.2824;
	params.a[3] = 0.2763;
	params.tau[0] = 1000000;
	params.tau[1] = 394.4;
	params.tau[2] = 36.54;
	params.tau[3] = 4.304;
	params.f2x = 3.71;
	carbon_boxes = new double*[params.horizon+1];
	for (int subt = 0; subt < params.horizon+1; subt++){
		carbon_boxes[subt] = new double[4];
	}
	carbon_boxes[0][0] = 58.71337292; // * ppm_to_gtc; //127.159
	carbon_boxes[0][1] = 43.28685286; // * ppm_to_gtc; //93.313;
	carbon_boxes[0][2] = 18.44893718; // * ppm_to_gtc; //37.840;
	carbon_boxes[0][3] = 3.81581747; // * ppm_to_gtc; //7.721
	taunew = 0.0;
	params.c_pi = 278.0; // * 2.124;
	c = new double[params.horizon+1];
	c[0] = params.c_pi;
	for (int idx = 0; idx < 4; idx++){
		c[0] += carbon_boxes[0][idx];
	}
	c_acc = new double[params.horizon+1];
	c_acc[0] = 305.1673751542558; //value in 2015 // * ppm_to_gtc; //597.0; //(400+197)
	forc = new double[params.horizon+1];
	forc[0] = 0.0;

	params.ranges[0] = 0.0;
	params.ranges[1] = 2000.0;
	params.ranges[2] = -1.0;
	params.ranges[3] = 10.0;
	params.nnprms[0] = -6.66006035e+02;
	params.nnprms[1] = 2.09443154e+02;
	params.nnprms[2] = -4.83968920e+00;
	params.nnprms[3] = 2.31243377e+00;
	params.nnprms[4] = 2.75031497e+00;
	params.nnprms[5] = 8.89902682e+02;
	params.nnprms[6] = 2.40146799e+00;
	params.nnprms[7] = 6.83316702e-02;
	params.nnprms[8] = 2.89753011e-02;
	rfothidx[0] = 26;
	rfothidx[1] = 45;
	rfothidx[2] = 60;
	rfothidx[3] = 85;

	std::ifstream forc_file;
	for (int rfidx = 0; rfidx < 4; rfidx++){
		forc_file.open("./src/carbon/RFoth_"+std::to_string((unsigned long long int) rfothidx[rfidx])+".txt", std::ios_base::in);
		if(!forc_file){
			std::cout << "The other forcing file specified could not be found!" << std::endl;
			exit(1);
		}
		for (int idx=0; idx < 486 ; idx++){
			forc_file >> rfoth[rfidx][idx];
		}
		forc_file.close();		
	}
	params.rfoth_type = 1; // rcp4.5
	statesVector = new double[1];
	toClimate = new double[1];
	t = 0;
	return;
}
// to correct as FAIR has been corrected
void FAIRCarbon::computeAlpha(){
	double input[2];
	//normalize inputs
	input[0] = (c_acc[t] - params.ranges[0]) /	(params.ranges[1] - params.ranges[0]);
	input[1] = (tatm - params.ranges[2]) / (params.ranges[3] - params.ranges[2]);

	//compute alpha via ANN 
	alpha[t] = params.nnprms[0] + \
	    (params.nnprms[1]) * \
	    	(-1.0 + 2.0 / \
	        	( 1.0 + exp( -2.0 * \
	          	(params.nnprms[2] + \
	            	(params.nnprms[3]) * input[0] + \
	            	(params.nnprms[4]) * input[1] )))) +\
	    (params.nnprms[5]) * \
	    	(-1.0 + 2.0 / \
	        	( 1.0 + exp( -2.0 * \
	        	(params.nnprms[6] + \
	            	(params.nnprms[7]) * input[0] + \
	            	(params.nnprms[8]) * input[1] ))));
	if (alpha[t] < 1e-3){
		alpha[t] = alpha[t-1];
	}
	return;
}
// sampling uncertainty
void FAIRCarbon::sampleUnc(){
	// if (config->rfoth_unc==1){
		params.rfoth_type = floor( rand() * (1.0/RAND_MAX) * 4 );
	// }
	return;
}

// simulates next step
void FAIRCarbon::nextStep(){

	e = fromEcon[0] * 12/44;
	tatm = fromClimate[0];
	params.iirf = std::min(params.r0 + params.rc*c_acc[t] + params.rt*tatm, params.iirf_max);
	computeAlpha();
	for (int box = 0; box < 4; box++){
		taunew = params.tau[box] * alpha[t];
		carbon_boxes[t+1][box] = 
			carbon_boxes[t][box] * exp(-1.0/taunew) + 
			params.a[box] * e / params.ppm_to_gtc;
	}
	c[t+1] = params.c_pi;
	for (int box = 0; box < 4; box++){
		c[t+1] += carbon_boxes[t+1][box];
	}
	c_acc[t+1] = c_acc[t] + e - (c[t+1]-c[t]) * params.ppm_to_gtc;
	if (t < 486){
		forc[t+1] = params.f2x/log(2.0) * log(c[t+1]/params.c_pi) + rfoth[params.rfoth_type][t];
	}
	else{
		forc[t+1] = params.f2x/log(2.0) * log(c[t+1]/params.c_pi) + rfoth[params.rfoth_type][485];
	}
	updateLinks();
	t++;
	return;
}


void FAIRCarbon::reset(){
	t = 0;
	return;
}
//writes header for output
void FAIRCarbon::writeHeader(std::fstream& output){
	output << "CCONC" << "\t" <<
		"FORC" << "\t" ;
	t = 0;
}
//writes step to output - every 5 years to match Econ time step
void FAIRCarbon::writeStep(std::fstream& output){
	output << c[t] << "\t" <<
		forc[t] << "\t" ;
	t+=5;
}
//get states
double* FAIRCarbon::getStates(){
	statesVector[0] = c[t];
	return statesVector;
}
// get number of states
int FAIRCarbon::getNStates(){
	return 1;
}
// update shared info with climate
void FAIRCarbon::updateLinks(){
	toClimate[0] = forc[t+1];
	return;
}
// frees allocated memory
void FAIRCarbon::carbonDelete(){
	for (int subt = 0; subt < params.horizon+1; subt++){
		delete[] carbon_boxes[subt];
	}
	delete[] carbon_boxes;
	delete[] alpha;
	delete[] c_acc;
	delete[] forc;
	delete[] c;
	delete[] statesVector;
	delete[] toClimate;
	return;
}