import os
import re
import io
import time
import datetime
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

from graph_creator import create_cirkel2_graph_v1

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
        self.root = str(root)
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
    data = json.dumps(data).encode('ascii')
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
    
def read_graph(path):    
    data = read_metadata(path)
    return data['graph']    
    
def walk_folder(path, shuffle = True):
    files = list(path.rglob("*.jpg"))
    if shuffle:
        random.shuffle(files)
    for x in files:
        yield x
                
                
def save_image(L, image, type, graph):
    graph_dump = graph.dump()
    now = datetime.datetime.now() 
    now = now.strftime("%Y-%m-%d %H:%M:%S")
    metadata = {
        'info' : {
            'type' : type,
            'created' : now,
            'version' : g_commits
        },
        'graph' : graph_dump
    }    
    
    stream = io.BytesIO()
    imageio.imwrite(stream, image, format='jpg')   
    imdata = stream.getvalue()        
    imdata = set_image_data(imdata, metadata)
    folder = L()
    path = "{}/c_{}.jpg".format(folder, int(time.time()*100))
    print(path)
    open(path, "wb").write(imdata) 
    return path
    

######################################################################################################

def vary_folder(L, folder):       
    for x in walk_folder(folder, False):
        print(x)
        data = read_graph(x)
        old_graph = znode.load(data)
        nrnd = old_graph.find_first_of_type(ŋnp_RandomState)
        print(nrnd[0].r)
        graph = create_graph(nrnd[0].r, random.randint(0,1000000000))
        graph.eval(debug=0)
        image = cirkel.cirkel2(*graph.r)
        save_image(L, image, "cirkel.cirkel2", graph) 
        
def clone_folder(L, folder):       
    for x in walk_folder(folder, False):
        print("read:", x)        
        graph = read_graph(x)
        graph = znode.load(graph)
        #print(graph)
        graph.eval()
        image = cirkel.cirkel2(*graph.r)
        print("write:", x)        
        save_image(L, image, "cirkel.cirkel2", graph) 
        
              
def create_new_cirkel2(L):       
    seed1 = random.randint(0,1000000000)
    seed2 = random.randint(0,1000000000)
    seed3 = random.randint(0,1000000000)
    graph = create_cirkel2_graph_v1(seed1, seed2, seed3)        
    graph.eval()
    image = cirkel.cirkel2(*graph.r)
    save_image(L, image, "cirkel.cirkel2", graph) 

if __name__ == '__main__':
    print("-------------------------------------------------------------------")
    g_commits = do_git()  
    print("-------------------------------------------------------------------")

    root_folder = Path(r"f:/gen/cirkel")
    good_folder = root_folder / "XK"
    L = layered_saver(root_folder / "GEN", 300)
    
    for i in range(100):
        #vary_folder(L, good_folder)
        #clone_folder(L, good_folder)
        create_new_cirkel2(L)




 