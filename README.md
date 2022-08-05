# TSP-project

We implemented Lin-Kernighan Heuristic algorithm with its improvements for solving TSP in C++ as project of Combinatorial optimization course at SUT.

We also tried to solve HCP with our algorithm for TSP using reduction, but the results were not good enough.

## Overview

* `tsp_solver.h`: The main algorithm is implemented in this file. You can use it by pass the distances matrix to its solve method.
* `utils.h`: Some utils that we needs are implemented here, Like a_nearness, farthest-tour-insertion, etc.
* `tsp.cpp`: This is the main file of TSP algorithm. It reads file name as input and will process it and executes tsp_solver.
* `hcp.cpp`: This is the main file of HCP algorithm. It reads file name as input and will reduce it to TSP and solves it with tsp_solver.

## Description of the algorithm

LKH algorithm is an iterative algorithm which starts by an initial tour and improves it at each step. It repeat the improvment step until no improvement found.

First, we start the LKH algorithm with an initial tour obtained by the farthest insertion algorithm.

Let define two sets X and Y. They contains the edges that we want to remove them from or add to the current tour, respectively. They are both empty at the first of each improvement step.

At the start of improvement step, we choose an edge `e` from the our tour and put it to X. Then we choose an edge that shares an endpoint with e that is not appeared in tour and put it to Y. Then we call a recursive function named chooseX.

The union of edges in the X and Y sets, creates a path. 
Suppose that the endpoint of this path that corresponds to Y edges be `v` and the other endpoint be `u` (`u` is the first choosen vertex). 
In the chooseX function, we choose one of two edges from our tour that are connected to `v` and not previously added to X and we put it to X (name it (v, w)). 
If we could achieve a better tour by connecting w to u (putting (w, u) to Y), then we end this improvement step and start another improvement step.
Otherwise, we choose another edge to put in Y. It should be one of the edges of `w`. 
We search for this edge in 5 nearest neighbours of `w` in the graph and the distances is based on `alpha nearness` parameter. Then we call chooseX again and so on.


## Getting Started

### Dependencies

You must have an C++ compiler (and python if you want to plot the results) installed on your device to run the program.


### Executing TSP solver

Compile and run `tsp.cpp` and then enter relative address of test name as input. It's recommended to compile the code with c++11 or greater version.

### Executing HCP solver

Compile and run `hcp.cpp` and then enter relative address of test name as input. It's recommended to compile the code with c++11 or greater version.

### Executing plotter

You can plot the graph and tour using our plotter program. Just run the `plotter.py` and then enter the name of test file as input.

## Results

TODO

## Acknowledgments
Inspiration, code snippets, etc.
* [An Effective Implementation of the Lin-Kernighan Traveling Salesman Heuristic](http://webhotel4.ruc.dk/~keld/research/LKH/LKH-2.0/DOC/LKH_REPORT.pdf)
* [Implementing Lin-Kernighan in Python](https://arthur.maheo.net/implementing-lin-kernighan-in-python/)
* [TSPLIB](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/)
* [TSPLIB95 documents](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/tsp95.pdf)
