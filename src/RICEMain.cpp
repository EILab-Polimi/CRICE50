/*        _            _________         
    _____(_)_______  / ____/ __ \   __     __         
   / ___/ / ___/ _ \/___ \/ / / / _/ /_  _/ /_         
  / /  / / /__/  __/___/ / /_/ //_ x__//_ x__/         
 /_/  /_/\___/\___/_____/\____/  /_/    /_/         

TODO: Here we will put authors and license
*/
#include "RICE.h"
#include "./moeaframework/moeaframework.h"
#include <iostream>
#include <time.h>
#include <string>
#include <math.h>
#include <fstream>
#include <dlib/optimization.h>
#include <dlib/global_optimization.h>

RICE rice;

RICE* riceptr = &rice;

// using namespace dlib;

typedef dlib::matrix<double,0,1> column_vector;

// double flag = 0.0;
double dlibopt_miu(const column_vector& m){
	double vars[58*57*2];
	for (int decs=0; decs < 58*57*2 ; decs = decs + 2){
		*(vars + decs) = m(decs/2);
		*(vars + decs + 1) = 0.2;
	}
	riceptr->setVariables(vars);
	riceptr->simulate();
	double obj =  riceptr->econ->utility * pow(10,10);	
	return obj;
}

double dlibopt(const column_vector& m){
	double vars[58*57*2];
	for (int decs=0; decs < 58*57*2 ; decs++){
		*(vars + decs) = m(decs);
	}
	riceptr->setVariables(vars);
	riceptr->simulate();
	double obj =  riceptr->econ->utility * pow(10,10);	
	return obj;
}

int main(int argc, char* argv[])
{	
	clock_t start, end;
	start = clock();
	// ==== MODEL SETTINGS ==========
	// here we should be reading the input file
	// and fix the settings for the simulations 
	// to be run

	// do we want to consider different delta t ?
	// int delta_t = 1;

	// ==== MODEL BUILD ==========	
	// here we create the RICE instance
	// and the associated pointer

    // std::cout << "Loading input data: " << std::endl;
    // std::cout << "total time elapsed: " << ((clock() - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
 //    std::cout << "Simulating: " << std::endl;

	// ==== SIMULATION EXECUTION ==========
	// if (argc <= 1){
	// 	riceptr->simulate();				
	// 	std::cout << riceptr->econ->utility << std::endl;	
	// }
	// else{
	// 	MOEA_Init(nobjs, 0);
	// 	while (MOEA_Next_solution() == MOEA_SUCCESS) {
	// 		MOEA_Read_doubles(nvars, vars);
	// 		riceptr->setVariables(vars);
	// 		riceptr->simulate();
	// 		objs[0] = riceptr->econ->utility * pow(10,10);
	// 		MOEA_Write(objs, NULL);
	// 	}
	// }
	int nvars = 58*57;
	// int nvars = 58 * 57 * 2 ;
	double vars[nvars*2];
	column_vector starting_point;
	starting_point.set_size(nvars);
	column_vector bound1, bound2;
	bound1.set_size(nvars);
	bound2.set_size(nvars);
	//optimize MIU
	for (int ag=0; ag < 57 ; ag++){
		for (int tidx=0; tidx < 58; tidx++){
			starting_point(ag*57+tidx) = std::min(1.0, 0.05 * tidx);
			bound1(ag*57+tidx) = 0.0;
			bound2(ag*57+tidx) = 1.2;
		}
	}
	// //optimize MIU & S
	// for (int ag=0; ag < 57 ; ag++){
	// 	for (int tidx=0; tidx < 58*2; tidx++){
	// 		if (tidx%2 == 0){
	// 			starting_point(ag*57*2+tidx) = std::min(1.0, 0.05 * tidx / 2);
	// 			bound1(ag*58*2+tidx) = 0.0;
	// 			bound2(ag*58*2+tidx) = 1.2;
	// 		}
	// 		else{
	// 			starting_point(ag*58*2+tidx) = 0.2;
	// 			bound1(ag*58*2+tidx) = 0.1;
	// 			bound2(ag*58*2+tidx) = 0.7;				
	// 		}
	// 	}
	// }
	std::cout << "optimization starting: " << std::endl;
    // find_max_using_approximate_derivatives(lbfgs_search_strategy(10*57*57),
    //                                        objective_delta_stop_strategy(1).be_verbose(),
    //                                        dlibopt, starting_point, 10000);
    dlib::find_max_box_constrained(dlib::lbfgs_search_strategy(10*58*57),
                                           dlib::objective_delta_stop_strategy(1e-5).be_verbose(),
                                           dlibopt_miu, dlib::derivative(dlibopt_miu, 5e-2), starting_point, bound1, bound2);
    // dlib::find_max_box_constrained(dlib::lbfgs_search_strategy(10*58*57*2),
    //                                        dlib::objective_delta_stop_strategy(1).be_verbose(),
    //                                        dlibopt, dlib::derivative(dlibopt), starting_point, bound1, bound2);
    std::ofstream solution;
    solution.open("./solution.txt");

	// for (int decs=0; decs < 58*57*2 ; decs++){
	// 	solution << starting_point(decs) << std::endl;
	// }
	for (int decs=0; decs < 58*57 ; decs++){
		solution << starting_point(decs) << std::endl;
	}
	solution.close();

    std::cout << "total time elapsed: " << ((clock() - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
    std::cout << "Writing output data: " << std::endl;

	// ==== POST PROCESSING ==========

	// export simulation results in output file

	for (int decs=0; decs < 58*57*2 ; decs = decs + 2){
		*(vars + decs) = starting_point(decs/2 );
		*(vars + decs + 1) = 0.2;
	}
	riceptr->setVariables(vars);
	riceptr->simulate();


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