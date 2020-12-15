#include "Carbon.h"

#include <iostream>

using namespace std;
// constructor
Carbon::Carbon(){

}
// allocates the carbon component
void Carbon::allocate(int hrzn){
	mat = new double[hrzn + 1];
	mup = new double[hrzn + 1];
	mlo = new double[hrzn + 1];
	forc = new double[hrzn + 1];
	t = 0;
	return;
}
// simulates next step
void Carbon::nextStep(){
	mat[t+1] = mat[t] + 20.0;
	forc[t] = 3.0;
	cout << "Here the carbon cycle evolves to next step: " << t+1 << endl;
	t++;
	return;
}
// frees allocated memory
void Carbon::carbon_delete(){
	delete[] mat;
	delete[] mup;
	delete[] mlo;
	delete[] forc;
	return;
}