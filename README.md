# TSP-project

We implemented the Lin-Kernighan Heuristic algorithm with its improvements for solving TSP in C++ as the project of the Combinatorial optimization course at SUT.

We also tried to solve HCP with our algorithm for TSP using reduction, but the results were not good enough.

## Overview

* `tsp_solver.h`: The main algorithm is implemented in this file. You can use it by passing the distances matrix to its solve method.
* `utils.h`: Some utils that we need are implemented here, Like a_nearness, farthest-tour-insertion, etc.
* `tsp.cpp`: This is the main file of the TSP algorithm. It reads the file name as input and will process it and executes tsp_solver. It also supports `EUC_2D`, `CEIL_2D`, `ATT`, `GEO`, `MAX_2D` and `MAN_2D` distance types.
* `hcp.cpp`: This is the main file of the HCP algorithm. It reads the file name as input and will reduce it to TSP and solves it with tsp_solver.

## Description of the algorithm

LKH algorithm is an iterative algorithm that starts with an initial tour and improves it at each step. It repeats the improvement step until no improvement is found.

First, we start the LKH algorithm with an initial tour obtained by the farthest insertion algorithm.

Let's define two sets X and Y. They contain the edges that we want to remove from or add to the current tour, respectively. They are both empty at the first of each improvement step.

At the start of the improvement step, we choose an edge `e` from our tour and put it to X. Then we choose an edge that shares an endpoint with e that is not appeared in the tour and put it to Y. Then we call a recursive function named chooseX.

The union of edges in the X and Y sets creates a path.
Suppose that the endpoint of this path that corresponds to Y edges is `v` and the other endpoint is `u` (`u` is the first chosen vertex).
In the chooseX function, we choose one of two edges from our tour that are connected to `v` and not previously added to X and we put it to X (name it (v, w)).
If we could achieve a better tour by connecting w to u (putting (w, u) to Y), then we end this improvement step and start another improvement step.
Otherwise, we choose another edge to put in Y. It should be one of the edges of `w`.
We search for this edge in the 5 nearest neighbors of `w` in the graph and the distances are based on the `alpha nearness` parameter. Then we call chooseX again and so on.


## Getting Started

### Dependencies

You must have a C++ compiler (and python if you want to plot the results) installed on your device to run the program.


### Executing TSP solver

Compile and run `tsp.cpp` and then enter the relative address of the test name as input. It's recommended to compile the code with c++11 or greater version.

### Executing HCP solver

Compile and run `hcp.cpp` and then enter the relative address of the test name as input. It's recommended to compile the code with c++11 or greater version.

### Executing plotter

You can plot the graph and tour using our plotter program. Just run the `plotter.py` and then enter the name of the test file as input.

## Results

We ran our algorithm for testing in the tsplib dataset. Here are the results:

| Test Name 	| Optimal Solution 	| Our Solution 	| Ratio 	| Execution Time 	|
|:---------:	|:----------------:	|:------------:	|:-----:	|:--------------:	|
|    a280   	|         2579         	|     2650     	|       	|     0.323 s    	|
|   ali535  	|         202310         	|    205825    	|       	|     2.903 s    	|
|   att48   	|         10628         	|     10711    	|       	|     0.104 s    	|
|   att532    |         27686         	|     28293        	|       	|       1.706 s         	|
|   berlin52    |        7542          	|     7542        	|       	|       0.064 s         	|
|   bier127    |          118282        	|     122967        	|       	|       0.696 s         	|
|   brd14051    |        469935          	|     479561        	|       	|       208.887 s         	|
|   burma14    |        3323          	|     3461        	|       	|       0.019 s         	|
|   ch130    |        6110          	|     6197        	|       	|       0.414 s         	|
|   ch150    |         6528         	|     6655        	|       	|       0.253 s         	|
|   d198    |         15780         	|     15818        	|       	|       1.993 s         	|
|   d493    |         35002         	|     35558        	|       	|       3.293 s         	|
|   d657    |         48912         	|     50247        	|       	|       2.67 s         	|
|   d1291    |         50801         	|     53180        	|       	|       8.642 s         	|
|   d1655    |         62128         	|     64900        	|       	|       8.363 s         	|
|   d2103    |         80529         	|     83021        	|       	|       8.839 s         	|
|   d15112    |                  	|     1608776        	|       	|       245.01 s         	|
|   d18512    |         645923         	|     659005        	|       	|       383.11 s         	|
|   dsj1000    |         18659688         	|     19192559        	|       	|       9.411 s         	|
|   eil51    |         426         	|     426        	|       	|       0.035 s         	|
|   eil76    |         538         	|     549        	|       	|       0.093 s         	|
|   eil101    |                  	|     638        	|       	|       0.173 s         	|
|   fl417    |        11861          	|     11874        	|       	|       2.497 s         	|
|   fl1400    |        20127          	|     20485        	|       	|       22.443 s         	|
|   fl1577    |        22249          	|     23537        	|       	|       16.399 s         	|
|   fl3795    |        28772          	|     30465        	|       	|       53.452 s         	|
|   fnl4461    |        182566          	|     185991        	|       	|       21.796 s         	|
|   gil262    |        2378          	|     2419        	|       	|       0.818 s         	|
|   gr96    |       55209           	|     56970        	|       	|       0.317 s         	|
|   gr137    |         69853         	|     74225        	|       	|       0.362 s         	|
|   gr202    |        40160          	|     41065        	|       	|       1.293 s         	|
|   gr229    |        134602          	|     135702        	|       	|       0.604 s         	|
|   gr431    |       171414           	|     174655        	|       	|       1.796 s         	|
|   gr666    |       294358           	|     302052        	|       	|       4.113 s         	|
|   kroA100    |        21282          	|     21825        	|       	|       0.244 s         	|
|   kroA150    |        26524          	|     26927        	|       	|       0.334 s         	|
|   kroA200    |         29368         	|     30012        	|       	|       0.497 s         	|
|   kroB100    |        22141          	|     22141        	|       	|       0.115 s         	|
|   kroB150    |        26130          	|     26182        	|       	|       0.331 s         	|
|   kroB200    |        29437          	|     29559        	|       	|       0.353 s         	|
|   kroC100    |        20749          	|     20852        	|       	|       0.261 s         	|
|   kroD100    |        21294          	|     21294        	|       	|       0.302 s         	|
|   kroE100    |        22068          	|     22160        	|       	|       0.178 s         	|
|   lin105    |        14379          	|     14819        	|       	|       0.459 s         	|
|   lin318    |        42029          	|     43311        	|       	|       1.346 s         	|
|   linhp318    |        41345          	|     43311        	|       	|       1.365 s         	|
|   nrw1379    |         56638         	|     57473        	|       	|       4.395 s         	|
|   p654    |        34643          	|     34666        	|       	|       7.059 s         	|
|   pcb442    |         50778         	|     52179        	|       	|       0.811 s         	|
|   pcb1173    |         56892         	|     58234        	|       	|       3.836 s         	|
|   pcb3038    |        137694          	|     141129        	|       	|       20.177 s         	|
|   pla7397    |        23260728          	|     24168995        	|       	|       99.168 s         	|
|   pla33810    |        66138592          	|     67631733        	|       	|       925.686 s         	|
|   pr76    |        108159          	|     108159        	|       	|       0.156 s         	|
|   pr107    |        44303          	|     44303        	|       	|       0.496 s         	|
|   pr124    |        59030          	|     60088        	|       	|       0.249 s         	|
|   pr136    |         96772         	|     99471        	|       	|       0.234 s         	|
|   pr144    |        58537          	|     58763        	|       	|       0.785 s         	|
|   pr152    |        73682          	|     73818        	|       	|       0.643 s         	|
|   pr226    |        80369          	|     80369        	|       	|       0.643 s         	|
|   pr264    |        49135          	|     49135        	|       	|       1.191 s         	|
|   pr299    |        48191          	|     48662        	|       	|       0.503 s         	|
|   pr439    |       107217           	|     108741        	|       	|       1.578 s         	|
|   pr1002    |       259045           	|     263891        	|       	|       3.28 s         	|
|   pr2392    |       378032           	|     390148        	|       	|       10.809 s         	|
|   rat99    |        1211          	|     1245        	|       	|       0.097 s         	|
|   rat195    |       2323           	|     2386        	|       	|       0.229 s         	|
|   rat575    |        6773          	|     6936        	|       	|       1.101 s         	|
|   rat783    |        8806          	|     8988        	|       	|       2.265 s         	|
|   rd100    |        7910          	|     8042        	|       	|       0.226 s         	|
|   rd400    |        15281          	|     15515        	|       	|       1.122 s         	|
|   rl1304    |        252948          	|     263787        	|       	|       4.567 s         	|
|   rl1323    |        270199          	|     278696        	|       	|       5.077 s         	|
|   rl1889    |        316536          	|     326360        	|       	|       11.443 s         	|
|   rl5915    |        565544          	|     580725        	|       	|       39.785 s         	|
|   rl5934    |        556050          	|     580254        	|       	|       52.556 s         	|
|   rl11849    |        923473          	|     945786        	|       	|       185.029 s         	|
|   st70    |        675          	|     682        	|       	|       0.089 s         	|
|   ts225    |        126643          	|     134721        	|       	|       0.337 s         	|
|   tsp225    |        3919          	|     4027        	|       	|       0.419 s         	|
|   u159    |        42080          	|     42727        	|       	|       0.255 s         	|
|   u574    |       36905           	|     37565        	|       	|       2.343 s         	|
|   u724    |       41910           	|     42754        	|       	|       1.539 s         	|
|   u1060    |        224094          	|     230376        	|       	|       6.174 s         	|
|   u1432    |        152970          	|     156796        	|       	|       3.075 s         	|
|   u1817    |        57201          	|     59157        	|       	|       9.659 s         	|
|   u2152    |        64253          	|     66640        	|       	|       8.8 s         	|
|   u2319    |        234256          	|     236054        	|       	|       4.24 s         	|
|   ulysses16    |         6859         	|     6929        	|       	|       0.019 s         	|
|   ulysses22    |        7013          	|     7162        	|       	|       0.057 s         	|
|   usa13509    |        20167722          	|     20529179        	|       	|       228.765 s         	|
|   vm1084    |        239297          	|     245569        	|       	|       4.756 s         	|
|   vm1748    |        336556          	|     347823        	|       	|       12.521 s         	|

## Acknowledgments
Inspiration, code snippets, etc.
* [An Effective Implementation of the Lin-Kernighan Traveling Salesman Heuristic](http://webhotel4.ruc.dk/~keld/research/LKH/LKH-2.0/DOC/LKH_REPORT.pdf)
* [Implementing Lin-Kernighan in Python](https://arthur.maheo.net/implementing-lin-kernighan-in-python/)
* [TSPLIB](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/)
* [TSPLIB95 documents](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/tsp95.pdf)
