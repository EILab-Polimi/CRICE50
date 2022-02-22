import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

data = pd.read_csv("./simulationOutput.txt", sep='\t')
pd.set_option('precision',10)
#select field to plot
# field = ['E','E']
field = ['TATM','MIUcan','MIUfra','MIUbra','MIUaus','MIUita','MIUrsaf','MIUzaf'] #
field = [x for x in data.columns if 'MIU' in x ]
# field = ['MIUchn', 'MIUusa' ,'MIUita'] #
print(data[field].describe)
for el in field:
	plt.plot(data['YEAR'], data[el], label=el)
plt.legend(ncol=4)
plt.show()
