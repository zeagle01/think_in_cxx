
import numpy as np
import matplotlib.pyplot as plt

fig=plt.figure()

data_type=[np.float32,np.float64]

ax=fig.subplots()

xb = -18
xe = -1

for i in range(len(data_type)):
    x=np.linspace(xb,xe,xe-xb+1,dtype=data_type[i])
    x=np.power(10,x)
    y=((2+x)-2)/x

    ax.plot(x,y,'-o',label=str(data_type[i]))

ax.set_xscale('log')
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.legend()
ax.set_title(r'$y=\frac{(x+2)-2}{x}$')


plt.show()


