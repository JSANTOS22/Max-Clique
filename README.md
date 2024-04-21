# Max Clique Algorithms

## Input Graph Structure
The structure of the graphs being read into all three aglorithms consist of a diagonal matrix with each line starting with the vertice's number. An example of this can be seen below:

```
0
1 0
2 1 0
3 1 1 1
```

It's important to note that 0's represent a non existent edge where as 1's represent an existing edge. This is so the algorithms can tell if there's an edge or not in between to vertices.

Within each algorithm, there's a function that reads in the diagonal matrix and makes it symmetrical to fit the complete graph representation like this below:

```
0 0 1 1
0 0 0 1
1 0 0 1
1 1 1 0
```

For instance, in the final matrix above the 4th vertice has an existing edge with all the other vertices but of course not with itself.

When making an input graph, make sure you lable the file as an adjacency matrix file: 
```example.adjmat```

## Brute Force Compiling
For this program it is very important that you do not compile it with graphs that have greater than around `26` vertices because it has a `O(2^(n-1))` time complexity.