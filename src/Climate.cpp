#include "Climate.h"

#include <iostream>

using namespace std;
// constructor
Climate::Climate(){

}
// allocates memory for the climate component
void Climate::allocate(int hrzn){
	tatm = new double[hrzn];
	tocean = new double[hrzn];
	t = 0;
	return;
}
// simulates one time step
void Climate::nextStep(){
	tatm[t+1] = tatm[t] + 0.15;
	cout << "Here the climate evolves to next step: " << t+1 << endl;
	cout << "E.g.: temperature at " << t << " is : " << tatm[t] << endl;	
	cout << " and temperature at " << t+1 << " is : " << tatm[t+1] << endl;	
	t++;
	return;
}
// frees allocated memory
void Climate::climate_delete(){
	delete[] tatm;
	delete[] tocean;
	return;
}