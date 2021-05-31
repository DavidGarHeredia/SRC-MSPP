# A Solution Method for the Shared Resource-Constrained Multi-Shortest Path Problem (SRMSPP)

In this repo you will find the code to reproduce the experiments in [1]. Before describing the code notice that:

1. The data sets employed in [1] can be download from [this link](https://mega.nz/file/HFdWRaSC#75TxZWam7IW1FkvNApv5JXu89kbdNjAX6uvUXYVSL50). From now on, every time I refer to a particular collection of data sets, I will mean these. **WARNING:** If you are going to download the data sets, be aware: the size of these, once decompressed, is about 50 GB.

2. The code solves the problem either by exact methods or using the matheuristic described in **Citation TBA**. In any case [Gurobi](https://www.gurobi.com/) is required, i.e.: you need to have this solver installed to compile and execute the code.

3. To compile the code, you will also need to have installed: a `C++` compiler, [Cmake](https://cmake.org/) and [OpenMP](https://www.openmp.org/).

4. When explaining the code for the matheuristic or the data sets employed, I will assume that you are minimally familiar with the content exposed in **(Citation TBA)**. That is, you know what you want to solve, how the data sets look like and what parameters conform the algorithm.


## Compiling the code

To compile the code *CMakeLists.txt* files are provided, so hopefully the code will work in different Operating System (OS). Here, I will show you how to compile it in Linux, but the instructions should be read regardless of the OS used. Steps:

1. First, we have to create two libraries that are used in the code. One is to work with graphs and the other to make easier (when coding) operations like creating time stamps, exporting outputs etc. To create them, go to directory *./lib/graph/build*, open a terminal and run the following commands:

```bash
cmake ..
make
```

2. Repeat the previous step but in *./lib/jarvis/build*.

3. If everything worked correctly, you are supposed to see 2 libraries in the *./lib* directory.

4. Now, before compiling the code to solve the SRC-MSPP, you need to slightly modify the *CMakeLists.txt*  file located at the root directory of the project, so your Gurobi version matches with the one specified there. For that, open the mentioned *CMakeLists.txt*. In lines 7 and 8 there are the two variables (*GUROBI* and *LGUROBI*) to be modified by the user. Modify them so they match with the location and version of your Gurobi library.

5. Finally, go to *./build* and run the same two commands specified in the 1st step. If everything worked, you are supposed to see a file called *executable* in the root directory. Execute it to solve the instances with the input provided (see inputs in the next section).

**Note 1:** The code uses relative paths to export results. Paths are specified using slash (e.g.: dir1/dir2/...). This works for Linux, but probably it will not in Windows. In such a case, I'm afraid you will have to change "/" to "\\\\" in the code. Particularly, you will find those issues in file *./src/main.cpp* in lines 19 to 22.


## Input for the problem

The input for the problem is read from directory *./data*. The structure of this directory (see when downloading the data sets) is as follows (see description below):

```bash
./data
+--- /problem_of_size_N1
+--- /problem_of_size_Nn
|    +--- /problem_1
|    +--- /problem_m
|         +--- problem_m.csv
|         +--- /rhs
|              +--- /difficult
|              +--- /medium
|              +--- /easy
|                   +--- rhs_1.csv
|                   +--- rhs_k.csv
```

**Note 2:** After you have downloaded the data sets, you will see some *txt* files inside the directories of the above structure (with the name of *instances.txt* and *input.txt*). Forget about them. They correspond to an old way of executing the experiments.

Inside each directory of type *problem_of_size_Nx*, you will find directories of type *problem_y*. These contain instances to be solved. For example, in the data from the link provided, you will find inside each directory of type *problem_of_size_Nx*, 3 directories, each of them corresponding to a different flight plan (details in **(Citation TBA)**).

Inside each directory of type *problem_y*, you will find 2 items:

1. *problem_m.csv*, a file listing the arcs forming the networks.

2. A directory called *rhs* with the capacity constraints. In the case of the data provided, 3 different types of rhs were tested (directories *difficult*, *medium* and *easy*).

## Executing the code and parameters of the algorithm

To execute the code, you have to call the executable with some parameters. And although most of the parameters are only used in the matheuristic, you have to pass them too when solving the problem using exact methods (the code expect the parameters in both cases).

The following is a valid execution of the code (see the explanation of the arguments afterwards):

```bash
./executable ./data/tinyVersion/Tiny2019Jan16/Tiny2019Jan16.csv ./data/tinyVersion/Tiny2019Jan16/rhs4/difficult/BLOdif90.csv Tiny2019Jan16 3116931 2 8 100 4 6000 0.5 1 0.05 0.05 20 0.02 0.1
```

The first two args indicate the file with the network (what we called *problem_m.csv* before) and the capacity constraints to use. So basically this defines the instance to be solved. The third arg is the name of the instance. Then the rest of the arguments are: 

+ The number of arcs of the network. This value is used to reserved memory for the pointers.

+ The number of threads to use by Gurobi when: 1) Solving the problem thought exact methods, or 2) Using Gurobi in the phases of combining solutions and local search (matheuristic).

+ The number of solutions to generate in the pool.

+ The maximum number of iterations to perform when searching for a feasible solution. Works as parameter *maxIter* in the paper.

+ The number of threads to use when generating the pool of solutions.

+ The cost penalty for arcs belonging to infeasible solutions. Works as parameter *penalty* in the paper.

+ The probability value for the coin flip. This is for the stochastic part of the algorithm. The larger the value, the more likely is to penalize the set of arcs using an exceeded resource. Works as parameter $\alpha$ in the paper.

+ If using Gurobi searching feasible solution. This is to allow or not the usage of Gurobi when generating the pool of solutions.

+ The percentage of networks from which to use Gurobi searching feasible sol. The larger this value is, the sooner Gurobi is used to try to reach feasibility when generating the pool of solutions. In the paper it is used 5%.

+ The percentage of extra networks to use when employing Gurobi searching feasible sol. The percentage for extra networks, besides the infeasible ones, to use when trying to reach feasibility with Gurobi in the pool. Works as 1-$\beta$ in the paper. For the computational experience presented there, $\beta$ = 95%.

+ The number of iters until using Gurobi again in feasible sol. The larger this value is, the more time has to pass between attempts of using Gurobi to reach feasibility when creating the pool of solutions. Works as parameter *minIterIP* in the paper.

+ The percentage of total bad networks. Proportion of bad networks to consider. Works as parameter $\delta$ in the paper.

+ The percentage of networks in the same constraint that bad networks. Works as parameter $\gamma$ in the paper.

Naturally, you usually don't want to pass the arguments by hand. Moreover, when more of the parameters are the same for all the instances. To launch executions I use the file *callable.jl*. It is just a simple script to perform this task. If you want to use it, you will have to install [Julia](https://julialang.org/). Obviously, you can also create your own script using Python or R instead of using mine.


## Solving the problem using exact methods

To solve the problem just using Gurobi and having a reference to compare the goodness of the algorithm, you have to do the following:

1. Open file *./src/main.cpp* and look for the comment "// SOLVING USING EXACT METHODS".

2. Modify (using comments) the next 5 lines so you end up with this:

```cpp
// SOLVING USING EXACT METHODS
nameMethod = "Gurobi";
solve_whole_problem_with_gurobi(instance, input, jarvisObj, output);

// SOLVING USING THE MATHEURISTIC
// heuristic_algorithm(instance, input, jarvisObj, output);
```

Now you just have to compile and execute the code.

## Solving the problem using the matheuristic

Similarly, to solve the problem using the matheuristic, you have to modify file *./src/main.cpp* so you end up with:

```cpp
// SOLVING USING EXACT METHODS
// nameMethod = "Gurobi";
// solve_whole_problem_with_gurobi(instance, input, jarvisObj, output);

// SOLVING USING THE MATHEURISTIC
heuristic_algorithm(instance, input, jarvisObj, output);
```

Then, you can compile and execute the code.

**Note 3:** All the functions that use Gurobi have time limits and gap limits established. We did not need the time limits for the computational experience of the paper, but we include them here just in case you need them. To change their values, open file *./src/MathSolver.cpp*, and search for *GRB_DoubleParam_TimeLimit* or *GRB_DoubleParam_MIPGap*.

**Note 4:** Make sure that all the instances that you intend to solve at once use the same pool size (parameter *numSolInHeuristic* in the Julia file). This is for not having mismatches in the columns exported by the algorithm in file *Results.csv* (more about output files in the next section).


## Output of the code

Once you have set your input and run the code, the following two files will be generated in the output directory: *Results.csv* and *ArcsInSol.csv*. Each row in either file is one of the instances solved (e.g.: *problem_m.csv* with *rhs_1.csv* will be one row, and *problem_m.csv* with *rhs_2.csv* another one). The first file contains all the information about the performance of the algorithm or the exact method, while the second one contains the arcs that form the solution for each instance.

About file *Results.csv*, note the following:

+ When solving the problem with the heuristic algorithm, it is also exported info about each solution in the pool. Particularly, its objective function value, the number of iterations required to obtain it, and its status (feasible or not). This is exported by columns, so the total number of columns referring to the pool is the Cartesian product of the number of solutions in the pool by 3. This is the easiest way to export the info of the pool, but not the best to analyze it. For that, you will probably need to pivot (long) that part of the table (e.g.: using `R`).

+ When the number of iterations to reach a feasible solution in the pool is equal to 0 it means that, after penalizing arcs (in the first iteration), the number of networks penalized was that small that the exact method was used and it reached a feasible solution.

+ Column *fObjLocalSearch* contains the value of the objective function at the end of the matheuristic algorithm.

+ When solving the problem by exact methods, columns *timeGur* and *totalTime* mean CPU and Wall time respectively.


About file *ArcsInSol.csv*: This file saves the arcs forming the solution of each instance as a string. The arcs in the string are separated by character "/", so these can easily be transformed afterwards into a numerical vector using a regular expression (most programming languages implement a *string split* function for this task).

**Note 5:** Both files, unless you change that in *./src/main.cpp*, will be exported in the *./output* directory. Therefore, between executions of the program, it is recommended to save the last output obtained in a different place. In case you forget about doing that, don't worry because the previous output will not be overwritten with the new one. The new one will be simply appended to the file. The only issue you may encounter is that mentioned before in **Note 4**.

## Extra information

The following information is only important if you want to test the code with different data sets. If you don't have that intention, you can skip this part.

**Notes** about files *problem_y.csv*: 1) The code assumes that **each** network inside this file is in topological order and that the source node is labeled as 0, and the sink one as N-1, where N is the number of nodes in that network. So for a network with 300 nodes, the source node will be labeled as 0, and the sink one as 299. 2) The order of the columns in the file is also important and it is: i) network (note that the first network is labeled as 0), ii) arc, iii) tail node, iv) head node, v) cost of using the arc, and vi) rest of the columns (if any). 3) The number of the arc doesn't start from 0 in each network. It is accumulative, that is, it goes from 0 to the number of arcs - 1.

**Note** about files *rhs_z.csv*: In case you want to create and test your own rhs files, you have to know what format to employ so the program can read the file. I warn you that the format is a little bit strange at first glance. Let's see it. In the first line of the file you have the following header: "Constraint, rhs, NetArcs". So the first column of the file will contain the name of the constraint, the second one the rhs value for the constraint (decimal point numbers will be round down in the program), and the 3rd one the duples {net, arc} forming the constraint. It is this latter one which has the strange format. Let me expose it with an example (see description below):

```
nameOfTheConstraint,2,|220/553042|+336/863700|+336/863715|+336/863717|+616/1701178|+648/1774528|*
```

The previous represents one row (constraint) in file *rhs_z.csv*. Note that in the constraint, the capacity is equal to 2, and it is formed by 6 arcs corresponding to 4 different networks: 220, 336, 616 and 648. Note the following structure: 1) after the capacity level and the comma we have a vertical separator "|". 2) Then, all the duples follow the same pattern: network, slash, arc and a vertical separator. If after that duple we want to add another one to the constraint, we put a "+" symbol before it. On the other hand, if a duple is the last one appearing in the constraint, we identify it using the multiplication symbol at the end. As I said, the format is a little bit strange, but once you see the pattern underneath, it can be reproduced without much difficulty.


## References
[1] García-Heredia, D., Molina, E., Laguna, M. & Alonso-Ayuso, A. A Solution Method for the Shared Resource-Constrained Multi-Shortest Path Problem. Accepted (May 2020) in Expert Systems With Applications.
