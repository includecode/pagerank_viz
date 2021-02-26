# pagerank_viz
We will build a program for computing the famous pagerank algorithm. Our dataset (Vectors and Edges) is taken from Gnutella peer-to-peer network, August 9 2002. We will:
    1. **Build the network**
    2. **Execute a pagerank algorithm with the square method**
    3. **Adding some damping factors**
    4. **Graphical display of our network after the pagerank algorithm is applied**
    5. **Discuss our results**
# How to get this program
You can clone this repository:
```git clone https://github.com/includecode/pagerank_viz```

# How to run this program
Test with default values
- Go to the **pagerank_viz** folder and create the binary:
```cd pagerank_viz```
```make all```

## Run with build in examples
In other to be sure everything is ok, it is highly recommended to test the program with the build in examples,:
    ```./app 8114 0.85 10 4```
    Run the above line to launch the program
    This program takes 4 required arguments:
    -**8114** The size of the network (graph)
    -**0.85** The value of alpha (damping factor)
    -**10** After the program is done, the vertices are sorted according to the eigen values, this parameter precises the max number of vertices to display
    -**4** The number of lines to skip from input file: Some files have a description wich should be skipped when creating the graph, for example our file **p2p-Gnutella09.txt** have 4 "useless" lines. So we should skip them. BUT this parameter should be setted to 0 if you don't have lines to be skipped.

    The input file is very huge, i provided a more convenient file **test.txt**, if you want to use this simpler file, you should rename it to **p2p-Gnutella09.txt** (make a copy of the original file!). Run with ```./app 6 0.85 5 0```


# Important informations
    1. The size of the graph should be the correct one, the first node id should be 0. So if you have vertices from 0 to 50, your sizé is 51!
    
    2. /!\ This program will crash if you have a vertex with an id greater than the size of the graph
    example:
        ```
        0   1
        1   10
        ```
        The size of this graph is 3 (O, 1 and 10), but the program will crash because the id of vertices should be at most 2!, so if your file has this configuration, please rename 10 to 2.
     3. Make sure vertices are seperated by TABULATIONS, not spaces
