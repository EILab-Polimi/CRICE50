import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import axes3d
import os, matplotlib, subprocess, time
import seaborn as sns
import matplotlib.font_manager as font_manager
import numpy as np
import pandas as pd
import plotly.express as px

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

def patch_violinplot():
     from matplotlib.collections import PolyCollection
     ax = plt.gca()
     for art in ax.get_children():
          if isinstance(art, PolyCollection):
              art.set_edgecolor((0, 0, 0, 0))

# def simulate(params, path, run=None, niter=100, srand=5, damages=None):
# 	with open(path+'/general_settings.txt') as f:
# 		file = f.read().split("\n")
# 	newfile = file
# 	for el in enumerate(newfile):
# 		if 'NITER' in el[1]:
# 			newfile[el[0]] = "NITER\t\t"+str(niter)
# 		if 'DAMAGES' in el[1]:
# 			if damages is not None:
# 				newfile[el[0]] = "DAMAGES\t\t"+str(damages)
# 	with open(path+'/general_settings.txt','w') as f:
# 		[f.write(el+"\n") for el in newfile]			
# 	with open(path+'/test_policy.txt','w+') as f:
# 		f.write((' ').join(params))
# 	command = "cd "+path+" ; "+path+"/ADCDICE2016 "+path+"/nominal_params.txt "+ str(srand)+" < "+path+"/test_policy.txt"
# 	with open(path+'/output.txt','w') as f:
# 		subprocess.run(command, shell=True, stdout=f)
# 	# time.sleep(1)
# 	with open(path+'/output.txt') as f:
# 		file = f.read()		
# 	return file

### CREATE DATASETS TO PLOT RUNTIME DYNAMICS, PARETO FRONT
# damages = ['']
# adapt = ['']
# sobj = ['SO_det', 'SO_sto', 'SO_ad', 'SO_ad_sto', 'SO_ad_sto_MO','EMODPS']
# sobj = ['SO_ad_sto_maxmin', 'SO_ad_sto_MO_maxmin','EMODPS_maxmin']
# sobj = ['SO_ad_sto', 'SO_ad_sto_MO','EMODPS']
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
# for eld in damages:
# 	for elso in sobj:
# 		for ela in adapt:
# 			print(eld+elso+ela)
rnts = [Rnt() for el in range(nseeds)]
vec = []
vecp = []
fig4 = plt.figure()
gs = fig4.add_gridspec(4,2)
f4_ax1 = fig4.add_subplot(gs[0,0])
f4_ax2 = fig4.add_subplot(gs[1,0], xticklabels = [], sharex=f4_ax1)
f4_ax3 = fig4.add_subplot(gs[2,0], xticklabels = [], sharex=f4_ax1)
f4_ax4 = fig4.add_subplot(gs[3,0])
f4_ax5 = fig4.add_subplot(gs[:,1])
count = 0
sols = []
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

			f4_ax1.plot(rnts[seed-1].NFE, rnts[seed-1].IMP, label='seed '+str(seed), color=color[seed-1])
			f4_ax1.set_ylabel('Improvements')
			f4_ax2.plot(rnts[seed-1].NFE, rnts[seed-1].ARCSIZE, color=color[seed-1])
			f4_ax2.set_ylabel('Archive Size')
			f4_ax3.plot(rnts[seed-1].NFE, rnts[seed-1].RES, color=color[seed-1])
			f4_ax3.set_ylabel('Restarts')
			f4_ax4.plot(rnts[seed-1].NFE, [rnts[seed-1].IMP[0], *np.diff(rnts[seed-1].IMP)], color=color[seed-1])
			f4_ax4.set_xlabel('NFE')
			f4_ax4.set_ylabel('New Improvements')
			[sols.append(x) for x in rnts[seed-1].OBJS[-1]]
			# f4_ax5.
			# 				cnt = 0
			# 				for nfe, ob in zip(rnts[seed-1].NFE, rnts[seed-1].OBJS):
			# 					if cnt == len(rnts[seed-1].NFE) - 1:
			# 						ob = np.transpose(ob)
			# 						f4_ax5.scatter(ob[0], ob[1], marker='^', color=color[seed-1], edgecolor='k', alpha=0.7)	
			# 						marker = '^'
			# 						edgecolor=None
			# 						if elso=='SO_ad_sto_MO':
			# 							marker = 'o'
			# 							edgecolor = 'k'
			# 						if elso=='SO_ad_sto_MO_maxmin':
			# 							marker = 'o'
			# 							edgecolor = 'k'
			# 						if elso=='SO':
			# 							marker = 'd'
			# 							edgecolor = 'k'
			# 						if elso=='SO_ad_sto':
			# 							marker = '*'
			# 							edgecolor = 'k'
			# 						if elso=='SO_ad_sto_maxmin':
			# 							marker = '*'
			# 							edgecolor = 'k'
			# 						if elso=='SO_ad':
			# 							marker='D'
			# 							edgecolor = 'k'
			# 						if elso=='SO_sto':
			# 							marker='s'
			# 							edgecolor = 'k'
			# 						objs_val = []
			# 						for el in rnts[seed-1].PARAMS[cnt]:
			# 							p = [str(x) for x in el]
			# 							objs_val.append([float(x) for x in simulate(p, os.getcwd()+path[1:]+eld+elso+ela, niter=niter, srand=srand, damages=None).split("\n")[0].split()])
			# 						objs_val = np.transpose(objs_val)
			# 						ax_cal.scatter(y=-objs_val[0], x=objs_val[1], marker=marker, edgecolor=edgecolor, color=color[seed-1], alpha=0.7, label='CDICE'+elso)
			# 					if cnt != len(rnts[seed-1].NFE) - 1:
			# 						ob = np.transpose(ob)
			# 						f4_ax5.scatter(ob[0], ob[1], color=color[seed-1], s=8, alpha=0.4)
			# 					cnt += 1
	except FileNotFoundError:
		print('Runtime file is not present for seed ' + str(seed))
	count += 1
solsnew = []
for el in sols:
	flag = 0
	for el2 in sols:
		if el[0] > el2[0]:
			if el[1] >= el2[1]:
				if el[2] >= el2[2]:
					flag = 1
	if flag==0:
		solsnew.append(el)

sols = pd.DataFrame(sols, columns=['Welfare','Years above 2°C','Gini'])
fig = px.parallel_coordinates(sols, color='Gini',
    color_continuous_scale=px.colors.diverging.Tealrose)
# ,
    # color_continuous_midpoint=2)
# Hide the color scale that is useless in this case
fig.update_layout(coloraxis_showscale=False)

# Show the plot
fig.show()
# print(sols)
f4_ax1.set_title('Convergence')
f4_ax5.set_title('Pareto Front')
fig4.legend(loc='lower right', ncol=nseeds)
fig4.tight_layout()
# RNTS[eld+elso+ela] = rnts

# # # ## simulate solutions
# niter = 200
# color = ['C3','C4','forestgreen']
# label=['Single obj. static opt.', 'Multi-objective static opt.', 'EMODPS']
# for ela in adapt:
# 	fig, ax = plt.subplots(1,1)
# 	fig4, ax4 = plt.subplots(1,3)
# 	fig5, ax5 = plt.subplots(1,5, sharey=True)
# 	fig6, ax6 = plt.subplots(2,2, sharey=False, sharex=True)
# 	fig7, ax7 = plt.subplots(1,1)
# 	violindata = pd.DataFrame(columns=['Type', 'T2100','GDP2100','GDP2050'])
# 	violindata2 = pd.DataFrame(columns=['Type', 'T2100','GDP2100','GDP2050'])
# 	count = 0
# 	for eld in damages:
# 		for elso in sobj:
# 			print(eld+elso+ela)
# 			with open(path+eld+elso+ela+'/BorgOutput/optADCDICE2016.reference') as f:
# 				ref = f.read().split("\n")[:-1]
# 			ref = [[round(float(x),5) for x in el.split()] for el in ref] 
# 			objs = []
# 			params = []
# 			for seed in range(1,nseeds+1):
# 				try:
# 					with open(path+eld+elso+ela+'/BorgOutput/optADCDICE2016_'+str(seed)+'.out') as f:
# 						file = f.read()
# 						if file != '':
# 							for el in file.split("\n")[:-1]:
# 								if elso in ['SO_ad_sto_MO', 'EMODPS']:
# 									if el[0] != '#' and [round(float(x),5) for x in el.split()[-nobjs:]] in ref:
# 										objs.append([float(x) for x in el.split()[-nobjs:]])
# 										params.append([float(x) for x in el.split()[:-nobjs]])
# 								else:
# 									if el[0] != '#' and [round(float(x),5) for x in el.split()[-1:]] in ref:
# 										objs.append([float(el.split()[-1]), 0.305])
# 										params.append([float(x) for x in el.split()[:-1]])										
# 						else:
# 							[objs.append(el) for el in RNTS[eld+elso+ela][seed-1].OBJS[-1]]
# 							[params.append(el) for el in RNTS[eld+elso+ela][seed-1].PARAMS[-1]]
# 				except FileNotFoundError:
# 					print('Output file is not present in '+path+eld+ela+elso)
# 					[objs.append(el) for el in RNTS[eld+elso+ela][seed-1].OBJS[-1]]
# 					[params.append(el) for el in RNTS[eld+elso+ela][seed-1].PARAMS[-1]]
# 			if objs != []:
# 				index = [x for x in range(len(objs))]
# 				for x in enumerate(objs):
# 					for y in enumerate(objs):
# 						if x[1][0] > y[1][0] and x[1][1] > y[1][1] and x[0] not in index:
# 							index.pop(y[0])
# 				index = []
# 				for x in enumerate(objs):
# 					for y in enumerate(objs):
# 						if x[1][0] > y[1][0] and x[1][1] > y[1][1] and x[0] not in index:
# 							index.append(x[0])
# 				temp = []
# 				temp2 = []
# 				for x in range(0, len(objs)):
# 					if x not in index:
# 						temp.append(objs[x])
# 						temp2.append(params[x])
# 				objs = np.asarray(temp)
# 				params = np.asarray(temp2)
# 				# if elso in ['_SO_ad_sto_MO', '_EMODPS']:
# 				# 	tempthres = 0.05
# 				# 	params = params[(np.transpose(objs)[1]>tempthres) & (np.transpose(objs)[1]<tempthres+0.01)]
# 				# 	objs = objs[(np.transpose(objs)[1]>tempthres) & (np.transpose(objs)[1]<tempthres+10.0)]
# 					# wthres = -502200
# 					# params = params[(np.transpose(objs)[0]<wthres) & (np.transpose(objs)[0]>wthres-50)]
# 					# objs = objs[(np.transpose(objs)[0]<wthres) & (np.transpose(objs)[0]>wthres-50)]
# 					# params = params[np.transpose(objs)[0]==min(np.transpose(objs)[0])]
# 					# objs = objs[np.transpose(objs)[0]==min(np.transpose(objs)[0])]
# 					# params = params[np.transpose(objs)[1]==min(np.transpose(objs)[1])]
# 					# objs = objs[np.transpose(objs)[1]==min(np.transpose(objs)[1])]

# 				# tempthres = 0.31
# 				# params = params[(np.transpose(objs)[1]>tempthres) & (np.transpose(objs)[1]<tempthres+0.03)]
# 				# objs = objs[(np.transpose(objs)[1]>tempthres) & (np.transpose(objs)[1]<tempthres+0.03)]
# 				# wthres = -502250
# 				# params = params[(np.transpose(objs)[0]<wthres) & (np.transpose(objs)[0]>wthres-100)]
# 				# objs = objs[(np.transpose(objs)[0]<wthres) & (np.transpose(objs)[0]>wthres-100)]
# 				edgecolor = 'k'
# 				marker='o'
# 				if elso=='EMODPS':
# 					edgecolor='k'
# 					marker='^'
# 				if len(objs)>1:
# 					objs = np.transpose(objs)
# 				objs_val = []
# 				gdp2100 = []
# 				gdp2050 = []
# 				miucost = []
# 				iacost = []
# 				fadcost = []
# 				res_dam = []
# 				total = []
# 				t2100 = []
# 				# count = 0
# 				if params.size!=0:
# 					for el in params:
# 						p = [str(x) for x in el]
# 						objs_val.append([float(x) for x in simulate(p, os.getcwd()+path[1:]+eld+elso+ela, niter=niter, srand=10304092400, damages=4).split("\n")[0].split()])
# 						with open(path+eld+elso+ela+'/GDP2100.txt') as f:
# 							gdp2100.append([float(x) for x in f.read().split("\n")[:-1]])
# 						with open(path+eld+elso+ela+'/GDP2050.txt') as f:
# 							gdp2050.append([float(x) for x in f.read().split("\n")[:-1]])
# 						with open(path+eld+elso+ela+'/T2100.txt') as f:
# 							t2100.append([float(x) for x in f.read().split("\n")[:-1]])
# 						with open (path+eld+elso+ela+'/COSTS.txt') as f:
# 							miucost_= [float(x.split()[0]) for x in f.read().split("\n")[:-1]]
# 							f.seek(0)
# 							iacost_ = [float(x.split()[1]) for x in f.read().split("\n")[:-1]]
# 							f.seek(0)
# 							fadcost_ = [float(x.split()[2]) for x in f.read().split("\n")[:-1]]
# 							f.seek(0)
# 							res_dam_ = [float(x.split()[3]) for x in f.read().split("\n")[:-1]]
# 							f.seek(0)
# 							g_ = [float(x.split()[4]) for x in f.read().split("\n")[:-1]]
# 							discount = np.transpose(np.reshape(np.asarray(g_),(niter*2,-1)))[1:]
# 							miucost_ = np.transpose(np.reshape(np.asarray(miucost_),(niter*2,-1)))[1:] * discount
# 							iacost_ = np.transpose(np.reshape(np.asarray(iacost_),(niter*2,-1)))[1:] * discount
# 							fadcost_ = np.transpose(np.reshape(np.asarray(fadcost_),(niter*2,-1)))[1:] * discount
# 							res_dam_ = np.transpose(np.reshape(np.asarray(res_dam_),(niter*2,-1)))[1:] * discount
# 							# miucost_ = np.transpose(np.reshape(np.asarray(miucost_),(niter*2,-1))) 
# 							# iacost_ = np.transpose(np.reshape(np.asarray(iacost_),(niter*2,-1))) 
# 							# fadcost_ = np.transpose(np.reshape(np.asarray(fadcost_),(niter*2,-1))) 
# 							# res_dam_ = np.transpose(np.reshape(np.asarray(res_dam_),(niter*2,-1))) 
# 							total_ = miucost_ + iacost_ + fadcost_ + res_dam_
# 							# miucost.append(miucost_.flatten().tolist())
# 							# iacost.append(iacost_.flatten().tolist())
# 							# fadcost.append(fadcost_.flatten().tolist())
# 							# res_dam.append(res_dam_.flatten().tolist())
# 							# total.append(total_.flatten().tolist())
# 							miucost.append(np.nansum(miucost_, axis=0))
# 							iacost.append(np.nansum(iacost_, axis=0))
# 							fadcost.append(np.nansum(fadcost_, axis=0))
# 							res_dam.append(np.nansum(res_dam_, axis=0))
# 							total.append(np.nansum(total_, axis=0))
# 							im1 = ax6[0][0].scatter(100*objs_val[-1][1], -objs_val[-1][0], label=elso[:]+'(val)', c=np.nansum(miucost_)/(niter*2), cmap='plasma', norm=matplotlib.colors.Normalize(vmin=2.7,vmax=10.4), marker=marker, edgecolor=edgecolor)
# 							im2 = ax6[1][0].scatter(100*objs_val[-1][1], -objs_val[-1][0], label=elso[:]+'(val)', c=np.nansum(iacost_)/(niter*2), cmap='plasma', norm=matplotlib.colors.Normalize(vmin=4.8,vmax=5.8), marker=marker, edgecolor=edgecolor)
# 							im3 = ax6[1][1].scatter(100*objs_val[-1][1], -objs_val[-1][0], label=elso[:]+'(val)', c=np.nansum(fadcost_)/(niter*2), cmap='plasma', norm=matplotlib.colors.Normalize(vmin=2.1,vmax=2.7), marker=marker, edgecolor=edgecolor)
# 							im4 = ax6[0][1].scatter(100*objs_val[-1][1], -objs_val[-1][0], label=elso[:]+'(val)', c=np.nansum(res_dam_)/(niter*2), cmap='plasma', norm=matplotlib.colors.Normalize(vmin=18.8,vmax=21.8), marker=marker, edgecolor=edgecolor)
# 							# im5 = ax7.scatter(100*objs_val[-1][1], -objs_val[-1][0], label=elso[1:]+'(val)', c=np.mean(miucost_) + np.mean(iacost_) + np.mean(fadcost_) + np.mean(res_dam_), cmap='plasma', norm=matplotlib.colors.Normalize(vmin=2.15,vmax=3.59), marker=marker, edgecolor=edgecolor)
# 							# im5 = ax7.scatter(100*objs_val[-1][1], -objs_val[-1][0], label=elso[1:]+'(val)', c=np.mean(total_), cmap='plasma', norm=matplotlib.colors.Normalize(vmin=2.15,vmax=3.59), marker=marker, edgecolor=edgecolor)
# 							im5 = ax7.scatter(100*objs_val[-1][1], -objs_val[-1][0], label=elso[:]+'(val)', c=np.sum(total_)/(niter*2), cmap='plasma', norm=matplotlib.colors.Normalize(vmin=32.6,vmax=36.4), marker=marker, s=75, edgecolor=edgecolor)
# 							# ##alternative
# 							# im1 = ax6[0][0].scatter(100*objs_val[-1][1], c=-objs_val[-1][0], label=elso[:]+'(val)', y=np.nansum(miucost_)/(niter*2), cmap='plasma', norm=matplotlib.colors.Normalize(vmin=515000,vmax=516000), marker=marker, edgecolor=edgecolor)
# 							# im2 = ax6[1][0].scatter(100*objs_val[-1][1], c=-objs_val[-1][0], label=elso[:]+'(val)', y=np.nansum(iacost_)/(niter*2), cmap='plasma', norm=matplotlib.colors.Normalize(vmin=515000,vmax=516000), marker=marker, edgecolor=edgecolor)
# 							# im3 = ax6[1][1].scatter(100*objs_val[-1][1], c=-objs_val[-1][0], label=elso[:]+'(val)', y=np.nansum(fadcost_)/(niter*2), cmap='plasma', norm=matplotlib.colors.Normalize(vmin=515000,vmax=516000), marker=marker, edgecolor=edgecolor)
# 							# im4 = ax6[0][1].scatter(100*objs_val[-1][1], c=-objs_val[-1][0], label=elso[:]+'(val)', y=np.nansum(res_dam_)/(niter*2), cmap='plasma', norm=matplotlib.colors.Normalize(vmin=515000,vmax=516000), marker=marker, edgecolor=edgecolor)
# 							# # im5 = ax7.scatter(100*objs_val[-1][1], -objs_val[-1][0], label=elso[1:]+'(val)', c=np.mean(miucost_) + np.mean(iacost_) + np.mean(fadcost_) + np.mean(res_dam_), cmap='plasma', norm=matplotlib.colors.Normalize(vmin=2.15,vmax=3.59), marker=marker, edgecolor=edgecolor)
# 							# # im5 = ax7.scatter(100*objs_val[-1][1], -objs_val[-1][0], label=elso[1:]+'(val)', c=np.mean(total_), cmap='plasma', norm=matplotlib.colors.Normalize(vmin=2.15,vmax=3.59), marker=marker, edgecolor=edgecolor)
# 							# im5 = ax7.scatter(100*objs_val[-1][1], c=-objs_val[-1][0], label=elso[:]+'(val)', y=np.sum(total_)/(niter*2), cmap='plasma', norm=matplotlib.colors.Normalize(vmin=515000,vmax=516000), marker=marker, s=75, edgecolor=edgecolor)
# 							print(np.sum(miucost_)/(niter*2), np.sum(iacost_)/(niter*2), np.sum(fadcost_)/(niter*2), np.sum(res_dam_)/(niter*2), np.sum(miucost_)/(niter*2) + np.sum(iacost_)/(niter*2) + np.sum(fadcost_)/(niter*2) + np.sum(res_dam_)/(niter*2))
# 					objs_val = np.transpose(objs_val)
# 					# ax.scatter(objs_val[1], -objs_val[0], color = color[count], label=label[count]+elso+'(val)', marker='^', edgecolor=edgecolor)
# 					ax.scatter(100*objs_val[1], -objs_val[0], label=label[count], marker=marker, edgecolor=edgecolor, color = color[count])

# 					vdata = np.transpose(np.asarray([[eld+elso[1:]+ela for el in range(len(np.asarray(t2100).flatten()))], 
# 						[float(el) for el in np.asarray(t2100).flatten()], [float(el) for el in np.asarray(gdp2100).flatten()],
# 						[float(el) for el in np.asarray(gdp2050).flatten()]]))
# 					vdata = pd.DataFrame(vdata, columns=['Optimization', 'T2100 [°C]','GDP2100 [trillion USD]','GDP2050 [trillion USD]'])
# 					vdata2 = np.transpose(np.asarray([[eld+elso[1:]+ela for el in range(len(np.asarray(miucost).flatten()))], 
# 						[float(el) for el in np.asarray(miucost).flatten()], [float(el) for el in np.asarray(iacost).flatten()],
# 						[float(el) for el in np.asarray(fadcost).flatten()], [float(el) for el in np.asarray(res_dam).flatten()],
# 						[float(el) for el in np.asarray(total).flatten()]]))
# 					vdata2 = pd.DataFrame(vdata2, columns=['Optimization', 'NPV ABATEMENT COSTS [trillion USD]','NPV STOCK ADAPTATION COSTS [trillion USD]','NPV FLOW ADAPTATION COSTS [trillion USD]', 'NPV RESIDUAL DAMAGES [trillion USD]', 'NPV TOTAL COSTS & DAMAGES [trillion USD]'])

# 					violindata = violindata.append(vdata, ignore_index=True)
# 					violindata2 = violindata2.append(vdata2, ignore_index=True)
# 			count += 1
# 	ax.scatter(0,516000,label='Ideal solution (no conflict)', marker='*', color='forestgreen', edgecolor='k', s=150)
# 	ax.legend()
# 	violindata['T2100 [°C]'] = pd.to_numeric(violindata['T2100 [°C]'])
# 	violindata['GDP2100 [trillion USD]'] = pd.to_numeric(violindata['GDP2100 [trillion USD]'])
# 	violindata['GDP2050 [trillion USD]'] = pd.to_numeric(violindata['GDP2050 [trillion USD]'])
# 	sns.violinplot(x="Optimization", y="T2100 [°C]", data=violindata, ax=ax4[0])
# 	sns.violinplot(x="Optimization", y="GDP2050 [trillion USD]", data=violindata, ax=ax4[1])
# 	sns.violinplot(x="Optimization", y="GDP2100 [trillion USD]", data=violindata, ax=ax4[2])
# 	violindata2['NPV ABATEMENT COSTS [trillion USD]'] = pd.to_numeric(violindata2['NPV ABATEMENT COSTS [trillion USD]'], errors='coerce')
# 	violindata2['NPV STOCK ADAPTATION COSTS [trillion USD]'] = pd.to_numeric(violindata2['NPV STOCK ADAPTATION COSTS [trillion USD]'], errors='coerce')
# 	violindata2['NPV FLOW ADAPTATION COSTS [trillion USD]'] = pd.to_numeric(violindata2['NPV FLOW ADAPTATION COSTS [trillion USD]'], errors='coerce')
# 	violindata2['NPV RESIDUAL DAMAGES [trillion USD]'] = pd.to_numeric(violindata2['NPV RESIDUAL DAMAGES [trillion USD]'], errors='coerce')
# 	violindata2['NPV TOTAL COSTS & DAMAGES [trillion USD]'] = pd.to_numeric(violindata2['NPV TOTAL COSTS & DAMAGES [trillion USD]'], errors='coerce')

# 	sns.boxplot(x="Optimization", y="NPV TOTAL COSTS & DAMAGES [trillion USD]", data=violindata2, ax=ax5[0], showfliers=False, palette=['C3','C4','forestgreen'])
# 	sns.boxplot(x="Optimization", y="NPV ABATEMENT COSTS [trillion USD]", data=violindata2, ax=ax5[1], showfliers=False, palette=['C3','C4','forestgreen'])
# 	sns.boxplot(x="Optimization", y="NPV STOCK ADAPTATION COSTS [trillion USD]", data=violindata2, ax=ax5[2], showfliers=False, palette=['C3','C4','forestgreen'])
# 	sns.boxplot(x="Optimization", y="NPV FLOW ADAPTATION COSTS [trillion USD]", data=violindata2, ax=ax5[3], showfliers=False, palette=['C3','C4','forestgreen'])
# 	sns.boxplot(x="Optimization", y="NPV RESIDUAL DAMAGES [trillion USD]", data=violindata2, ax=ax5[4], showfliers=False, palette=['C3','C4','forestgreen'])

# 	# sns.violinplot(x="Optimization", y="NPV ABATEMENT COSTS [trillion USD]", data=violindata2, ax=ax5[0], palette=['C3','C4','forestgreen'], cut=0, inner=None, zorder=-1, linewidth=0.1)
# 	# sns.violinplot(x="Optimization", y="NPV STOCK ADAPTATION COSTS [trillion USD]", data=violindata2, ax=ax5[1], palette=['C3','C4','forestgreen'], cut=0, inner=None, zorder=-1, linewidth=0.1)
# 	# sns.violinplot(x="Optimization", y="NPV FLOW ADAPTATION COSTS [trillion USD]", data=violindata2, ax=ax5[2], palette=['C3','C4','forestgreen'], cut=0, inner=None, zorder=-1, linewidth=0.1)
# 	# sns.violinplot(x="Optimization", y="NPV RESIDUAL DAMAGES [trillion USD]", data=violindata2, ax=ax5[3], palette=['C3','C4','forestgreen'], cut=0, inner=None, zorder=-1, linewidth=0.1)
# 	# sns.violinplot(x="Optimization", y="NPV TOTAL COSTS & DAMAGES [trillion USD]", data=violindata2, ax=ax5[4], palette=['C3','C4','forestgreen'], cut=0, inner=None, zorder=-1, linewidth=0.1)
# 	# sns.boxplot(x="Optimization", y="NPV ABATEMENT COSTS [trillion USD]", data=violindata2, ax=ax5[0], showfliers=False , boxprops={'facecolor':'None', "zorder":10}, whiskerprops={'linewidth':2, "zorder":10})
# 	# sns.boxplot(x="Optimization", y="NPV STOCK ADAPTATION COSTS [trillion USD]", data=violindata2, ax=ax5[1], showfliers=False, boxprops={'facecolor':'None', "zorder":10}, whiskerprops={'linewidth':2, "zorder":10})
# 	# sns.boxplot(x="Optimization", y="NPV FLOW ADAPTATION COSTS [trillion USD]", data=violindata2, ax=ax5[2], showfliers=False, boxprops={'facecolor':'None', "zorder":10}, whiskerprops={'linewidth':2, "zorder":10})
# 	# sns.boxplot(x="Optimization", y="NPV RESIDUAL DAMAGES [trillion USD]", data=violindata2, ax=ax5[3], showfliers=False, boxprops={'facecolor':'None', "zorder":10}, whiskerprops={'linewidth':2, "zorder":10})
# 	# sns.boxplot(x="Optimization", y="NPV TOTAL COSTS & DAMAGES [trillion USD]", data=violindata2, ax=ax5[4], showfliers=False, boxprops={'facecolor':'None', "zorder":10}, whiskerprops={'linewidth':2, "zorder":10})
# 	# patch_violinplot()
# 	[el.tick_params(axis='x',labelrotation=45) for el in ax4]
# 	[el.set_xticklabels(['1 obj. static', '2 obj. static', 'EMODPS']) for el in ax5]
# 	[el.tick_params(axis='x',labelrotation=45) for el in ax5]
# 	ax.set_xlabel('Percentage of simulation time spent above 2°C [%]')
# 	ax.set_ylabel('Mean Welfare')
# 	cbar1 = fig6.colorbar(im1, ax=ax6[0][0])
# 	cbar1.set_label('NPV ABATEMENT COSTS [trillion USD]')
# 	cbar2 = fig6.colorbar(im2, ax=ax6[1][0])
# 	cbar2.set_label('NPV STOCK ADAPTATION COSTS [trillion USD]')
# 	cbar3 = fig6.colorbar(im3, ax=ax6[1][1])
# 	cbar3.set_label('NPV FLOW ADAPTATION COSTS [trillion USD]')
# 	cbar4 = fig6.colorbar(im4, ax=ax6[0][1])
# 	cbar4.set_label('NPV RESIDUAL DAMAGES [trillion USD]')
# 	cbar5 = fig6.colorbar(im5, ax=ax7)
# 	cbar5.set_label('NPV TOTAL COSTS & DAMAGES [trillion USD]')
# 	ax6[0][0].set_ylabel('Mean Welfare')
# 	ax6[1][0].set_ylabel('Mean Welfare')
# 	ax6[1][0].set_xlabel('Percentage of simulation time spent above 2°C [%]')
# 	ax6[1][1].set_xlabel('Percentage of simulation time spent above 2°C [%]')
# 	ax7.set_xlabel('Percentage of simulation time spent above 2°C [%]')
# 	ax7.set_ylabel('Mean welfare')

plt.show()
