#include "Optimizer.h"
#include "../RICE.h"
#include <torch/torch.h>


Optimizer::Optimizer(RICE* ricep){	
	riceptr = ricep;
}

Optimizer::~Optimizer(){

}

void Optimizer::sayhello(){
	std::cout << "hello" << std::endl;
}

void Optimizer::run(){

    std::ofstream optoutput;
    optoutput.open("./optoutput.txt");
    optoutput << "niter" <<"\t";
    // std::vector<int> aglist = {10, 55};
    std::vector<int> aglist;
    for (int ag = 0; ag < 57; ag++){
        aglist.push_back(ag);
        optoutput << ag << "\t";
    }
    optoutput << std::endl;

	std::vector<Net*> nets(riceptr->econ->agents);
	for (int ag = 0; ag < aglist.size(); ag++){
		nets[aglist[ag]] = riceptr->econ->agents_ptr[aglist[ag]]->getAgentTorchPolicy();
	}
	std::vector<torch::optim::Adam*> optimizers(riceptr->econ->agents);
	for (int ag = 0; ag < aglist.size(); ag++){
		torch::optim::Adam* optimizer = new torch::optim::Adam(nets[aglist[ag]]->parameters(), torch::optim::AdamOptions(0.1));
		optimizers[aglist[ag]] = optimizer;
	}	
    // torch::optim::Adam optimizer(net->parameters(), torch::optim::AdamOptions(1.0));



    for (int niter = 0; niter < 100 ; niter++){


        std::vector<std::vector<torch::Tensor>> rewards(aglist.size());
        std::vector<std::vector<torch::Tensor>> logprobs(aglist.size());
        std::vector<std::vector<torch::Tensor>> returns(aglist.size());
        
        for (int niter2=0; niter2 < 10; niter2++){
            riceptr->simulate();
            for (int ag = 0; ag < riceptr->econ->agents; ag++){
                std::vector<torch::Tensor> rew = riceptr->econ->agents_ptr[aglist[ag]]->getRewards();
                std::vector<torch::Tensor> logs = riceptr->econ->agents_ptr[aglist[ag]]->getLogProb();
                for (int tidx = 0; tidx < riceptr->econ->horizon ; tidx++){
                    rewards[ag].push_back(rew[tidx]);
                    logprobs[ag].push_back(logs[tidx]);
                }
                torch::Tensor ret = torch::zeros({1});
                returns[ag].resize(riceptr->econ->horizon);
                for (int tidx = riceptr->econ->horizon - 1; tidx >= 0; tidx--){
                    ret = rew[tidx] + 1 / pow(1 + (float) riceptr->econ->params.prstp, 5) * ret;
                    returns[ag][tidx] = ret;
                    // returns[ag][tidx] = (ret - (float) riceptr->econ->agents_ptr[ag]->utility);
                }
            }
        }
        // for (int ag = 0; ag < aglist.size(); ag++){   
        // }
        // riceptr->simulate();   
    	for (int ag = 0; ag < aglist.size(); ag++){   
            // for (int niterx = 0; niterx < 1; niterx++){
			// for (const auto& pair : nets[ag]->named_parameters()) {
			// 	std::cout << pair.key() << ": " << pair.value() << std::endl;
			// }		
            optimizers[aglist[ag]]->zero_grad();
			// std::vector<torch::Tensor> rewards = riceptr->econ->agents_ptr[aglist[ag]]->getRewards();
   //          std::vector<torch::Tensor> logprobs = riceptr->econ->agents_ptr[aglist[ag]]->getLogProb();
   //  		std::vector<torch::Tensor> returns;
   //          torch::Tensor ret = torch::zeros({1});
            torch::Tensor loss = torch::zeros({1});
    		for (int tidx=0; tidx < logprobs.size(); tidx++){
    			// ret = rewards[tidx] + 1 / pow(1 + (float) riceptr->econ->params.prstp, 5) * ret;
                // loss = loss + logprobs[tidx] * ret;
                loss = loss + logprobs[ag][tidx] * (returns[ag][tidx]);
                // loss = loss + logprobs[tidx] * riceptr->econ->agents_ptr[ag]->utility;
            }

    		// auto ret = torch::from_blob(returns.data(), {58});
    		// std::cout << ret.grad_fn()->name() << std::endl;

    		// std::cout << ret << std::endl <<riceptr->econ->agents_ptr[ag]->getLogProb() << std::endl;
    		// auto loss = ret * riceptr->econ->agents_ptr[ag]->getLogProb();
    		// std::cout << loss << std::endl;
    		// loss = loss.mean();
    		// loss.backward({}, c10::optional<bool>(true), true);
            loss.backward();
    		optimizers[aglist[ag]]->step();
            // }
    	}
    	
    	// auto options = torch::TensorOptions().requires_grad(true);
    	// std::vector<float> lossf(10);
    	// for (int itsim = 0; itsim < 10; itsim++){
	    // 	// riceptr->simulate();
	    // 	lossf[itsim] = (float) -riceptr->econ->utility ;
	    // 	// auto ut = torch::from_blob(obj.data(), {1}, torch::requires_grad());
	    // 	// auto logprob = riceptr->econ->agents_ptr[ag]->getLogProb();
	    // 	// auto sloss = logprob*ut;
	    // 	// std::cout << ut << std::endl;
	    // 	// torch::cat({loss, ut});
    	// }
    	// auto loss = torch::from_blob(lossf.data(), {10,1});
    	// loss.mean();
    	// loss.backward();

     // 	for (int ag = 0; ag < riceptr->econ->agents; ag++){
    	// 	optimizers[ag]->step();
    	// }
    	// optimizer.step();
		std::cout << "Epoch: " << niter <<  " \t" << riceptr->econ->utility 
            << "\t" << riceptr->econ->agents_ptr[aglist[0]]->utility
            << "\t" << riceptr->econ->agents_ptr[aglist[1]]->utility << std::endl;
			// << " | Loss: " << loss.item<float>() << std::endl;
        optoutput << niter << "\t";
        for (int ag=0; ag < riceptr->econ->agents; ag++){
            optoutput << riceptr->econ->agents_ptr[ag]->utility << "\t";
        }
        optoutput << std::endl;

    }
    riceptr->writeSimulation();
	for (int ag = 0; ag < riceptr->econ->agents; ag++){
		delete optimizers[ag];
	}
    optoutput.close();

	std::cout << "opt\t" << - riceptr->econ->utility << std::endl;

}