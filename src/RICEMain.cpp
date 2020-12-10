/*
Here we will put authors and license
*/

#include "RICE.h"

int main(int argc, char* argv[])
{	
	// here we should be reading the input file
	// and fix the settings for the simulations 
	// to be run
	int nagents = 2;
	int horizon = 2;
	int delta_t = 1;

	// here we create the RICE instance
	// and the associated pointer
	RICE rice(nagents, horizon);
	RICE* riceptr = &rice;

	// simulate RICE
	riceptr->simulate();

	// delete memory used by RICE 
	riceptr->RICE_delete();

	// end
	return 0;
}