/*
Here we will put authors and license
*/

#include "RICE.h"

int main(int argc, char* argv[])
{	
	// here we should be reading the input file
	// and fix the settings for the simulations 
	// to be run
	int carbontype = 1;
	if (argc > 1){
		carbontype = atoi(argv[1]);		
	}
	int horizon = 10;

	// do we want to consider different delta t ?
	// int delta_t = 1;

	// here we create the RICE instance
	// and the associated pointer
	RICE rice(horizon, carbontype);
	RICE* riceptr = &rice;

	// simulate RICE
	riceptr->simulate();
	riceptr->writeSimulation();
	// delete memory used by RICE 
	riceptr->RICE_delete();

	// end
	return 0;
}