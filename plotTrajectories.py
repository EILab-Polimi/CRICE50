import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

data = pd.read_csv("./simulationOutput.txt", sep='\t')
#select field to plot
# field = ['E','E']
field = ['IMPACTchn','KOMEGAchn','Kchn','TOCEAN','MAT','MUP','MLO','FORC','E'] #
print(data[field].describe)
for el in field:
	plt.plot(data['YEAR'], data[el], label=el)
plt.legend()
# plt.show()