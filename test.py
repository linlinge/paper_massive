import pandas as pd 
import matplotlib.pyplot as plt
import numpy as np

dat=pd.read_csv("/home/llg/dataset_paper/1.txt")
dat=dat.values
print(np.min(dat))
print(np.max(dat))
plt.plot(dat[0:100],"*")
plt.show()