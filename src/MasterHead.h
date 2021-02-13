//
// Created by David Garcia-Heredia on 7/24/19.
//
// Description:
// This file contains the definition of all the structs employed in the problem,
// as well as the libraries (e.g: <string>) to use

#ifndef NEWHEURISTIC_MASTERHEAD_H
#define NEWHEURISTIC_MASTERHEAD_H

#include <jarvis/Jarvis.h>
#include <graph/Graph.h>
#include <gurobi_c++.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <list>
#include <map>
#include <omp.h>


//-----------------------------
// STRUCTS FOR THE DATA:
// INPUT, GRAPH & CONSTRAINTS
//-----------------------------

struct Input {
    // path to each graph folder
    std::vector<std::string> graphPath;

    // about the network
    size_t numArcs;

    // pool of feasible sol
    size_t numSolInHeuristic;
    int numThreads;
    int numIterAfterGurobi;
    float arcPenalization;
    bool useGurobi;
    float probCoin;
    float percToUseGurobi;
    float percExtraNets;
    size_t maxIterSearching;

    // local search
    float percBadNets;
    float percRelatedNets;

    // gurobi
    int numThreadsGurOpti;

    std::vector<std::string> rhsFolders;
    unsigned *pSeed;
};


struct Constraint {
    std::string name;
    unsigned int rhs;
    std::vector<unsigned int> nets;
    std::vector<std::vector<t_node> > arcs;
};


struct Instance {
    std::string name;
    float *pCost;
    std::vector<Constraint> constraints;
    std::vector<graph::Graph> gProjects;
    // lower bound costs
    float *pLB;
    float costLB;
};


//-----------------------------
// STRUCT TO USE IN THE POOL OF SOLUTIONS
//-----------------------------

struct TempSolution {
    bool *pPenalized;
    float *pCost;
    bool feasible = false;
    std::vector<std::unordered_set<t_node> > arcs;
};


//-----------------------------
// STRUCT TO SAVE THE OUTPUT
//-----------------------------

struct RefSet {
    std::vector<t_node> arcsSol;
    float Fobj;
    bool newElement = true;
    bool feasible;
};

// Output for the heuristic
struct Output {
    double fObjCombined = -1;
    double fObjLocalSearch = -1;
    // Pool of solutions
    unsigned int posBestSol = 0;
    std::vector<RefSet> refSet;
    float *pFobjPool;
    unsigned int *pNumIter;
    bool *pSol;
    char *status;
};

// Output when using exact methods
struct OutputGur {
    double fObj = -1;
    double timeGur = -1;
    double gap = -1;
    int numNodes = 0;
    int status;
};


#endif //NEWHEURISTIC_MASTERHEAD_H
