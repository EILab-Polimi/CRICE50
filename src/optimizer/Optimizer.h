#ifndef OPTIMIZER__H
#define OPTIMIZER__H

#include "../economy/Econ.h"
#include "../economy/EconAgent.h"
#include "../RICE.h"
#include <torch/torch.h>

class Optimizer{
public:
	Optimizer(RICE* ricep);
	~Optimizer();
	RICE* riceptr;
	void sayhello();
	void run();
};

#endif