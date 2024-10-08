import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import matplotlib

matplotlib.rc('font', family='Helvetica')

# plt.figure(figsize=(6,4))
# x = [x_ for x_ in np.linspace(0,5,100)]
# damages = [x_**2 for x_ in x]
# mitcost = [20*np.exp(-x_) for x_ in x]
# totcost = [x+y for x,y in zip(damages, mitcost)]

# plt.plot(x, damages, 'r')
# plt.plot(x, mitcost, 'g')
# plt.plot(x, totcost, 'k', lw=2.5 )
# plt.annotate('Impacts of climate change', xy=(2.75,6), color='r', ha='left', va='center')
# plt.scatter(x[totcost.index(min(totcost))], min(totcost), marker='*', edgecolor='k', color='gold', s=200, zorder=20)
# plt.annotate('Mitigation costs', xy=(3,1.5), color='g', ha='left', va='center')
# plt.annotate('Total costs', xy=(2.5,10), color='k', ha='center', va='center')
# plt.annotate('Economically optimal', xy=(x[totcost.index(min(totcost))], min(totcost)+1.5), color='darkorange', ha='center', va='center')
# plt.ylabel('Costs [trillion $]')
# plt.xlabel('Temperature increase [°C]')
# plt.xlim(0,4.3)
# plt.ylim(0,15)
# plt.savefig('/Users/angelocarlino/Desktop/IAMbasics.png', dpi=200)
# plt.show()
# exit()

# data = pd.read_csv("./valOutput/SEED3_SOL3_SSP2_DAMAGE8_ADAPTEFF1_ECS3.0_TCR1.8.txt", sep='\t')
gamma = 0.0
data = pd.read_csv("./SimOutput_GCF_g"+str(gamma)+"/Sol_SSP2_DAMAGE1_ADAPTEFF1_ECS3.0_TCR1.8.txt", sep='\t')
# pd.set_option('precision',10)
#select field to plot
field = ['TATM'] #

print(data[field].describe)
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
print(GCFB)
print(data['GCFFLUXusa'])
plt.plot(data['YEAR'],GCFB, label='Total transfers')
plt.plot(data['YEAR'],data['GCFFLUXchn'], label='China')
# plt.plot(data['YEAR'],data['CEMUTOTPERchn'].values, label='Ychn')
# plt.plot(data['YEAR'],data['CEMUTOTPER'])
plt.plot(data['YEAR'],data['DAMAGESusa'] - data['RDusa'], label='Removed damages')
# plt.plot(data['YEAR'],data['RDrsaf'], label='RDrsaf')
# plt.plot(data['YEAR'],data['Ychn'], label='Ychn')
# plt.plot(data['YEAR'],data['Ynde'], label='Ynde')
# plt.plot(data['YEAR'],data['Yrsaf'], label='Yrsaf')


# plt.plot(data['YEAR'],data['CEMUTOTPERrsaf'])
plt.plot(data['YEAR'],data['GCFFLUXrus'], label='Russia')
plt.plot(data['YEAR'],data['GCFFLUXusa'], label='USA')

plt.plot(data['YEAR'],data['GCFFLUXrfa'], label='Germany')
plt.plot(data['YEAR'],data['GCFFLUXrsas'], label='RSAS')
# plt.plot(data['YEAR'],data['GCFFLUXcan'], label='Canada')
# plt.plot(data['YEAR'],data['GCFFLUXarg'])
# plt.plot(data['YEAR'],data['MIUchn'])
# plt.plot(data['YEAR'],data['GCFFLUXidn'])
# plt.plot(data['YEAR'],data['GCFFLUXrjan57'])
# plt.plot(data['YEAR'],data['GCFFLUXita'])
plt.plot(data['YEAR'],data['GCFFLUXnde'], label='India')
plt.plot(data['YEAR'],data['GCFFLUXrsaf'], label='RSAF')
plt.legend()
# plt.xlim((2015,2105))
plt.ylabel('Trillion USD')
data_ = data[gcfs].copy().transpose()
gdps_ = data[gdps].copy().transpose()
pops_ = data[pops].copy().transpose()
for el in data_.columns:
	if int(el)*5+2015 < 2105:
		print(int(el)*5+2015)
		x = data_[el].copy()
		# print(x)
		y = gdps_[el].copy()

		x = x.sort_values()
		y = y.sort_values()
		print(x)
		print(y)
		# print(x.index)
# for el in data.columns:
# 	if 'GCFFLUX' in el:
# 		print(el, data[el].values)
# exit()
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

# print('Total emissions: [GtCO2]', data.iloc[[x for x in range(6)]]['E'].sum()*5)
# print('Total mitigation costs: [trillion USD]', data.iloc[[x for x in range(57)]][[x for x in data.columns if 'ABATECOST' in x]].sum().sum()*5)
# print('Total adaptation costs: [trillion USD]', data.iloc[[x for x in range(57)]][[x for x in data.columns if 'ADCOST' in x]].sum().sum()*5)
# print('Total adaptation costs: [trillion USD]', data.iloc[[x for x in range(57)]][[x for x in data.columns if 'RD' in x]].sum().sum()*5)
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
# dfcemutotper.plot.bar(ax=ax[2])

plt.suptitle('Gamma = '+ str(gamma)+' - Cumulative values until 2100')
ax[0].set_ylabel('Trillion USD')
ax[1].set_ylabel('GtCO2')
# ax[2].set_ylabel('Trillion USD')
plt.subplots_adjust(bottom=.2, left=0.1, right=0.9, top=0.9)
# plt.figure()
data[mius].plot()

plt.show()