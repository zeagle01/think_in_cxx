
import numpy as np
import matplotlib.pyplot as plt

fig =plt.figure()
ax=fig.subplots()


def func(x):
    return x*x*x

def deriv(x):
    return 3*x*x

n=10
init_step=1.
h=init_step

sample_point=1

x=[]
error_backword=[]
error_foreword=[]
error_center=[]
for i in range(n):
    x.append(h)
    e=(func(sample_point)-func(sample_point-h))/h-deriv(sample_point)
    error_backword.append(np.abs(e))

    e=(func(sample_point+h)-func(sample_point))/h-deriv(sample_point)
    error_foreword.append(np.abs(e))

    e=(func(sample_point+h)-func(sample_point-h))/h/2-deriv(sample_point)
    error_center.append(np.abs(e))
    h/=2


ax.plot(x,error_backword,'o',label='backward')
ax.plot(x,error_foreword,'o',label='forward')
ax.plot(x,error_center,'o',label='center')
x=np.array(x)
ax.plot(x,x,'-',label='y=x')
ax.plot(x,x*x,'-',label=r'$y=x^2$')

ax.set_xscale('log')
ax.set_yscale('log')

ax.set_title(r'finite difference error $e=D^hf(1)-Df(1)$ with $f(x)=x^3$')
ax.set_ylabel(r'$e$')
ax.set_xlabel(r'$h$')

ax.legend()
plt.show()
