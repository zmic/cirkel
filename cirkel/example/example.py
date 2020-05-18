import os
import numpy as np
import imageio
import cirkel

def example1():    

    #--------------------------------------------------------------------------
    # Jump distances. Must be positive.
    #
    # For this example I picked a 4x3 table, but this can basically
    # have any dimension you like.
    #
    J = np.array([
        [0.21,       0.1,        1,        ],
        [0.29,       0.1,        1,        ],
        [1.13376944, 1.09989127, 0.17242821],
        [0.87785842, 0.04221375, 0.58281521],
    ])
 
    # Rotation angles in radians. Can be negative.
    #
    # Again this can have any shape you like 
    #
    R = np.array([
        [ 1.46210794, -2.06014071, -0.3224172,  -0.38405435],
        [-1.3,        -0.5,        -0.8,        -0.09      ],
        [ 0.3,        -1.5,         0.8,        -0.09      ],
    ])

    # create 3072x3072 picture
    picture_output_size = 3072
    
    # max depth of the stack
    stack_size = 8
    
    # begin_D should be < picture_output_size
    begin_D = 1500
    # How aggressively the plot "fans out".
    fanout_multiplier = 80
    
    # f0 must be smaller than f1
    f0 = 0.1
    f1 = 0.4
    
    # Colors of the disks as (r,g,b)
    # Number of colors must be equal to stack size or bad things happen!
    RGB = np.array([
        [200,25,200],
        [12,200,0],
        [200,12,0],
        [200,200,12],
        [100,200,0],
        [200,12,0],
        [200,200,12],
        [100,200,0],
    ], dtype = np.uint8)

    # number of RGB_INCREMENT must be equal to stack size!
    RGB_INCREMENT = np.array([
        [15,0,0],
        [15,0,0],
        [15,1,0],
        [12,0,5],
        [5,0,0],
        [5,0,0],        
        [0,0,0],
        [0,0,0],        
    ], dtype = np.uint8)

    unused = 0
    
    #--------------------------------------------------------------
    #      
    #   We've got everyting, let's go
    # 
    image = cirkel.cirkel1(
        J, R, picture_output_size, 
        stack_size, 
        begin_D, unused, fanout_multiplier, f0, f1,
        RGB, RGB_INCREMENT)
    
    # should be (3, 3072, 3072)    
    print(image.shape)   

    # save picture do disk
    imageio.imwrite("test.jpg", image)
    
    # pop up the picture
    os.system("test.jpg")
            
def example2():

    # try to fit diameters of 51, 33, 23,... pixels    
    diameters = [51, 33, 23, 15, 11, 7]
    cirkel.circlefitter1_set_diameter(np.array(diameters, dtype=np.int32))

    # jump distances, > 0
    J = np.array([
       [0.3, 0. , 0.1, 0.3, 0.4, 0.4, 0.5, 1. ],
       [0.35, 0.2 , 0.14, 0.4, 0.4, 0.5, 0.5, 1.1 ]
    ])
       
    # rotation angles in radians, can be positive or negative       
    R = np.array([
        [ 6.30268806, -3.50992726, -1.80165634, -0.09338542 ],
        [ 6.30268806, -3.50992726, -1.80165634, -0.09338542 ]
    ])

    # create 4000x4000 picture    
    picture_output_size = 4000
    
    # max depth of the stack
    stack_size = 10
    
    # how far from the center we start
    begin_j = 1500
    
    fanout_multiplier = 20
    
    # f0 must be smaller than f1
    f0 = 0.0
    f1 = 0.2  
    
    # RGB values of the disks, per layer of the stack.
    # Number of RGB must be equal to stack size!    
    RGB = np.array([
       [134, 134, 134],
       [235, 135, 135],
       [136, 236, 136],
       [137, 137, 137],
       [139, 139, 139],
       [142, 142, 142],
       [146, 146, 146],
       [153, 153, 153],
       [170, 170, 170],
       [170, 70, 55],
    ], dtype=np.uint8)    
    
    # Number of RGB must be equal to stack size!    
    RGB_INCREMENT = np.array([
       [  1,   1,   1],
       [  1,   1,   1],
       [  1,   1,   1],
       [  1,   1,   1],
       [  0,   0,   0],
       [  1,   1,   1],
       [  1,   1,   1],
       [  1,   1,   1],
       [  1,   1,   1],
       [  30,   14,   14],
    ], dtype=np.uint8)


    unused = 0

    picture = cirkel.cirkel1(
        J, R, picture_output_size, 
        stack_size, 
        begin_j, unused, fanout_multiplier, f0, f1,
        RGB, RGB_INCREMENT)
        
    print(picture.shape)     
    fn = "test_{}.jpg".format(fanout_multiplier)    
    imageio.imwrite(fn, picture)
    os.system(fn)
    
if __name__ == '__main__':
    #example1()
    example2()
    
    
    