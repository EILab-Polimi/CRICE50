#include "Carbon.h"

#include <iostream>

using namespace std;
// constructor
Carbon::Carbon(){

}
// allocates the carbon component
void Carbon::allocate(int hrzn){
	mat = new double[hrzn];
	mup = new double[hrzn];
	mlo = new double[hrzn];
	forc = new double[hrzn];
	t = 0;
	return;
}
// simulates next step
void Carbon::nextStep(){
	mat[t+1] = mat[t] + 20.0;
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