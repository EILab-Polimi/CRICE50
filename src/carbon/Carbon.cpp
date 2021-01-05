#include "Carbon.h"

#include <iostream>

// constructor
Carbon::Carbon(){

}
// destructor
Carbon::~Carbon(){

}

////// DESCRIPTION OF THE DICE CARBON MODEL //////
// constructor
DICECarbon::DICECarbon(){

}
// destructor
DICECarbon::~DICECarbon(){

}
// allocates the carbon component
DICECarbon::DICECarbon(int hrzn){
	mat = new double[hrzn + 1];
	mup = new double[hrzn + 1];
	mlo = new double[hrzn + 1];
	forc = new double[hrzn + 1];
	forcoth = new double[hrzn + 1];
	readParams();
	t = 0;
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
// simulates next step
void DICECarbon::nextStep(double e){
	forcoth[t] = params.fex0 + (params.fex1 - params.fex0) * std::min(t/18,1);
	mat[t+1] = mat[t] * params.b11 + mup[t] * params.b21 + e * (5/3.666);
	mup[t+1] = mat[t] * params.b12 + mup[t] * params.b22 + mlo[t] * params.b32;
	mlo[t+1] = mup[t] * params.b23 + mlo[t] * params.b33;
	forc[t] = params.fco22x * ((log((mat[t]/588.000))/log(2))) + forcoth[t];
	std::cout << "\t\tDICE carbon cycle evolves to next step: " << std::endl;
	std::cout <<  "\t\t" << mat[t] << "\t" << mup[t] << "\t" << mlo[t] << "\t" << forc[t] << "\t" << t+1 << std::endl;
	t++;
	return;
}
//writes header for output
void DICECarbon::writeHeader(std::fstream& output){
	output << "MAT" << "\t" <<
		"MUP" << "\t" <<
		"MLO" << "\t" <<
		"FORC" << "\t" <<
		"FORCOTH" << "\t" ;
	t = 0;
}
//writes step to output
void DICECarbon::writeStep(std::fstream& output){
	output << mat[t] << "\t" <<
		mup[t] << "\t" <<
		mlo[t] << "\t" <<
		forc[t] << "\t" <<
		forcoth[t] << "\t" ;
	t++;
}
// frees allocated memory
void DICECarbon::carbonDelete(){
	delete[] mat;
	delete[] mup;
	delete[] mlo;
	delete[] forc;
	delete[] forcoth;
	return;
}


////// DESCRIPTION OF THE WITCH CARBON MODEL //////
// constructor
WITCHCarbon::WITCHCarbon(){

}
// destructor
WITCHCarbon::~WITCHCarbon(){

}
// allocates the carbon component
WITCHCarbon::WITCHCarbon(int hrzn){
	mat = new double[hrzn + 1];
	mup = new double[hrzn + 1];
	mlo = new double[hrzn + 1];
	forc = new double[hrzn + 1];
	readParams();
	t = 0;
}
// read parameters from text file
// and stores them in the params struct
// and setting initial conditions
void WITCHCarbon::readParams(){
	std::fstream in;
	std::string sJunk = "";
	in.open("./settings/WITCHcarbonParams.txt", std::ios_base::in);
	if (!in){
		std::cout << "The WITCH carbon settings specified could not be found!" << std::endl;
	    exit(1);
	}
	while (sJunk!="CO2toC"){
		in >>sJunk;
	}
	in >> params.CO2toC;
	while (sJunk!="at2at"){
		in >>sJunk;
	}
	in >> params.at2at;
	while (sJunk!="at2up"){
		in >>sJunk;
	}
	in >> params.at2up;
	while (sJunk!="up2at"){
		in >>sJunk;
	}
	in >> params.up2at;
	while (sJunk!="up2up"){
		in >>sJunk;
	}
	in >> params.up2up;
	while (sJunk!="up2lo"){
		in >>sJunk;
	}
	in >> params.up2lo;
	while (sJunk!="lo2up"){
		in >>sJunk;
	}
	in >> params.lo2up;
	while (sJunk!="lo2lo"){
		in >>sJunk;
	}
	in >> params.lo2lo;
	while (sJunk!="rfoth_a"){
		in >>sJunk;
	}
	in >> params.rfoth_a;
	while (sJunk!="rfoth_I"){
		in >>sJunk;
	}
	in >> params.rfoth_I;
	while (sJunk!="rfc_alpha"){
		in >>sJunk;
	}
	in >> params.rfc_alpha;
	while (sJunk!="rfc_beta"){
		in >>sJunk;
	}
	in >> params.rfc_beta;
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
// simulates next step
void WITCHCarbon::nextStep(double e){
	mat[t+1] = mat[t] * params.at2at + mup[t] * params.up2at +
		e * 5 * params.CO2toC;
	mup[t+1] = mat[t] * params.at2up + mup[t] * params.up2up +
		mlo[t] + params.lo2up;
	mlo[t+1] = mlo[t] * params.lo2lo + mup[t] * params.up2lo;
	forc[t] = params.rfoth_I + (1 + params.rfoth_a) * 
		(params.rfc_alpha * (log(mat[t]) - log(params.rfc_beta)));
	std::cout << "\t\tWITCH carbon cycle evolves to next step: " << std::endl;
	std::cout <<  "\t\t" << mat[t] << "\t" << mup[t] << "\t" << mlo[t] << "\t" << forc[t] << "\t" << t+1 << std::endl;
	t++;
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
// frees allocated memory
void WITCHCarbon::carbonDelete(){
	delete[] mat;
	delete[] mup;
	delete[] mlo;
	delete[] forc;
	return;
}