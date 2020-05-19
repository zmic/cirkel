import os
import numpy as np
import imageio
import cirkel

def example_type2():    

    # for reproducibility
    
    np.random.seed(32)

    # An array of start points (x, y, angle). 4 in this example, but as many as you like.
    S = []
    n = 8
    for x in range(n):
        for y in range(n):
            S.append((1/n*(.5+x),1/n*(.5+y),np.random.random()/10))
    S = np.array(S)
    
    # 2d-array of instructions, either 0 or 1    
    J = np.random.randint(0, 2, (2,2))
 
    # 2d-array of rotation angles in radians. Can be negative.
    R = np.random.randint(0,8,(4,4))*np.pi/4

    # 2d-array of diameters (integers > 0)
    D = np.array([
        [50,60,50,50],
        [20,50,10,15]
    ])
    
    # branch array
    B = np.random.randint(0,8,(2,4))*np.pi/2
    B[0][0] *= 0.98
    B[1][2] *= 1.02
        
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
    RGB = (np.random.random((stack_size, 3))*256).astype(np.uint8)

    # number of RGB_INCREMENT must be equal to stack size!
    RGB_INCREMENT = (np.random.randn(stack_size, 3)*10).astype(np.uint8)

    rgb_background = (0,0,23)
    
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
    example_type2()
    
    
    