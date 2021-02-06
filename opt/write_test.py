import numpy as np
with open('./test_policy.txt', 'w+') as f:
	# for el in range(57*58):
	# 	vars = np.random.rand(2)
	# 	# print(vars)
	# 	f.write(str(vars[0]*1.2)+"\t"+str(vars[1])+"\t")
	for el in range(74):
		# vars = np.random.rand(2)
		# print(vars)
		f.write(str(np.random.rand(1)[0])+"\t")

