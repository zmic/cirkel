import json
import randomgen
import numpy as np

def dump____(L, D, n):
    nid = id(n)
    if nid in D:
        return D[nid]
    if isinstance(n, node_literal__):
        args = [n[0][0]]
    elif isinstance(n, node__):
        args = [dump____(L, D, x) for x in n[:-1]]
    r = len(L)
    L.append([n.__class__.__name__, args])
    D[nid] = r
    return r
              
class node____(tuple):        
    def dump(self):
        L = []
        D = {}
        dump____(L, D, n)
        return L
    def json_dumps(self):
        return json.dumps(self.dump())
        
    @staticmethod
    def pack_arg(a):
        if isinstance(a, node____):
            return a
        if isinstance(a, int):
            return ŋint(a)
        if isinstance(a, float):
            return ŋfloat(a)
        if isinstance(a, tuple):
            return ŋtuple(a)
        else:
            raise TypeError("Can't handle type " + type(a).__name__)
        
    @property
    def r(self):
        return self[-1][0]
        
    def __mul__(self, other):
        return self.mul1(self, other)
        
    def __add__(self, other):
        return self.add1(self, other)
   
    def __sub__(self, other):
        return self.sub1(self, other)
        
    def __pow__(self, other):
        return self.pow1(self, other)
        
    def __div__(self, other):
        return self.div1(self, other)
        
    def __mod__(self, other):
        return self.mod1(self, other)
        
    def __neg__(self):
        return self.neg1(self)
        
    def __gt__(self, other):
        return self.gt1(self, other)
        
    def __lt__(self, other):
        return self.lt1(self, other) 
        
    def __ge__(self, other):
        return self.ge1(self, other)
        
    def __le__(self, other):
        return self.le1(self, other)   
    
#-------------------------------------------------------    
class node_literal__(node____):
    def __repr__(self):
        return '{}({})'.format(self.__class__.__name__,repr(self[0][0]))        
    def  __new__(cls, v):
        t = super().__new__(cls, ((v,),))
        return t
  
class ŋint(node_literal__):
    pass

class ŋfloat(node_literal__):
    pass

class ŋtuple(node_literal__):
    def  __new__(cls, v):
        if isinstance(v, list):
            v = tuple(v)
        t = node____.__new__(cls, ((v,),))
        return t

#-------------------------------------------------------    
class node__(node____):
    def  __new__(cls, *args):
        args = [cls.pack_arg(x) for x in args]
        t = super().__new__(cls, args+[list()])
        return t
    def __repr__(self):
        a = self[:-1]
        if len(a) == 1:
           return '{}({})'.format(self.__class__.__name__,repr(self[0]))        
        else:
            return self.__class__.__name__ + repr(self[:-1])        
    def eval(self):
        A = self[:-1]
        for i in A:
            if not i[-1]:
                i.eval()
        r = self.eval__(*[i[-1][0] for i in A])
        self[-1].append(r)
        return r
        
#-------------------------------------------------------    
class ŋadd(node__):
    @staticmethod
    def eval__(x, y):
        return x + y
    
class ŋmul(node__):
    @staticmethod
    def eval__(x, y):
        return x * y
            
#-------------------------------------------------------    
class node_rg__(node__):
    pass

class ŋrg_MT19937(node_rg__):
    @staticmethod
    def eval__(i):
        return randomgen.Generator(randomgen.MT19937(i, mode='sequence'))

#-------------------------------------------------------    
class node_random_quantity__(node__):
    pass


#-------------------------------------------------------    
class node_apply__(node__):
    pass
    
class ŋapply_metaclass__(type):
    def __new__(cls, name, bases, attr):
        cls = type(node_apply__)
        t = cls.__new__(cls, name, node_apply__.__bases__, attr)
        name = name[1:]
        def eval__(s, o, *args):
            return getattr(o, name)(*args)
        t.eval__ = eval__
        return t
        
class ŋstandard_normal(metaclass=ŋapply_metaclass__):
    pass
    
#-------------------------------------------------------    
class node_numpy__(node__):
    pass
    
class node_numpy_metaclass__(type):
    def __new__(cls, name, bases, attr):
        cls = type(node_numpy__)
        t = cls.__new__(cls, name, node_numpy__.__bases__, attr)
        name = name[4:]
        f = getattr(np, name)
        def eval__(s, o, *args):
            return f(*args)
        t.eval__ = eval__
        return t
        
class ŋnp_add(metaclass=node_numpy_metaclass__):
    pass
    

#-------------------------------------------------------    
def json_loads(json_string):
    data = json.loads(json_string)
    L = []
    G = globals()
    for type, args in data:
        t = G[type]
        if issubclass(t, node__):
            args = [L[x] for x in args]
        elif issubclass(t, node_literal__):
            pass
        else:
            raise TypeError()
        L.append(t(*args))  
    return L[-1]

if __name__ == '__main__':
    n = ŋstandard_normal(ŋrg_MT19937(ŋint(12)),(3,4))
    print(n.eval())
    print(n.dump())
    print(json.dumps(n.dump()))
    print(json.loads(json.dumps(n.dump())))
    print(json.loads(n.json_dumps()))
    n2 = json_loads(n.json_dumps())
    print(n)
    print(n2)
    print(n[0][0])
    assert( "ŋstandard_normal(ŋrg_MT19937(ŋint(12)), ŋtuple((3, 4)))" == repr(ŋstandard_normal(ŋrg_MT19937(ŋint(12)), ŋtuple((3, 4)))))
    assert( "ŋstandard_normal(ŋrg_MT19937(ŋint(12)), ŋtuple((3, 4)))" == repr(ŋstandard_normal(ŋrg_MT19937(ŋint(12)), (3, 4))))
    #loads(json.dumps(n.dump()))
    
    
    