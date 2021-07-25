# bonsai++

`bonsai++` is a 3D bonsai tree generation algorithm built on top of a primitive OpenGL graphics engine. Bonsai growth is randomly generated and animated for fun.  

There are two viewing modes: `USER` where the camera is operated by the user and `ROTATING` where the camera follows a preset path. 


https://user-images.githubusercontent.com/80584824/126897086-d440bc3c-6082-4d9f-aef6-d3bfa200f0ed.mp4



## general-info

`Bonsai` is made up of two parts: a recursive bonsai generation algortihm and an OpenGL graphics engine!  
<br>

#### bonsai generation 

`generateBonsai` is where the brunt of the tree-generation algorithm is, as this function dictates the location of each branch and leaf. `generateBonsai` slowly grows each branch using a recursive function, and on each recursive step, a new branch has a posibility to spawn. As the branches go further outwards, the likelihood of a new branch also increases, this is so the bonsai is more 'tree-y' and mimics a real tree's growth.

As each branch dies, foliage is also generated recursively, simply stacking loose circles of diminishing size on top of the final branch position. To give noise to the foliage, the probabilty that a leaf block will generate decreases proptionate to its distance from the end of the branch. 

Beyond the features mentioned above, the algorithm does also take extra precautions to make sure that a (fairly) life-like bonsai tree is grown.



#### OpenGL graphics engine

Instead of a using a pre-built library to render the model generated by the above algorithm, I decided to build my own model viewer using OpenGL. While still basic, this part of the program features a shading system (vertex and frament shaders only), a primitive lighting system and user-controlled camera movement. 





## technology

This project was written in `C++` and used `OpenGL` to create the graphics engine - the following helper libraries were used:

*   GLAD v0.1.34
*   GLFW v3.3.4
*   stb_image.h



## set-up

This project requires `OpenGL 3.3` to be installed on the system, as it uses the API specification to communicate with with onboard graphics. Being platform independent, this program runs on every major operating system given the libraries listed above and `OpenGL` version 3.3 are setup. 



To run, just excute the `Makefile`

```
make *
```

And then, run `bonsai` using

```
./bonsai
```



## features

`bonsai++` features 2 modes, `USER` where the camera movement is defined by the user and `ROTATING` directed camera view that circles around the bonsai.



### key bindings

#### movement

| keybind      | action                    |
| ------------ | ------------------------- |
| <kbd>w</kbd> | moves camera forward      |
| <kbd>a</kbd> | moves camera to the left  |
| <kbd>s</kbd> | moves camera backwards    |
| <kbd>d</kbd> | moves camera to the right |

(these only work when in `USER` camera mode)



#### camera control

| input             |                                                  |
| ----------------- | ------------------------------------------------ |
| move mouse        | changes view angle                               |
| scroll mousewheel | zooms in and out                                 |
| <kbd>r</kbd>      | switches between `USER` view and `ROTATING` view |



#### bonsai generation

| keybind      | action                              |
| ------------ | ----------------------------------- |
| <kbd>q</kbd> | creates an entirely new bonsai tree |
| <kbd>e</kbd> | re-animates the current bonsai tree |

