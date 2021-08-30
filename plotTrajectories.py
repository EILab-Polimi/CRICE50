import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

data = pd.read_csv("./simulationOutput.txt", sep='\t')
pd.set_option('precision',10)
#select field to plot
# field = ['E','E']
field = ['TATM','E'] #

print(data[field].describe)
for el in field:
	plt.plot(data['YEAR'], data[el], label=el)
plt.legend()

ks = [el for el in data.columns if 'K' in el and "OMEGA" not in el]
regions = [el[1:] for el in ks]
tfps = [el for el in data.columns if 'TFP' in el]
gdps = [el for el in data.columns if 'Y' in el and "NET" not in el and "GROSS" not in el and "YEAR" not in el]
# print(gdps)
tatms = [el for el in data.columns if "LOCAL" in el]
pops = [el for el in data.columns if 'POP' in el]
for reg in range(len(regions)):
# 	data['effK'+regions[reg]] = data[ks[reg]] / (data[tfps[reg]] * data[pops[reg]])
# 	# data['effK'+regions[reg]] = data[ks[reg]] 
# 	# data['effK'+regions[reg]] = data[gdps[reg]] / data[pops[reg]] / data[tfps[reg]]
	data['effK'+regions[reg]] = data[gdps[reg]] / data[pops[reg]] 
effks = [el for el in data.columns if 'effK' in el]
# data[pops].plot()
# data[tfps].plot()
# data[gdps].plot()
# data[effks].plot()
# data[tatms].plot()
# print(data[effks].min().min(), data[effks].max().max())

print('Total emissions: [GtCO2]'data.iloc[[x for x in range(6)]]['E'].sum()*5)

plt.show()