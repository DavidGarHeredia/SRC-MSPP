# A Solution Method for the Shortest Path Problem in Multiple Networks with Limited Shared Resources

In this repo you will find the code to solve the problem described in **(Citation TBA)**. Before describing the code notice that:

1. The data sets employed in **Citation TBA** can be download from <http://www.example.com> **PONER LINK**. From now on, every time I refer to a particular collection of data sets, I will mean these. **WARNING:** If you are going to download the data sets, be aware: the size of these, once decompressed, is about **50**GB.

2. The code solves the problem either by exact methods or using the matheuristic described in **Citation TBA**. In any case [Gurobi](https://www.gurobi.com/) is required, i.e.: you need to have this solver installed to compile and execute the code.

3. When explaining the code for the matheuristic or the data sets employed, I will assume that you are minimally familiar with the content exposed in **(Citation TBA)**. That is, you know what you want to solve, how the data sets look like and what parameters conform the algorithm.


## Compiling the code

**WARNING:** *OpenMP* is required!

To compile the code in any Operating System (OS), *CMakeLists.txt* files are provided. Here, I will show you how to use them in Linux. I recommend you to check the instructions regardless of your OS. Steps:

1. First, we have to create two libraries that are used in the code. One is to work with graphs and the other to make easier (when coding) operations like creating time stamps, exporting outputs etc. To create them, go to directory *./lib/graph/build*, open a terminal and run the following commands:

```bash
cmake ..
make
```

2. Repeat the previous step but in *./lib/jarvis/build*. Note: Check my GitHub account if you are interested in this library.

3. If everything worked correctly, you are supposed to see 2 libraries in the *./lib* directory.

4. Now, before compiling the code to solve the SRC-MSPP, you need to slightly modify the *CMakeLists.txt*  file located at the root directory of the project, so your Gurobi version matches with the one specified there. For that, open the mentioned *CMakeLists.txt*. In lines 7 and 8 there are the two variables (*GUROBI* and *LGUROBI*) to be modified by the user. Modify them so they match with the location and version of your Gurobi library.

5. Finally, go to *./build* and run the same two commands specified in the 1st step. If everything worked, you are supposed to see a file called *executable* in the root directory. Execute it to solve the problems with the input provided (see inputs in the next section).


## Input for the problem

The input for the problem is read from directory *./data*. The structure of this directory (see when downloading the data sets) is as follows (see description below):

```bash
./data
+--- instances.txt
+--- /problem_of_size_N1
+--- /problem_of_size_Nn
|    +--- /problem_1
|    +--- /problem_m
|         +--- input.txt
|         +--- problem_m.csv
|         +--- /rhs
|              +--- /difficult
|              +--- /medium
|              +--- /easy
|                   +--- rhs_1.csv
|                   +--- rhs_k.csv
```


Inside directory *./data* you have: 1) a collection of directories, each of them containing different problems of similar size; and 2) *instances.txt*, a file listing the problems you want to solve. **Important:** do not change the name of this file (i.e.: "instances.txt") for the code to work.

**Note:** Due to the size of the problems in <http://www.example.com> **PONER LINK**, I don't recommend you to solve all the cases at once, but one size-group at a time, i.e.: solve all the problems in *problem_of_size_N1* at once, then all the problems in *problem_of_size_N2*, etc. Note that when executing the code, the output of the problem is exported at the end, so if you decide to solve all the problems at once and something goes wrong (e.g.: you run out of memory), you will have spent a lot of time in vain.

Inside each directory of type *problem_of_size_Nx*, you will find directories of type *problem_y*. These contain data of a problem to be solved. For example, in the data from the link provided, you will find inside each directory of type *problem_of_size_Nx*, 3 directories, each of them corresponding to a different flight plan (details in **(Citation TBA)**).

Inside each directory of type *problem_y*, you will find 3 items:

1. *input.txt*, a file with: 1) The input to use in the algorithm when solving *problem_y*, and 2) A list of the right-hand sides (rhs) to use, so you can test the algorithm under different scenarios. Details of this file (the meaning of the parameters) are discussed in *README2.md*. **Important:** do not change the name of this file (i.e.: "input.txt") for the code to work.

2. *problem_m.csv*, a file listing the arcs forming the networks. See important notes about this file after the enumeration list.

3. A directory called *rhs* with the capacity constraints. Note that this directory, for our data sets, is redundant in the sense that, the content inside it (directories *difficult*, *medium* and *easy*) could be inside *./problem_m* instead. So, why did I make directory *rhs*? Easy, because those rhs were created with a particular logic (see in  **(Citation TBA)**). If you want to create some different ones, you will like to have those grouped and separated from mines.

**Notes** about files *problem_y.csv*: 1) The code assumes that **each** network inside this file is in topological order and that the source node is labeled as 0, and the sink one as N, where N is the number of nodes in that network - 1. So for a network with 300 nodes, the source node will be labeled as 0, and the sink one as 299. This is important if you want to test the algorithm with your own data. 2) The order of the columns in the file is also important and it is: i) net (note that the first net is labeled as 0), ii) arc, iii) tail, iv) head, v) cost of using the arc, and vi) rest of the columns (if any). 3) The number of the arc doesn't start from 0 in each network. It is accumulative, that is, it goes from 0 to the number of arcs - 1.

**Note** about files *rhs_z.csv*: In case you want to create and test your own rhs files, you have to know the format to employ so the program can read the file. I warn you that the format is a little bit strange at first glance. Let's see it. In the first line of the file you have the following header: "Constraint, rhs, NetArcs". So the first column of the file will contain the name of the constraint, the second one the rhs value for the constraint (decimal point numbers will be round down in the program), and the 3rd one the duples {net, arc} forming the constraint. It is this latter one which has the strange format. Let me expose it with an example (see description below):

```
nameOfTheConstraint,2,|220/553042|+336/863700|+336/863715|+336/863717|+616/1701178|+648/1774528|*
```

The previous represents one row (constraint) in file *rhs_z.csv*. Note that in the constraint, the capacity is equal to 2, and it is formed by 6 arcs corresponding to 4 different networks: 220, 336, 616 and 648. Note the following structure: 1) after the capacity level and the comma we have a vertical separator "|". 2) Then, all the duples follow the same pattern: network, slash, arc and a vertical separator. If after that duple we want to add another one to the constraint, we put a "+" symbol before it. On the other hand, if a duple is the last one appearing in the constraint, we identify it using the multiplication symbol at the end. As I said, the format is weird (I don't remember how I came up with this crazy idea), but once you see the pattern underneath, it can be reproduced without much difficulty.

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

Similarly to solving the problem with exact methods, you have to modify file *./src/main.cpp* so you end up with:

```cpp
// SOLVING USING EXACT METHODS
// nameMethod = "Gurobi";
// solve_whole_problem_with_gurobi(instance, input, jarvisObj, output);

// SOLVING USING THE MATHEURISTIC
heuristic_algorithm(instance, input, jarvisObj, output);
```

Then, you can compile and execute the code.

**Note:** All the functions that use Gurobi have time limits and gap limits established. We did not need the time limits for the computational experience of the paper, but we include them here just in case you need them. To change their values, open file *./src/MathSolver.cpp*, search for *GRB_DoubleParam_TimeLimit* or *GRB_DoubleParam_MIPGap*.

**Note:** When solving problems with the algorithm, for all the problems listed in *instances.txt* (i.e.: those that you intend to solve in the same execution), make sure that all of them use the same pool size (parameter to set in *input.txt*). This is for not having mismatches in the columns exported by the algorithm in file *Results.csv* (more about output files in the next subsection).


## Output of the code

Once you have set your input and run the code, the following two files will be generated: *Results.csv* and *ArcsInSol.csv*. Each row in either file is one of the instances solved (e.g.: *problem_m.csv* with *rhs_1.csv* will be one row and *problem_m.csv* with *rhs_2.csv* another one). The first file contains all the information about the performance of the algorithm or the exact method, while the second one contains the arcs that form the solution for each instance.

About file *Results.csv*, note the following:

+ When solving the problem with the heuristic algorithm, it is also exported info about each solution in the pool, particularly, its objective function value, the number of iterations required to obtain it, and its status (feasible or not). This is exported by columns, so the total number of columns referring to the pool is the Cartesian product of the number of solutions in the pool by 3. This is the easiest way to export the info of the pool, but not the best to analyze it. For that, you will probably need to pivot (long) that part of the table (e.g.: using `R`).

+ When the number of iterations to reach a feasible solution in the pool is equal to 0 it means that, after penalizing arcs (in the first iteration), the number of networks penalized was that small that the exact method was used and it reached a feasible solution.

+ Column *fObjLocalSearch* represents the value of the objective function for the problem at the end of the matheuristic algorithm.

+ When solving the problem by exact methods, columns *timeGur* and *totalTime* mean CPU and Wall time respectively.


About file *ArcsInSol.csv*: This file saves the arcs forming the solution of each problem as a string. The arcs in the string are separated by character "/", so these can easily be transformed into a numerical vector using a regular expression (e.g.: in `R`).

**Note:** Both files, unless you change that in *./src/main.cpp*, will be exported in the *./output* directory. Therefore, between executions of the program, it is recommended to save the last output obtained in a different place. In case you forget about doing that, don't worry because the previous output will not be overwritten with the new one. The new one will be simply appended to the file, but with a new header. This means that the next time you open any of the output files, you will have multiple headers along the file. For *Results.csv* this is not a problem, you can simply use your favorite Spreed Sheet editor to delete the extra headers. However, for *ArcsInSol.csv*, due to the length of the string containing the arcs in the solution, you will probably need to use a programming language or a text editor like `Vim` to open the file and delete the extra headers.


## References

**(Citation TBA)**
