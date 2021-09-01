import pandas as pd

file = pd.read_csv('./comega_neg.csv')
file = file.loc[file[file.columns[1]]=='climcost']
file = file.drop([file.columns[0],file.columns[1]], axis=1)
file = pd.pivot(file, values='value', index='n', columns='V3')
file.columns = ['linear','nonlinear','exponent','intercept']
file.index.names = ['Region']
file.to_csv('./DamagesCoeffWITCH.txt', columns=None, sep='\t')