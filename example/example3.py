import os
import numpy as np
import imageio
from znode import *
import cirkel

#-------------------------------------------------------------------------------------
#
#  an example of cirkel.cirkel2
#
def example_type3():    

    nrnd = ŋnp_RandomState(32)
    
    # An array of start points (x, y, angle). 64 points in this example.
    i = 8
    nxy = ŋnp_transpose(ŋnp_indices((i,i)), (1,2,0))
    nxy = (1/i)*(i+0.5)
    nxy = ŋnp_reshape(nxy, (i*i,2))
        
    na = ŋrandom(nrnd, (i*i,1)) / 10
    S = ŋnp_concatenate((nxy, na), 1)
        
    # 2d-array of "instructions", either 0 or 1    
    J = ŋintegers(nrnd, 0, 2, (2,2))
 
    # 2d-array of rotation angles in radians. Can be negative.
    R = ŋintegers(nrnd,0,8,(4,4))*np.pi/4

    # 2d-array of diameters (integers > 0)
    D = ŋnp_array((
        (50,60,50,50),
        (20,50,10,15)
    ))
    
    # branch array, also angles
    B = ŋintegers(nrnd,0,8,(2,4))*np.pi/2
    
    n = B.slice_multiply[0,0,0.98]
    n = B.slice_multiply[1,2,1.02]
    
        
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
    RGB_INCREMENT = (ŋrandom(nrnd, (stack_size, 3))*256).astype(np.uint8)

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
        
    #--------------------------------------------------------------
    #      
    #   We've got everything, let's go
    # 
    image = cirkel.cirkel2(
        S, J, R, D, B,
        picture_output_size, 
        stack_size, 
        param1, param2,
        RGB, RGB_INCREMENT, rgb_background[0], rgb_background[1], rgb_background[2])
    
    # should be (3, 3072, 3072)    
    print(image.shape)   

    # save picture do disk
    imageio.imwrite("test2.jpg", image)
    
    os.system("test2.jpg")
            
    
if __name__ == '__main__':
    example_type3()
    
    
    
