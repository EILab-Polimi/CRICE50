#include "Carbon.h"

#include <iostream>

using namespace std;

Carbon::Carbon(){

}

void Carbon::allocate(int hrzn){
	mat = new double[hrzn];
	mup = new double[hrzn];
	mlo = new double[hrzn];
	forc = new double[hrzn];
	t = 0;
	return;
}

void Carbon::nextStep(){
	mat[t+1] = mat[t] + 20.0;
	cout << "Here the carbon cycle evolves to next step: " << t+1 << endl;
	t++;
	return;
}

void Carbon::carbon_delete(){
	delete[] mat;
	delete[] mup;
	delete[] mlo;
	delete[] forc;
	return;
}