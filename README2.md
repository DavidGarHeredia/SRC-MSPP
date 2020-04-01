# Parameters of input.txt

In file *input.txt* you will find the parameters listed below. To understand the description of all of them, it is recommended to first read the algorithm described in **(Citation TBA)**.

+ *nameInstance*: contains the name of the network inside the directory. E.g.: inside directory */problem_m*, the file with the networks is: *problem_m.csv*, so we write it without the *.csv* extension, i.e.: *problem_m*.

+ *numArcs*: number of arcs of the network. This value is used to reserved memory for the pointers.

+ *numThreadsGurOpti*: number of threads to use by Gurobi when: 1) Solving the problem thought exact methods, or 2) Using Gurobi in the phases of combining solutions and local search (matheuristic).

+ *numSolInHeuristic*: number of solutions to generate in the pool.

+ *maxIterSearching*: maximum number of iterations to perform when searching for a feasible solution.

+ *numThreads*: number of threads to use when generating the pool of solutions.

+ *arcPenalization*: cost penalization to used when generating the pool of solutions.

+ *probability value for the coin flip*: this is for the stochastic part of the algorithm. The larger the value, the more likely is to penalize the set of arcs using an exceeded resource. Works as parameter $\alpha$ in the paper.

+ *use Gurobi searching feasible solution?*: this is to allow or not the usage of Gurobi when generating the pool of solutions.

+ *percentage of nets from which to use Gurobi searching feasible sol*: The larger this value is, the sooner Gurobi is used to try to reach feasibility when generating the pool of solutions. Works as parameter *maxNets* in the paper.

+ *percentage of extra nets to use when employing Gurobi searching feasible sol*: The percentage for extra nets, besides the infeasible ones, to use when trying to reach feasibility with Gurobi in the pool.

+ *number of iters until using Gurobi again in feasible sol*: The larger this value is, the more time has to pass between attempts of using Gurobi to reach feasibility when creating the pool of solutions. Works as parameter *minIterIP* in the paper.

+ *percentage of total bad nets*: proportion of bad nets to consider. Works as parameter $\beta$ in the paper.

+ *percentage of nets in the same constraint that bad nets*: Works as parameter $\gamma$ in the paper.

After setting all these parameters, you list the rhs files for which you want to solve the problem. Note the format of the path and the non-usage of the `csv` extension.

```
/--------------- rhsFolders ---------------/
rhs/difficult/rhs_1
rhs/difficult/rhsBLOstyle
rhs/easy/rhs1
```
