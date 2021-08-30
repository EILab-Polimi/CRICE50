import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import axes3d
import os, matplotlib, subprocess, time
import seaborn as sns
import matplotlib.font_manager as font_manager
import numpy as np
import pandas as pd
import plotly.express as px
import plotly.io as pio
import plotly.graph_objects as go
import geopandas as gpd

sns.set_style('whitegrid')
# sns.set_context('paper')
import matplotlib.font_manager as font_manager
fontpath = '/Users/angelocarlino/Library/Fonts/OpenSans-Regular.ttf'
prop = font_manager.FontProperties(fname=fontpath, size='large')
prop.set_size(12)
matplotlib.rcParams['font.family'] = prop.get_name()
matplotlib.rcParams['font.size'] = prop.get_size()
matplotlib.rcParams['mathtext.fontset'] = 'custom'
matplotlib.rcParams['mathtext.rm'] = prop.get_name()
matplotlib.rcParams['mathtext.sf'] = prop.get_name()
matplotlib.rcParams['mathtext.it'] = prop.get_name()
matplotlib.rcParams['mathtext.bf'] = prop.get_name()
matplotlib.rcParams['mathtext.tt'] = prop.get_name()
matplotlib.rcParams['mathtext.cal'] = prop.get_name()


def simulate_traj(params, path):
	with open(path+'opt/sol_policy.txt','w+') as f:
		f.write((' ').join([str(x) for x in params]))
	command = "cd "+path+" ; "+path+"/RICE50++ "+" < "+path+"opt/sol_policy.txt"
	with open(path+'/output.txt','w') as f:
		subprocess.run(command, shell=True, stdout=f)
	# time.sleep(1)
	file = pd.read_csv(path+'/simulationOutput.txt', sep='\t')
	return file

nseeds = 5
nobjs = 8
path = './'
RNTS = {}
srand = np.random.randint(0,1e6)
class Rnt:
	def __init__(self):
		self.NFE = []
		self.SBX = []
		self.DE = []
		self.PCX = []
		self.SPX = []
		self.UNDX = []
		self.UM = []
		self.IMP = []
		self.RES = []
		self.ARCSIZE = []
		self.OBJS = []
		self.PARAMS = []

# runtime data & plot
# niter = 100
# fig_cal, ax_cal = plt.subplots(1,1)
color = ['C0','C1','C2']
color = ['C0','C1','C2','C3','C4','C5','C6','C7','C8','C9','C10']

# sols = []
# with open('./BorgOutput/optRICE50.reference') as f:
# 	file = f.read()
# if file !='':
# 	for line in file.split("\n")[:-1]:
# 		vec = [float(x) for x in line.split()]
# 		sols.append(vec)

# nseeds = 5
# nobjs = 8
# path = './'
# params = []
# objs = []
# for seed in range(1,nseeds+1):
# 	with open('./BorgOutput/optRICE50_'+str(seed)+'.out') as f:
# 		file = f.read()
# 		for el in file.split("\n")[:-1]:
# 			if el[0] != "#" and [float(x) for x in el.split()[-3:]] in sols:
# 				params.append([float(x) for x in el.split()[:-3]])
# 				objs.append([float(x) for x in el.split()[-3:]])
RNTS = {}
class Rnt:
	def __init__(self):
		self.NFE = []
		self.SBX = []
		self.DE = []
		self.PCX = []
		self.SPX = []
		self.UNDX = []
		self.UM = []
		self.IMP = []
		self.RES = []
		self.ARCSIZE = []
		self.OBJS = []
		self.PARAMS = []

rnts = [Rnt() for el in range(nseeds)]
vec = []
vecp = []
count = 0
sols = []
params = []
for seed in range(1,nseeds+1):
	try:
		with open('./BorgOutput/rntdynamics_'+str(seed)+'.txt') as f:
			file = f.read()
		if file !='':
			for line in file.split("\n"):
				if "NFE" in line:
					rnts[seed-1].NFE.append(int(line.split('=')[-1]))
				if "SBX" in line:
					rnts[seed-1].SBX.append(float(line.split('=')[-1]))
				if "DE" in line:
					rnts[seed-1].DE.append(float(line.split('=')[-1]))
				if "PCX" in line:
					rnts[seed-1].PCX.append(float(line.split('=')[-1]))
				if "SPX" in line:
					rnts[seed-1].SPX.append(float(line.split('=')[-1]))
				if "UNDX" in line:
					rnts[seed-1].UNDX.append(float(line.split('=')[-1]))
				if "UM" in line:
					rnts[seed-1].UM.append(float(line.split('=')[-1]))
				if "Improvements" in line:
					rnts[seed-1].IMP.append(int(line.split('=')[-1]))
				if "Restarts" in line:
					rnts[seed-1].RES.append(int(line.split('=')[-1]))
				if "ArchiveSize" in line:
					rnts[seed-1].ARCSIZE.append(int(line.split('=')[-1]))			
				if "#" in line:
					rnts[seed-1].OBJS.append(vec)
					vec = []
					rnts[seed-1].PARAMS.append(vecp)
					vecp = []
				if "//" not in line and "#" not in line and line not in ['', '\n']:
					vec.append([float(x) for x in line.split(' ')[-nobjs:]])
					vecp.append([float(x) for x in line.split(' ')[:-nobjs]])
			[sols.append(x) for x in rnts[seed-1].OBJS[-1]]
			[params.append(x) for x in rnts[seed-1].PARAMS[-1]]
	except FileNotFoundError:
		print('Runtime file is not present for seed ' + str(seed))
	count += 1
solsnew = []
paramsnew = []
for el,elp in zip(sols,params):
	flag = 0
	for el2 in sols:
		if el[0] > el2[0]:
			# flag = 1
			if el[1] >= el2[1]:
				if el[2] >= el2[2]:
					if el[3] >= el2[3]:
						if el[4] >= el2[4]:
							if el[5] >= el2[5]:
								# flag=1
								if el[6] >= el2[6]:
									if el[7] >= el2[7]:
										flag = 1
		# if el[1] > 1.2*1e-6:
		# 	flag = 1
		# if el[3] > 150:
		# 	flag = 1
		# if el[7] > 30:
		# 	flag = 1
	if flag==0:
		solsnew.append(el)
		paramsnew.append(elp)
objs = solsnew
params = paramsnew


# with open('./BorgOutput/optRICE50.reference') as f:
# 	file = f.read()
# if file !='':
# 	for line in file.split("\n")[:-1]:
# 		vec = [float(x) for x in line.split()]
# 		sols.append(vec)

# path = './'
# params = []
# objs = []
# for seed in range(1,nseeds+1):
# 	with open('./BorgOutput/optRICE50_'+str(seed)+'.out') as f:
# 		file = f.read()
# 		for el in file.split("\n")[:-1]:
# 			if el[0] != "#" and [float(x) for x in el.split()[-nobjs:]] in sols:
# 				params.append([float(x) for x in el.split()[:-nobjs]])
# 				objs.append([float(x) for x in el.split()[-nobjs:]])

columns = ['SSP','Damages','AdaptEff','Welfare','Y2C','Gini']
welf, gini = [], []

compare = [np.argmin(np.array(objs).T[1])]
# compare = [10,11,12]
print(compare)
for selection in compare:
	# selection = np.argmin(np.array(objs).T[4])
	print(selection)
	params_sim = [params[selection]]
	objs_sim = [objs[selection]]
	for sol,obj in zip(params_sim, objs_sim):
		output = simulate_traj(sol, os.getcwd()+"/../")
		omega = output[[x for x in output.columns if 'OMEGA'==x[:5] or x=='YEAR']]
		komega = output[[x for x in output.columns if 'KOMEGA'==x[:6] or x=='YEAR']]
		tfp = output[[x for x in output.columns if 'TFP'==x[:3] or x=='YEAR']]
		miu = output[[x for x in output.columns if 'MIU'==x[:3] or x=='YEAR']]
		fad = output[[x for x in output.columns if 'FAD'==x[:3] or x=='YEAR']]
		iac = output[[x for x in output.columns if 'IAC'==x[:3] or x=='YEAR']]
		gac = output[[x for x in output.columns if 'GAC'==x[:3] or x=='YEAR']]
		ia = output[[x for x in output.columns if ('IA'==x[:2] and "IAC"!=x[:3]) or x=='YEAR']]
		damfrac = output[[x for x in output.columns if 'DAMFRAC'==x[:7] or x=='YEAR']]
		cemutotper = output[[x for x in output.columns if 'CEMUTOTPER'==x[:10] and len(x) > 10 or x=='YEAR']]
		adapt = output[[x for x in output.columns if 'ADAPT'==x[:5] or x=='YEAR']]
		rd = output[[x for x in output.columns if 'RD'==x[:2] or x=='YEAR']]
		ygross = output[[x for x in output.columns if 'YGROSS'==x[:6] or x=='YEAR']]
		gdp = output[[x for x in output.columns if ('Y'==x[:1] and x[1:4].islower()) or x=='YEAR']]
		pop = output[[x for x in output.columns if ('POP'==x[:3]) or x=='YEAR']]
		gdppc = gdp.copy()
		for el in gdp.columns[1:]:
			gdppc[el] = gdp[el] / pop['POP'+el[1:]]
		for el in adapt.columns[1:]:
			adapt.loc[:,adapt.columns==el] = 1 / ( 1 + adapt[el].values) * damfrac['DAMFRAC'+el[5:]].values
		totalad = fad.copy()
		for el in fad.columns[1:]:
			totalad[el] = fad[el] + ia['IA'+el[3:]] + iac['IAC'+el[3:]]
		omega_scaled = omega.copy()
		for el in omega_scaled.columns[1:]:
			omega_scaled[el] = omega_scaled[el] / (pop['POP'+el[5:]] * tfp['TFP'+el[5:]])
		for el in omega.columns[1:]:
			omega.loc[:,el] = 1 / ( 1 + omega[el].values )
			# omega.loc[:,el] = np.log(1 + omega[el].values)
		for el in rd.columns[1:]:
			rd.loc[:,el] = rd[el].values / ygross['YGROSS'+el[2:]].values
		# ia = output[(x[:2] == 'IA') & (x not in output.columns.str.contains('IAC', case=True)) for x in output.columns]

	var = omega
	vmin = var[var.columns[1:]].min().min()
	vmax = var[var.columns[1:]].max().max()
	# vmin = 0.0
	# vmax = 1.5
	shape = gpd.read_file('./shape_outfile/geo_ene57/geo_ene57.shp')
	years = [y for y in range(2020,2300,10)]
	for y in years:
		vary = var.loc[var['YEAR']==y]
		varc = [x[3:] for x in var.columns[1:]]
		vary = vary.values[0][1:]
		shape['VAR'] = vary
		shape.plot(column='VAR', legend=True, legend_kwds={'format': '%.10f'}, vmin=vmin, vmax=vmax, figsize=(15,5), linewidth=0)
		plt.xticks([])
		plt.yticks([])
		plt.title(str(y))
plt.show()


