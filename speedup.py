# libraries and data
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import matplotlib.colors

# Make a data 
df=pd.DataFrame({'x': [0,2,4], 
'Linear':[0,2,4],
'2^24': [0,0.812,0.702],
'2^24 (p)': [0,1.950,2.437],	
'2^26': [0,0.900,0.843],
'2^26 (p)': [0,1.98,2.722],
'2^28': [0,0.875,0.683],
'2^28 (p)': [0,1.933,2.242]
 })
 
# style
plt.style.use('seaborn-darkgrid')


colors = ('red', 'blue','purple', 'gray', "gold", "limegreen", 'red')
cmap = matplotlib.colors.ListedColormap(colors)

# create a color palette
palette = cmap
 

# multiple line plot
num=0
for column in df.drop('x', axis=1):
    num+=1
    plt.plot(df['x'], df[column], marker='', scalex= 'false',scaley='false', color=palette(num), linewidth=1, alpha=0.9, label=column)
 
# Add legend
plt.legend( ncol=2, title='DataSet')
 
# Add titles
plt.title("Gráfico do Speedup", loc='center', fontsize=12, fontweight=0, color='black')
plt.xlabel("Número de Processadores")
plt.ylabel("Speedup")


plt.show()