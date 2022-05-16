import numpy as np
import matplotlib.pyplot as plt
import copy


def read_obj(file):
    T=[]
    X=[]
    with open(file,'r') as fin:
        while fin:
            line=fin.readline()
            words=line.split()
            if len(words)==0:
                break
            indicator=words[0]
            #if indicator == 'v':
            #    x=[]
            #    for i in range(3):
            #        x.append(float(words[i+1]))
            #    X.append(x)
            if indicator == 'vt':
                x = []
                for i in range(2):
                    x.append(float(words[i + 1]))
                X.append(x)
            elif indicator=='f':
                t=[]
                for i in range(len(words)-1):
                    indices=words[i+1].split('/')
                    t.append(int(indices[0])-1)
                T.append(t)

    return np.array(X),np.array(T)

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



def partition(T,v_num):
    n=len(T)
    ret=[]
    init_v=0
    num_of_group=12


    class Topology:
        def __init__(self,vv,ve,ef,edges,tt,vt):
            self.vv=vv
            self.ve=ve
            self.ef=ef
            self.edges=edges
            self.tt=tt
            self.vt=vt

    def build_topology(T,v_num):
        vv=[[] for i in range(v_num)]
        ve=[[] for i in range(v_num)]
        ef=[]
        edges=[]
        tt=[[] for i in range(len(T))]
        vt=[[] for i in range(v_num)]

        for i,t in enumerate(T):
            for j in range(3):
                v0=t[j]
                v1=t[(j+1)%3]
                if v1 in vv[v0]:
                    e_pos=0
                    for k,v in enumerate(vv[v0]):
                        if v1==v:
                            e_pos=k
                            break

                    e=ve[v0][e_pos]

                    otherT=ef[e][0]
                    ef[e].append(i)
                    tt[otherT].append(i)
                    tt[i].append(otherT)
                else:
                    vv[v0].append(v1)
                    vv[v1].append(v0)
                    e=len(edges)
                    ve[v0].append(e)
                    ve[v1].append(e)
                    edges.append([v0,v1])
                    ef.append([i])

                vt[v0].append(i)

        return Topology(vv,ve,ef,edges,tt,vt)

    def get_neighbors(v,T,topology):
        ret=[]
        for i in range(3):
            for neighbor_t in topology.vt[T[v][i]]:
                if not  neighbor_t in ret:
                    ret.append(neighbor_t)
        return ret


    def find_largest_group(groups):
        largest_size=-1
        largest_g_index=-1;
        for i,g in enumerate(groups):
            if largest_size<len(g):
                largest_size=len(g)
                largest_g_index=i
        return largest_g_index

    def find_traverse_init_point(group,primitives,topology):
        distance=0
        init_v=0
        while True:
            levels=traverse(init_v,group,primitives,topology)
            new_distance=len(levels)
            if new_distance>distance:
                last_levels=levels[-1]
                min_degree=1e6
                for v in last_levels:
                    degree=len(get_neighbors(v,primitives,topology))
                    if degree<min_degree:
                        min_degree=degree
                        next_v= group.index(v)
                        distance=new_distance
                init_v=next_v
            else:
                break

        return init_v
        #return 0


    def traverse(init_v,group,primitives,topology):
        last_level=[group[init_v]]
        levels=[]
        levels.append(last_level)
        is_processed = np.array([False for i in range(len(primitives))])
        while True:
            current_level=[]
            for last_level_v in last_level:
                for v in get_neighbors(last_level_v,primitives,topology) :
                    if (not is_processed[v]) and (v in group):
                        current_level.append(v)
                        is_processed[v]=True
            if len(current_level)>0:
                levels.append(current_level)
                last_level=current_level
            else:
                break
        return levels

    def level_traverse(group,primitives,topology):
        init_v=find_traverse_init_point(group,primitives,topology)
        levels=traverse(init_v,group,primitives,topology)

        ret=[]
        if len(levels)<5:
            return ret

        low_level=[]
        boundary=[]
        high_level=[]
        for i in range(len(levels)):
            for v in levels[i]:
                if( i< len(levels)//2):
                    low_level.append(v)
                elif i==len(levels)//2:
                    boundary.append(v)
                else:
                    high_level.append(v)

        ret.append(low_level)
        ret.append(high_level)
        return ret,boundary



    topology=build_topology(T,v_num)

    primitives=T

    whole=[i for i in range(len(primitives))]
    ret=[]
    ret.append(whole)
    boundaries=[]
    while len(ret)<num_of_group:
        print(len(ret))
        largest_group_index=find_largest_group(ret)
        largest_group=ret[largest_group_index]
        tmep_groups=copy.copy(ret)
        ret=[]
        for i in range(len(tmep_groups)):
            if i==largest_group_index:
                continue
            ret.append(tmep_groups[i])

        sub_group,boundary=level_traverse(largest_group,primitives,topology)
        if len(sub_group)==2:
            for sub_g in sub_group:
                ret.append(sub_g)
            for b in boundary:
                boundaries.append(b)
        else:
            break
        print('abc')

    ret.append(boundaries)
    return ret

fig=plt.figure()
ax=fig.add_subplot(111)

def plot_mesh(ax,pos,T,c):
    for t in T:
        ax.fill(pos[t, 0], pos[t, 1],color=c)
        ax.fill(pos[t, 0], pos[t, 1],fill=False)
        ax.scatter(pos[t,0],pos[t,1],color='black')

X,T=create_square_mesh(30,30)
#X,T=read_obj('test_meshes/plane_176.obj')

groupT=partition(T,len(X))

n=len(groupT)

def ind2color(i,n):
    k=0
    while True:
        if n<=np.power(k+1,3) and n>np.power(k,3):
            m=k+1
            break
        k+=1

    c=[]
    k=i
    for j in range(3):
        r=k%m
        c.append(1.*r/m)
        k=k//m

    return c


for i,g in enumerate(groupT):
    color=ind2color(i,n)
    print(color)
    plot_mesh(ax,X,T[g],ind2color(i,n))
plt.show()