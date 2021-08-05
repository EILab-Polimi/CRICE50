import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import axes3d
import os, matplotlib, subprocess, time
import seaborn as sns
import matplotlib.font_manager as font_manager
import numpy as np
import pandas as pd
import plotly.express as px
import plotly.io as pio

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

sols = pd.DataFrame(sols, columns=['Welfare','Years above 2°C','Gini'])
fig = px.parallel_coordinates(sols, color='Gini',
    color_continuous_scale=px.colors.diverging.Tealrose, width=1200, height=800)
# ,
    # color_continuous_midpoint=2)
# Hide the color scale that is useless in this case
fig.update_layout(coloraxis_showscale=False)
pio.write_image(fig, './parallel.pdf', scale=3, width=1200, height=600)
# Show the plot
fig.show()
# print(sols)
# f4_ax1.set_title('Convergence')
# f4_ax5.set_title('Pareto Front')
# fig4.legend(loc='lower right', ncol=nseeds)
# fig4.tight_layout()


# plt.show()
