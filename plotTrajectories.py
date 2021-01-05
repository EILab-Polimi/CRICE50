import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

data = pd.read_csv("./simulationOutput.txt", sep='\t')
#select field to plot
field = ['Enorth','Esouth']
field = ['GDPBASEsouth','YGROSSsouth']
print(data[field].describe)
for el in field:
	plt.plot(data['YEAR'], data[el], label=el)
plt.legend()
plt.show()