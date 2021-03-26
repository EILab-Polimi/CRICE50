import pandas as pd

## read WITCH mapping
with open('./witch13.inc') as f:
	file = f.read().split("\n")
witch13 = [line.split() for line in file if line!=""]
regw13 = [line[0] for line in witch13[2:2 + 13]]

## read ed57 mapping
with open('./ed57.inc') as f:
	file = f.read().split("\n")
ed57 = [line.split() for line in file if line!=""]
reged57 = [line[0] for line in ed57[2:2 + 57]]

## combine the two above to map witch to ed57
mapping = []
for line57 in ed57[2 + 58:]:
	if line57[0].split('.')[0] in reged57:
		for line13 in witch13[2 + 14:]:
			if line13[0].split('.')[0] in regw13 and line13[0].split('.')[1] == line57[0].split('.')[1]:
				flag = 0
				for line in enumerate(mapping):
					if line57[0].split('.')[0] in line[1][0] and line13[0].split('.')[0] in line[1][1]: 
						mapping[line[0]][2] += 1.0
						flag = 1
				if flag == 0:
					mapping.append([line57[0].split('.')[0],line13[0].split('.')[0], 1.0])

## check
# print(mapping)
# print(len(mapping))
# mapping_unq = []
# for el in mapping:
# 	if el[0] not in mapping_unq:
# 		mapping_unq.append(el[0])
# print(len(mapping_unq))

## compute weights from WITCH to ed57
weights = {}
for reg57 in mapping:
	temp = []
	for line in mapping:
		if line[0] == reg57[0]:
			temp.append(line[1:])
	tot = 0.0
	for el in temp:
		tot += el[1]
	for el in enumerate(temp):
		temp[el[0]][1] = temp[el[0]][1] / tot
	weights[reg57[0]] = temp
# print(weights)

## read AdaptCoeff from AD-WITCH
with open('../../settings/AdaptCoeffOrigADWITCH.txt') as f:
	file = f.read().split("\n")
newfile = [line.split() for line in file]

## assign coefficients to ED57 via weighted average from AD-WITCH
insk = ["dk_adsad", "dk_adcap", "rho_ad", "rho_adact", "rho_adcap"]
insv = [0.1, 0.03, 0.167, 0.167, -4.0]
AdaptCoeffED57 = []
for reg in weights:
	temp = []
	for regw in weights[reg]:
		if regw[0] == 'india':
			regw[0] = 'sasia'
		if regw[0] == 'oldeuro':
			regw[0] = 'weuro'
		if regw[0] == 'neweuro':
			regw[0] = 'eeuro'
		for line in newfile:
			if line[0].lower() == regw[0]:
				temp.append([float(el)*regw[1] for el in line[1:]])
	values = [0.0 for x in range(len(temp[0]))]
	for value in temp:
		for x in enumerate(value):
			values[x[0]] += x[1]
	values = [*insv, *values]
	values = [str(x) for x in values]
	AdaptCoeffED57.append([reg, *values])
## check
# print(len(AdaptCoeffED57))

## write weights to file
keys = [newfile[0][0],*insk, *newfile[0][1:]]
with open('../../settings/AdaptCoeffOrigED57.txt','w') as f:
	f.write(('\t').join(keys)+"\n")
	for line in AdaptCoeffED57:
		f.write(('\t').join(line)+"\n")
