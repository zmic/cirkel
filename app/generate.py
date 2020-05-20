import os
import re
import io
import time
import numpy as np
import imageio
import exiv2
import random
import json
from pathlib import Path
from git import Repo
from znode import *
import znode
from znode import *
import cirkel

def do_git():
    git_path1 = Path(__file__).absolute().parent.parent
    git_path2 = git_path1.parent / 'znode'
    def check_commit(gp):
        print(gp)
        repo = Repo(gp)
        assert not repo.bare
        changed_files = [ item.a_path for item in repo.index.diff(None) ]
        if changed_files:
            for x in changed_files:
                print('   ',x)
            print("    Committing...")
            repo.git.commit('-a', '-m', 'autocommit', author='michael <michael@not.here.com>')
        return str(repo.head.commit)    
    return { 'cirkel':check_commit(git_path1), 'znode':check_commit(git_path2) }



##############################################################################################################################
class layered_saver:
    def __init__( self, root, nbr_per_folder ):
        self.root = root
        self.nbr_per_folder = nbr_per_folder
        self.boot()
    def set(self, hif, lof):
        self.path = self.root + '/%04d/%04d'%(hif, lof) 
        if not os.path.exists(self.path):
            print("makedir", self.path)
            os.makedirs(self.path)        
        self.hif, self.lof, self.fic = hif, lof, len(os.listdir(self.path))
    def boot(self):
        re_digits = re.compile(r'\d+\d+\d+\d+')
        for dirpath, dirnames, filenames in os.walk( self.root ):
            folders = [ d for d in dirnames if re_digits.match(d) ]
            folders.sort()
            break
        hif = folders[-1] if folders else '0000'
        for dirpath, dirnames, filenames in os.walk( self.root + os.sep + hif):
            folders = [ d for d in dirnames if re_digits.match(d) ]
            folders.sort()
            break
        lof = folders[-1] if folders else '0000'
        hif, lof = int(hif), int(lof)
        self.set(hif, lof)
        print("layered_saver starting at %d, %d, %d"%(self.hif, self.lof, self.fic))
    def __call__( self ):
        if self.fic > self.nbr_per_folder:
            lof = self.lof + 1
            self.set(self.hif + lof/self.nbr_per_folder, lof%self.nbr_per_folder)
        self.fic +=1 
        return self.path
    
#####################################################################################################    

def set_image_data(im, data):
    data = json.dumps(data)
    im = exiv2.ImageFactory.open(im)
    im.readMetadata()

    new_iptc_data = exiv2.IptcData()
    new_iptc_data[b"Iptc.Application2.Caption"] = data
    im.setIptcData(new_iptc_data)
    im.writeMetadata()

    io = im.io()
    size = io.size()
    buffer = io.read(size)
    return buffer
        
def read_metadata(path):
    image = exiv2.ImageFactory.open(str(path))
    image.readMetadata()
    ipc_data = image.iptcData()
    jsondata = ipc_data[b"Iptc.Application2.Caption"].toString().decode('ascii')
    data = json.loads(jsondata)
    return data
    #return znode_load(data['graph'])
    
def read_folder(path, shuffle = True):
    files = list(path.rglob("*.jpg"))
    if shuffle:
        random.shuffle(files)
    for x in files:
        yield x
                
                
def save_image(L, image, graph):

    data = graph.dump()
    data = {
        'info' : {
            g_commits
        },
        'graph' : data
    }
    
    data = json.dumps(data)
    stream = io.BytesIO()
    imageio.imwrite(stream, image, format='jpg')   
    imdata = stream.getvalue()        
    imdata = set_image_data(imdata, bytes(graph_data, encoding='ascii'))
    folder = L()
    path = "{}/c_{}.jpg".format(folder, int(time.time()*100))
    print(path)
    open(path, "wb").write(imdata) 
    
#####################################################################################################    

def create_graph(seed, seed2):
    nrnd = ŋnp_RandomState(seed)
    nrnd2 = ŋrg_MT19937(seed2)
    
    # An array of start points (x, y, angle). 64 points in this example.
    i = 6 + ŋintegers(nrnd2, 0, 4, (1,))
    nxy = ŋnp_transpose(ŋnp_indices((i,i)), (1,2,0))
    nxy = (1/i)*(nxy+0.5)
    nxy = ŋnp_reshape(nxy, (i*i,2))
        
    na = ŋrandom(nrnd, (i*i,1)) / 10
    S = ŋnp_concatenate((nxy, na), 1)
    S = ŋnp_ascontiguousarray(S)
        
    # 2d-array of "instructions", either 0 or 1    
    J = ŋrandint(nrnd, 0, 2, (2,2))
 
    # 2d-array of rotation angles in radians. Can be negative.
    R = ŋrandint(nrnd,0,8,(4,4))*np.pi/4

    # 2d-array of diameters (integers > 0)
    D = ŋnp_array((
        (50,60,50,50),
        (20,50,10,15)
    ))
    
    # branch array, also angles
    B = ŋrandint(nrnd,0,8,(2,4))*np.pi/2
    
    B = B.slice_multiply[0,0:1,0.98]
    B = B.slice_multiply[1,2:3,1.02]
    
        
    # create 3072x3072 picture
    picture_output_size = 3072
    
    # max depth of the stack
    stack_size = 4 + ŋintegers(nrnd2, 0, 4, (1,))
    
    # param1, integer
    param1 = 1

    # param2, integer
    param2 = 20
        
    # Colors of the disks as (r,g,b)
    # Number of colors must be equal to stack size or bad things happen!
    RGB = (ŋrandom(nrnd, (stack_size, 3))*256).astype(np.uint8)

    # number of RGB_INCREMENT must be equal to stack size!
    RGB_INCREMENT = (ŋrandn(nrnd, stack_size, 3)*10).astype(np.uint8)

    r_background = 0
    g_background = 0
    b_background = 23
    
    n_all = ŋtuple(
        S, J, R, D, B,
        picture_output_size, 
        stack_size, 
        param1, param2,
        RGB, RGB_INCREMENT, r_background, g_background, b_background
    )
        
    return n_all

def create_graph(seed):
    nrnd = ŋnp_RandomState(seed)
    
    # An array of start points (x, y, angle). 64 points in this example.
    i = 8
    nxy = ŋnp_transpose(ŋnp_indices((i,i)), (1,2,0))
    nxy = (1/i)*(nxy+0.5)
    nxy = ŋnp_reshape(nxy, (i*i,2))
        
    na = ŋrandom(nrnd, (i*i,1)) / 10
    S = ŋnp_concatenate((nxy, na), 1)
    S = ŋnp_ascontiguousarray(S)
        
    # 2d-array of "instructions", either 0 or 1    
    J = ŋrandint(nrnd, 0, 2, (2,2))
 
    # 2d-array of rotation angles in radians. Can be negative.
    R = ŋrandint(nrnd,0,8,(4,4))*np.pi/4

    # 2d-array of diameters (integers > 0)
    D = ŋnp_array((
        (50,60,50,50),
        (20,50,10,15)
    ))
    
    # branch array, also angles
    B = ŋrandint(nrnd,0,8,(2,4))*np.pi/2
    
    B = B.slice_multiply[0,0:1,0.98]
    B = B.slice_multiply[1,2:3,1.02]
    
        
    # create 3072x3072 picture
    picture_output_size = 3072
    
    # max depth of the stack
    stack_size = 6
    
    # param1, integer
    param1 = 1

    # param2, integer
    param2 = 20
        
    # Colors of the disks as (r,g,b)
    # Number of colors must be equal to stack size or bad things happen!
    RGB = (ŋrandom(nrnd, (stack_size, 3))*256).astype(np.uint8)

    # number of RGB_INCREMENT must be equal to stack size!
    RGB_INCREMENT = (ŋrandn(nrnd, stack_size, 3)*10).astype(np.uint8)

    r_background = 0
    g_background = 0
    b_background = 23
    
    n_all = ŋtuple(
        S, J, R, D, B,
        picture_output_size, 
        stack_size, 
        param1, param2,
        RGB, RGB_INCREMENT, r_background, g_background, b_background
    )
    
    n_all.eval()
    return n_all

######################################################################################################

def vary_folder():       
    folder = Path(r"F:\GEN\XC")
    L = layered_saver(r"F:\GEN\CIRKEL", 300)
    for x in read_folder(folder, False):
        print(x)
        data = read_metadata(x)
        graph = znode.load(data)
        #print(graph)
        nrnd = graph.find_first_of_type(ŋnp_RandomState)
        print(nrnd[0].r)
        #graph2 = create_graph(nrnd[0].r, random.randint(0,1000))
        '''
        graph2.eval()
        image = cirkel.cirkel2(*graph2.r)
        graph_dump = graph2.json_dumps()
        save_image(L, image, graph_dump) 
        '''
        
        
        
def generate_new():       
    L = layered_saver(r"F:\GEN\CIRKEL", 300)
    for i in range(1000,2000):
        graph = create_graph(i)        
        graph.eval()
        image = cirkel.cirkel2(*graph.r)
        save_image(L, image, graph) 

if __name__ == '__main__':
    print("-------------------------------------------------------------------")
    g_commits = do_git()  
    print("-------------------------------------------------------------------")

    vary_folder()




 