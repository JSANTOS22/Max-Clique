# Max Clique Algorithms

## What is a Maximum Clique?
A maximum clique is the biggest subset of vertices in a graph that are fully connected. Meaning that each vertice has edges to every other veertice within that subset. For more about the Maximum Clique Problem, click [here.](https://en.wikipedia.org/wiki/Clique_problem)

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
For this program it is very important that you do not compile it with graphs that have greater than around `26` vertices because it has a `O(2^n)` time complexity.

Once you have your input graph file, all that needs to be dones is compile the program with `C++11` because it uses functionality that earlier versions of C++ doesn't support. You also need to execute the program using `./a.out` along with the name of your input graph file as the command line argument. An example of this is shown below:
```
g++ -std=c++11 BruteForce.cpp
./a.out 'examplefile.adjmat'
```

## Heuristic Compiling
For this prgram, you can compile it with graphs up to around `5000` vertices because it creates a whole compliment of the original matrix. Which takes up storage and `O(n^2)` time along with solving for an estimate of the vertex cover problem to then the independet set which yeilds a clique in the original matrix.

Once you have your input graph file, all that needs to be dones is compile the program with `C++11` because it uses functionality that earlier versions of C++ doesn't support. You also need to execute the program using `./a.out` along with the name of your input graph file as the command line argument. An example of this is shown below:
```
g++ -std=c++11 IS_VC_MC.cpp
./a.out 'examplefile.adjmat'
```

## mySolution Compiling
For this program, you can run it with super large graphs up to around `15,000` vertices becuase this program has an `O((n^2)*lg(n))` time complexity. 

Once you have your input graph file, all that needs to be dones is compile the program with `C++11` because it uses functionality that earlier versions of C++ doesn't support. You also need to execute the program using `./a.out` along with the name of your input graph file as the command line argument. An example of this is shown below:
```
g++ -std=c++11 mySolution.cpp
./a.out 'examplefile.adjmat'
```

## Imporant Notes
- Each program has a brief description at the top about how the algorithm works for your leisure
- The `IS_VC_MC.cpp` file will yeild bad results for the maximum clique solution, but it's important to understand that the vertex cover, independent set, and max clique are all related
- All the programs have a timer to time the execution of the algorithms so you can compare performances of each
