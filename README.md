

# CIRKEL

<img src="https://raw.githubusercontent.com/zmic/cirkel/master/examples/test_0.jpg" width=400px height=400px>

## What is it?
It's a Python module written in C++ that paints colored disks on a canvas, using an inscrutable automaton-like ruleset.

Examples of what this program can produce can be found on 
[my Flickr page](https://www.flickr.com/photos/66348526@N00/albums/72157665578003511)

## How to install it (Windows only for now)

On Windows:
`pip install cirkel`

OR

clone this repository, build the binary with Visual Studio 2017/2019 and run setup.py

## Disclaimer
I wrote this code in a hurry during a two week period in 2014, for fun and quick experimentation. It was never refactored or cleaned up as it should be. It works, but does not meet my professional standards for deliverable source code.

## How to  use it
Call `cirkel.cirkel1` with a pile of parameters and you will get back an image (as a numpy array). Finding the parameters that produce nice results, therein lies the art.

```
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

    # The unused parameter. Don't use it.
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
```




