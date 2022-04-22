import numpy as np
import matplotlib.pyplot as plt


def create_square_mesh(n,m):
    gx=np.linspace(0,n,n);
    gy=np.linspace(0,m,m);
    mx=np.meshgrid(gx,gy)

    def getIndex(i,j):
        return i+j*n

    squareN=n-1
    squareM=m-1
    tempT=[]
    for i in range(squareN):
        for j in range(squareM):
            i0=getIndex(i,j)
            i1=getIndex(i+1,j)
            i2=getIndex(i+1,j+1)
            i3=getIndex(i,j+1)
            tempT.append([i0, i1, i3])
            tempT.append([i1, i2, i3])
    tempX=np.zeros((n*m,2))
    for i in range(n):
        for j in range(m):
            tempX[getIndex(i, j)][0] = mx[0][i, j]
            tempX[getIndex(i, j)][1] = mx[1][i, j]
    X=tempX
    T=np.array(tempT)
    return X,T



def partition(T):
    n=len(T)
    partition_num=3
    ret=[]
    for pi in range(partition_num):
        group=[]
        for i in range(n//partition_num):
            group.append(pi*(n//partition_num)+i)
        ret.append(group)
    return ret

fig=plt.figure()
ax=fig.add_subplot(111)

def plot_mesh(ax,pos,T,c):
    for t in T:
        ax.fill(pos[t, 0], pos[t, 1],color=[0,0,c])
        ax.scatter(pos[t,0],pos[t,1],color='black')

X,T=create_square_mesh(10,10)

groupT=partition(T)

for i,g in enumerate(groupT):
    plot_mesh(ax,X,T[g],i*0.2)
plt.show()