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
|    a280   	|         2579         	|     2650     	|    1.027   	|     0.323 s    	|
|   ali535  	|         202310         	|    205825    	|    1.017   	|     2.903 s    	|
|   att48   	|         10628         	|     10711    	|    1.007   	|     0.104 s    	|
|   att532    |         27686         	|     28293        	|    1.022   	|       1.706 s         	|
|   berlin52    |        7542          	|     7542        	|    1   	|       0.064 s         	|
|   bier127    |          118282        	|     122967        	|    1.039   	|       0.696 s         	|
|   brd14051    |        469385          	|     479561        	|    1.022   	|       208.887 s         	|
|   burma14    |        3323          	|     3461        	|    1.042   	|       0.019 s         	|
|   ch130    |        6110          	|     6197        	|    1.014   	|       0.414 s         	|
|   ch150    |         6528         	|     6655        	|    1.019   	|       0.253 s         	|
|   d198    |         15780         	|     15818        	|    1.002   	|       1.993 s         	|
|   d493    |         35002         	|     35558        	|    1.016   	|       3.293 s         	|
|   d657    |         48912         	|     50247        	|    1.027   	|       2.67 s         	|
|   d1291    |         50801         	|     53180        	|    1.047   	|       8.642 s         	|
|   d1655    |         62128         	|     64900        	|    1.045   	|       8.363 s         	|
|   d2103    |         80450         	|     83021        	|    1.032   	|       8.839 s         	|
|   d15112    |         1573084         	|     1608776        	|    1.023   	|       245.01 s         	|
|   d18512    |         645238         	|     659005        	|    1.021   	|       383.11 s         	|
|   dsj1000    |        18659688         |     19192559     	|    1.029   	|       9.411 s         	|
|   eil51    |         426         	|     426        	|    1   	|       0.035 s         	|
|   eil76    |         538         	|     549        	|    1.020   	|       0.093 s         	|
|   eil101    |        629          	|     638        	|    1.014   	|       0.173 s         	|
|   fl417    |        11861          	|     11874        	|    1.001   	|       2.497 s         	|
|   fl1400    |        20127          	|     20485        	|    1.018   	|       22.443 s         	|
|   fl1577    |        22249          	|     23537        	|    1.058   	|       16.399 s         	|
|   fl3795    |        28772          	|     30465        	|    1.059   	|       53.452 s         	|
|   fnl4461    |        182566          	|     185991        	|   1.019    	|       21.796 s         	|
|   gil262    |        2378          	|     2419        	|    1.017   	|       0.818 s         	|
|   gr96    |       55209           	|     56970        	|    1.032   	|       0.317 s         	|
|   gr137    |         69853         	|     74225        	|    1.063   	|       0.362 s         	|
|   gr202    |        40160          	|     41065        	|    1.023   	|       1.293 s         	|
|   gr229    |        134602          	|     135702        	|    1.008   	|       0.604 s         	|
|   gr431    |       171414           	|     174655        	|    1.019   	|       1.796 s         	|
|   gr666    |       294358           	|     302052        	|    1.026   	|       4.113 s         	|
|   kroA100    |        21282          	|     21825        	|    1.026   	|       0.244 s         	|
|   kroA150    |        26524          	|     26927        	|    1.015   	|       0.334 s         	|
|   kroA200    |         29368         	|     30012        	|    1.022   	|       0.497 s         	|
|   kroB100    |        22141          	|     22141        	|    1   	|       0.115 s         	|
|   kroB150    |        26130          	|     26182        	|    1.002   	|       0.331 s         	|
|   kroB200    |        29437          	|     29559        	|    1.004   	|       0.353 s         	|
|   kroC100    |        20749          	|     20852        	|    1.005   	|       0.261 s         	|
|   kroD100    |        21294          	|     21294        	|    1   	|       0.302 s         	|
|   kroE100    |        22068          	|     22160        	|    1.004   	|       0.178 s         	|
|   lin105    |        14379          	|     14819        	|    1.031   	|       0.459 s         	|
|   lin318    |        42029          	|     43311        	|    1.031   	|       1.346 s         	|
|   linhp318    |        41345          	|     43311        	|    1.048   	|       1.365 s         	|
|   nrw1379    |         56638         	|     57473        	|    1.015   	|       4.395 s         	|
|   p654    |        34643          	|     34666        	|    1.001   	|       7.059 s         	|
|   pcb442    |         50778         	|     52179        	|    1.028   	|       0.811 s         	|
|   pcb1173    |         56892         	|     58234        	|    1.024   	|       3.836 s         	|
|   pcb3038    |        137694          	|     141129        	|    1.025   	|       20.177 s         	|
|   pla7397    |        23260728         	|     24168995        	|    1.040   	|       99.168 s         	|
|   pla33810    |        66048945          	|     67631733       	|    1.024   	|       925.686 s         	|
|   pr76    |        108159          	|     108159        	|    1   	|       0.156 s         	|
|   pr107    |        44303          	|     44303        	|    1   	|       0.496 s         	|
|   pr124    |        59030          	|     60088        	|    1.018   	|       0.249 s         	|
|   pr136    |         96772         	|     99471        	|    1.028   	|       0.234 s         	|
|   pr144    |        58537          	|     58763        	|    1.004   	|       0.785 s         	|
|   pr152    |        73682          	|     73818        	|    1.002   	|       0.643 s         	|
|   pr226    |        80369          	|     80369        	|    1   	|       0.643 s         	|
|   pr264    |        49135          	|     49135        	|    1   	|       1.191 s         	|
|   pr299    |        48191          	|     48662        	|    1.010   	|       0.503 s         	|
|   pr439    |       107217           	|     108741        	|    1.014   	|       1.578 s         	|
|   pr1002    |       259045           	|     263891        	|    1.019   	|       3.28 s         	|
|   pr2392    |       378032           	|     390148        	|    1.032   	|       10.809 s         	|
|   rat99    |        1211          	|     1245        	|    1.028   	|       0.097 s         	|
|   rat195    |       2323           	|     2386        	|    1.027   	|       0.229 s         	|
|   rat575    |        6773          	|     6936        	|    1.024   	|       1.101 s         	|
|   rat783    |        8806          	|     8988        	|    1.021   	|       2.265 s         	|
|   rd100    |        7910          	|     8042        	|    1.017   	|       0.226 s         	|
|   rd400    |        15281          	|     15515        	|    1.015   	|       1.122 s         	|
|   rl1304    |        252948          	|     263787        	|    1.043   	|       4.567 s         	|
|   rl1323    |        270199          	|     278696        	|    1.031   	|       5.077 s         	|
|   rl1889    |        316536          	|     326360        	|    1.031   	|       11.443 s         	|
|   rl5915    |        565530          	|     580725        	|    1.027   	|       39.785 s         	|
|   rl5934    |        556045          	|     580254        	|    1.044   	|       52.556 s         	|
|   rl11849    |        923288          	|     945786        	|    1.024   	|       185.029 s         	|
|   st70    |        675          	|     682        	|    1.010   	|       0.089 s         	|
|   ts225    |        126643          	|     134721        	|   1.064    	|       0.337 s         	|
|   tsp225    |        3919          	|     4027        	|    1.028   	|       0.419 s         	|
|   u159    |        42080          	|     42727        	|    1.015   	|       0.255 s         	|
|   u574    |       36905           	|     37565        	|    1.018   	|       2.343 s         	|
|   u724    |       41910           	|     42754        	|    1.020   	|       1.539 s         	|
|   u1060    |        224094          	|     230376        	|    1.028   	|       6.174 s         	|
|   u1432    |        152970          	|     156796        	|    1.025   	|       3.075 s         	|
|   u1817    |        57201          	|     59157        	|    1.034   	|       9.659 s         	|
|   u2152    |        64253          	|     66640        	|    1.037   	|       8.8 s         	|
|   u2319    |        234256          	|     236054        	|   1.008  	|       4.24 s         	|
|   ulysses16    |         6859         	|     6929        	|    1.010   	|       0.019 s         	|
|   ulysses22    |        7013          	|     7162        	|   1.021    	|       0.057 s         	|
|   usa13509    |        19982859          	|     20529179     |    1.027   	|       228.765 s         	|
|   vm1084    |        239297          	|     245569        	|    1.026   	|       4.756 s         	|
|   vm1748    |        336556          	|     347823        	|    1.033   	|       12.521 s         	|

## Acknowledgments
Inspiration, code snippets, etc.
* [An Effective Implementation of the Lin-Kernighan Traveling Salesman Heuristic](http://webhotel4.ruc.dk/~keld/research/LKH/LKH-2.0/DOC/LKH_REPORT.pdf)
* [Implementing Lin-Kernighan in Python](https://arthur.maheo.net/implementing-lin-kernighan-in-python/)
* [TSPLIB](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/)
* [TSPLIB95 documents](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/tsp95.pdf)
