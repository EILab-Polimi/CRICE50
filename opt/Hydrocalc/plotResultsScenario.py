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


def simulate(params, path):
	with open(path+'opt/sol_policy.txt','w+') as f:
		f.write((' ').join([str(x) for x in params]))
	command = "cd "+path+" ; "+path+"/RICE50++ "+" < "+path+"opt/sol_policy.txt"
	with open(path+'/output.txt','w') as f:
		subprocess.run(command, shell=True, stdout=f)
	# time.sleep(1)
	with open(path+'/output.txt') as f:
		file = f.read()		
	return file

nseeds = 5
nobjs = 3
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
fig_cal, ax_cal = plt.subplots(1,1)
color = ['C0','C1','C2']
color = ['C0','C1','C2','C3','C4','C5','C6','C7','C8','C9','C10']

sols = []
with open('./BorgOutput/optRICE50.reference') as f:
	file = f.read()
if file !='':
	for line in file.split("\n")[:-1]:
		vec = [float(x) for x in line.split()]
		sols.append(vec)

nseeds = 5
nobjs = 3
path = './'
params = []
objs = []
for seed in range(1,nseeds+1):
	with open('./BorgOutput/optRICE50_'+str(seed)+'.out') as f:
		file = f.read()
		for el in file.split("\n")[:-1]:
			if el[0] != "#" and [float(x) for x in el.split()[-3:]] in sols:
				params.append([float(x) for x in el.split()[:-3]])
				objs.append([float(x) for x in el.split()[-3:]])

columns = ['SSP','Damages','AdaptEff','Welfare','Y1.5C','Gini']
welf, gini = [], []
newobjs = []
for sol,obj in zip(params, objs):
	output = simulate(sol, '/Users/angelocarlino/models/RICE50++/')
	sims = [[float(x) for x in el.split("\t")] for el in output.split("\n")[:-3]]
	print(output.split("\n")[-3].split())
	newobjs.append([float(x) for x in output.split("\n")[-3].split()])
	output = pd.DataFrame(sims, columns=columns)
	welf.append(output.loc[np.isclose(output['Welfare'] , obj[0], atol=1e-10)][columns[:3]].values.tolist()[0])
	gini.append(output.loc[np.isclose(output['Gini'] , obj[2], atol=1e-5)][columns[:3]].values.tolist()[0])

vec = []
for el in range(len(params)):
	line = [x for x in newobjs[el]]
	[line.append(x) for x in welf[el]]
	[line.append(x) for x in gini[el]]
	vec.append(line)
vec = pd.DataFrame(vec, columns=['Welfare','Years above 1.5°C','Gini','SSP-Welfare', 'Damages-Welfare', 'AdaptEff-Welfare', 'SSP-Gini', 'Damages-Gini', 'AdaptEff-Gini'])
ssps = ["SSP1","SSP2","SSP3","SSP4","SSP5"]
damages = ["BURKESR","BURKELR","BURKESR_DIFF","BURKELR_DIFF","DJO","KAHN"]
adapteff = ['0.0','0.2','0.4','0.6','0.8','1.0']
# vec['SSP-Welfare'] = [ssps[int(x)-1] for x in vec['SSP-Welfare']]
# vec['SSP-Gini'] = [ssps[int(x)-1] for x in vec['SSP-Gini']]
# vec['Damages-Welfare'] = [damages[int(x)-1] for x in vec['Damages-Welfare']]
# vec['Damages-Gini'] = [damages[int(x)-1] for x in vec['Damages-Gini']]
# vec['AdaptEff-Welfare'] = [adapteff[int(x)-1] for x in vec['AdaptEff-Welfare']]
# vec['AdaptEff-Gini'] = [adapteff[int(x)] for x in vec['AdaptEff-Gini']]
print(vec)
fig = go.Figure(data = 
	go.Parcoords(
		line = dict(color = vec['Gini'],
			colorscale = 'Tealrose',
			showscale = True),
		dimensions = list([
			dict(range = [0.0, 1.0],
				label = 'Welfare', values = (vec['Welfare'] - vec['Welfare'].min()) /\
				(vec['Welfare'].max() - vec['Welfare'].min()) ),
			dict(range = [vec['Years above 1.5°C'].min(), vec['Years above 1.5°C'].max()],
				label = 'Years above 1.5°C', values = vec['Years above 1.5°C']),
			dict(range = [vec['Gini'].min(), vec['Gini'].max()],
				label = 'Gini', values = vec['Gini']),
			dict(range = [1, 5],
				tickvals = [x for x in range(1,6)],
				ticktext = ssps,
				label = 'SSP-Welfare', values = vec['SSP-Welfare']),
			dict(range = [1, 6],
				tickvals = [x for x in range(1,7)],
				ticktext = damages,
				label = 'Damages-Welfare', values = vec['Damages-Welfare']),
			dict(range = [0, 5],
				tickvals = [x for x in range(6)],
				ticktext = adapteff,
				label = 'AdaptEff-Welfare', values = vec['AdaptEff-Welfare']),
			dict(range = [1, 5],
				tickvals = [x for x in range(1,6)],
				ticktext = ssps,
				label = 'SSP-Gini', values = vec['SSP-Gini']),
			dict(range = [1, 6],
				tickvals = [x for x in range(1,7)],
				ticktext = damages,
				label = 'Damages-Gini', values = vec['Damages-Gini']),
			dict(range = [0, 5],
				tickvals = [x for x in range(6)],
				ticktext = adapteff,
				label = 'AdaptEff-Gini', values = vec['AdaptEff-Gini']),
			])
		))
# fig = px.parallel_coordinates(vec, color='Gini',
#     color_continuous_scale=px.colors.diverging.Tealrose, width=1200, height=800)
# ,
    # color_continuous_midpoint=2)
# Hide the color scale that is useless in this case
# fig.update_layout(coloraxis_showscale=False)
pio.write_image(fig, './parallel_scenarios.pdf', width=1200, height=500, scale=2)

# Show the plot
fig.show()
# print(sols) 
# f4_ax1.set_title('Convergence')
# f4_ax5.set_title('Pareto Front')
# fig4.legend(loc='lower right', ncol=nseeds)
# fig4.tight_layout()


# plt.show()
