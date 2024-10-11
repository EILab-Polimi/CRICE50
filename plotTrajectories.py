import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import matplotlib

matplotlib.rc('font', family='Helvetica')

gamma = 0.0
data = pd.read_csv("./SimOutput_GCF_g"+str(gamma)+"/Sol_SSP2_DAMAGE1_ADAPTEFF1_ECS3.0_TCR1.8.txt", sep='\t')

#select field to plot
field = ['TATM'] 

for el in field:
	df2 = pd.read_csv("./SimOutput_noGCF_g"+str(gamma)+"/Sol_SSP2_DAMAGE1_ADAPTEFF1_ECS3.0_TCR1.8.txt", sep='\t')
	plt.plot(data['YEAR'], data[el], label=el)
	plt.plot(df2['YEAR'], df2[el], label=el+' noGCF')
	plt.plot(data['YEAR'], data['E'], label='E')
	plt.plot(df2['YEAR'], df2['E'], label='E - noGCF')
	del df2
plt.legend()

plt.figure()
ks = [el for el in data.columns if 'K' in el and "OMEGA" not in el]
regions = [el[1:] for el in ks]
tfps = [el for el in data.columns if 'TFP' in el]
mius = [el for el in data.columns if 'MIU' in el]
gdps = [el for el in data.columns if 'Y' in el and "NET" not in el and "GROSS" not in el and "YEAR" not in el]
cs = [el for el in data.columns if el[0]=='C' and not 'CPRICE' in el and not 'CPC' in el]
# print(gdps)
tatms = [el for el in data.columns if "LOCAL" in el]
pops = [el for el in data.columns if 'POP' in el]
gcfs = [el for el in data.columns if 'GCFFLUX' in el]
GCFB = data[gcfs][data[gcfs]>0].sum(axis=1)
plt.plot(data['YEAR'],GCFB, label='Total transfers')
plt.plot(data['YEAR'],data['GCFFLUXchn'], label='China')
plt.plot(data['YEAR'],data['DAMAGESusa'] - data['RDusa'], label='Removed damages')
plt.plot(data['YEAR'],data['GCFFLUXrus'], label='Russia')
plt.plot(data['YEAR'],data['GCFFLUXusa'], label='USA')
plt.plot(data['YEAR'],data['GCFFLUXrfa'], label='Germany')
plt.plot(data['YEAR'],data['GCFFLUXrsas'], label='RSAS')
plt.plot(data['YEAR'],data['GCFFLUXnde'], label='India')
plt.plot(data['YEAR'],data['GCFFLUXrsaf'], label='RSAF')
plt.legend()
plt.ylabel('Trillion USD')
data_ = data[gcfs].copy().transpose()
gdps_ = data[gdps].copy().transpose()
pops_ = data[pops].copy().transpose()
for el in data_.columns:
	if int(el)*5+2015 < 2105:
		x = data_[el].copy()
		y = gdps_[el].copy()

		x = x.sort_values()
		y = y.sort_values()
		
for reg in range(len(regions)):
	data['effK'+regions[reg]] = data[gdps[reg]] / data[pops[reg]] 
effks = [el for el in data.columns if 'effK' in el]

print('Total transfers: [trillion USD]', data[gcfs][data[gcfs]>0].sum(axis=1).sum()*5)

fig, ax = plt.subplots(1,2, figsize=(12,6))

horizon = 17

abatecost = data.iloc[[x for x in range(horizon)]][[x for x in data.columns if 'ABATECOST' in x]].sum().sum()*5
adaptcost = data.iloc[[x for x in range(horizon)]][[x for x in data.columns if 'ADCOST' in x]].sum().sum()*5
residualdam = data.iloc[[x for x in range(horizon)]][[x for x in data.columns if 'RD' in x]].sum().sum()*5
gcfst = data.iloc[[x for x in range(horizon)]][gcfs][data[gcfs]>0].sum(axis=1).sum()*5
df = [['GCF', 0, 0, 0, 0, gcfst]]
df.append(['GCF', abatecost, adaptcost, residualdam, 0, 0])
dfemi = [['GCF', data.iloc[[x for x in range(horizon)]]['E'].sum()*5]]
dfcemutotper = [['GCF', data.iloc[[x for x in range(horizon)]][cs].sum(axis=1).sum()*5]]

data = pd.read_csv("./SimOutput_noGCF_g"+str(gamma)+"/Sol_SSP2_DAMAGE7_ADAPTEFF1_ECS3.0_TCR1.8.txt", sep='\t')
abatecost = data.iloc[[x for x in range(horizon)]][[x for x in data.columns if 'ABATECOST' in x]].sum().sum()*5
adaptcost = data.iloc[[x for x in range(horizon)]][[x for x in data.columns if 'ADCOST' in x]].sum().sum()*5
residualdam = data.iloc[[x for x in range(horizon)]][[x for x in data.columns if 'RD' in x]].sum().sum()*5
df.append(['noGCF', abatecost, adaptcost, residualdam, 0, 0])
dfemi.append(['noGCF', data.iloc[[x for x in range(horizon)]]['E'].sum()*5])
dfcemutotper.append(['noGCF', data.iloc[[x for x in range(horizon)]][cs].sum(axis=1).sum()*5])

df = pd.DataFrame(df, columns=['Model', 'Abatement costs','Adaptation costs','Residual damages', 'Inaccessible consumption', 'Monetary transfers']).set_index('Model')
dfemi = pd.DataFrame(dfemi, columns=['Model', 'Carbon emissions']).set_index('Model')
dfcemutotper = pd.DataFrame(dfcemutotper, columns=['Model', 'Consumption']).set_index('Model')
df.loc[df.index=='noGCF','Inaccessible consumption'] = dfcemutotper.loc[dfcemutotper.index=='GCF','Consumption'].values[0] - \
	dfcemutotper.loc[dfcemutotper.index=='noGCF','Consumption'].values[0]
df.plot.bar(stacked=True, ax = ax[0])
dfemi.plot.bar(ax=ax[1])

plt.suptitle('Gamma = '+ str(gamma)+' - Cumulative values until 2100')
ax[0].set_ylabel('Trillion USD')
ax[1].set_ylabel('GtCO2')
plt.subplots_adjust(bottom=.2, left=0.1, right=0.9, top=0.9)
data[mius].plot()

plt.show()