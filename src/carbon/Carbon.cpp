#include "Carbon.h"

#include <iostream>

using namespace std;
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
void DICECarbon::allocate(int hrzn){
	mat = new double[hrzn + 1];
	mup = new double[hrzn + 1];
	mlo = new double[hrzn + 1];
	forc = new double[hrzn + 1];
	forcoth = new double[hrzn + 1];
	readParams();
	t = 0;
	return;
}
// read parameters from text file
// and stores them in the params struct
// and setting initial conditions
void DICECarbon::readParams(){
	fstream in;
	string sJunk = "";
	in.open("./settings/DICECarbon_params.txt", ios_base::in);
	if (!in){
		cout << "The general settings file specified could not be found!" << endl;
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
}
// simulates next step
void DICECarbon::nextStep(double e){
	forcoth[t] = params.fex0 + (params.fex1 - params.fex0) * min(t/18,1);
	mat[t+1] = mat[t] * params.b11 + mup[t] * params.b21 + e * (5/3.666);
	mup[t+1] = mat[t] * params.b12 + mup[t] * params.b22 + mlo[t] * params.b32;
	mlo[t+1] = mup[t] * params.b23 + mlo[t] * params.b33;
	forc[t] = params.fco22x * ((log((mat[t]/588.000))/log(2))) + forcoth[t];
	cout << "\t\tDICE carbon cycle evolves to next step: " << endl;
	cout <<  "\t\t" << mat[t] << "\t" << mup[t] << "\t" << mlo[t] << "\t" << t+1 << endl;
	t++;
	return;
}
// frees allocated memory
void DICECarbon::carbon_delete(){
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
void WITCHCarbon::allocate(int hrzn){
	mat = new double[hrzn + 1];
	mup = new double[hrzn + 1];
	mlo = new double[hrzn + 1];
	forc = new double[hrzn + 1];
	t = 0;
	return;
}
// simulates next step
void WITCHCarbon::nextStep(double e){
	mat[t+1] = mat[t] + 20.0;
	forc[t] = 3.0;
	cout << "Here the WITCH carbon cycle evolves to next step: " << t+1 << endl;
	t++;
	return;
}
// frees allocated memory
void WITCHCarbon::carbon_delete(){
	delete[] mat;
	delete[] mup;
	delete[] mlo;
	delete[] forc;
	return;
}