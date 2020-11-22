/*
Here we will put authors and license
*/

#include "RICE.h"

int main(int argc, char* argv[])
{
	int nagents = 2;
	int horizon = 2;

	RICE rice(nagents, horizon);
	RICE* riceptr = &rice;

	riceptr->simulate();

	riceptr->RICE_delete();

	return 0;
}