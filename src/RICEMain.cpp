/*
Here we will put authors and license
*/
#include "RICE.h"
#include<iostream>
#include<time.h>

int main(int argc, char* argv[])
{	
	// here we should be reading the input file
	// and fix the settings for the simulations 
	// to be run
	clock_t start, end;
	start = clock();
	int carbontype = 1;
	if (argc > 1){
		carbontype = atoi(argv[1]);		
	}
	int horizon = 57;

	// do we want to consider different delta t ?
	// int delta_t = 1;

	// here we create the RICE instance
	// and the associated pointer
    std::cout << "Loading input data: " << std::endl;
	RICE rice(horizon, carbontype);
    std::cout << "total time elapsed: " << ((clock() - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
    std::cout << "Simulating: " << std::endl;
	RICE* riceptr = &rice;

	// simulate RICE
	riceptr->simulate();
    std::cout << "total time elapsed: " << ((clock() - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
    std::cout << "Writing output data: " << std::endl;
	riceptr->writeSimulation();
    std::cout << "total time elapsed: " << ((clock() - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
    std::cout << "Freeing memory: " << std::endl;
	// delete memory used by RICE 
	riceptr->RICE_delete();
	// end
    end = clock();
    std::cout << "total time elapsed: " << ((end - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;

	return 0;
}