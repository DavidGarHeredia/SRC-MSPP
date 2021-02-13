//
// Created by David Garcia-Heredia on 7/26/19.
//
// Description:
// This file contains the functions (except those that use Gurobi),
// employed to solve the problem through the heursitic. Functions using Gurobi
// are in file MathSolver.h
// The description of each function is in the cpp file

#ifndef NEWHEURISTIC_HEURISTICII_H
#define NEWHEURISTIC_HEURISTICII_H

#include "MathSolver.h"

//-----------------------------
// FUNCTIONS CONFORMING THE ALGORITHM
//-----------------------------

void heuristic_algorithm (Instance &instance, Input &input, jarvis::Jarvis &jarvisObj,
                          Output &output);

// for the pool of solutions
void get_feasible_solution (Instance &instance, Input &input,
                             jarvis::Jarvis &jarvisObj, Output &output, size_t nSol);

// local search
void local_search (Instance &instance, Input &input, jarvis::Jarvis &jarvisObj,
                   Output &output);


//-----------------------------
// AUXILIARY IMPORTANT FUNCTIONS
//-----------------------------

// to load output of heuristic to Jarvis so this info is later exported
void load_output_to_jarvis (Output &output, jarvis::Jarvis &jarvisObj, size_t nSols);

// to compute the lower bound cost
void compute_lb_cost(Instance &instance, jarvis::Jarvis &jarvisObj,Input &input);

// functions to use when generating the pool
float shortest_path_acyclic (t_node source, t_node sink, graph::Graph &g,
                             jarvis::Jarvis &jarvisObj, TempSolution &sol, size_t idx);

void shortest_path (Instance &instance, TempSolution &sol, jarvis::Jarvis &jarvisObj);

void shortest_path_penalized_nets (Instance &instance, TempSolution &sol,
        jarvis::Jarvis &jarvisObj, std::unordered_set<unsigned int> &setNetsPenalized);

bool penalize_arcs (Instance &instance, TempSolution &sol, Input &input,
                    std::unordered_set<unsigned int> &netsPenalized, size_t nSol,
                    unsigned int numIter);

void select_k_random_nets (std::unordered_set<unsigned int> &netsPenalized,
                           Input &input, Instance &instance);

void select_k_worst_nets (std::unordered_set<unsigned int> &netsPenalized,
                          Input &input, Instance &instance, TempSolution &sol);

void is_solution_optimal (jarvis::Jarvis &jarvisObj, Input &input, Output &output,
                          bool &optimalSol, float optCost);

float get_solution_cost (Instance &instance, TempSolution &sol);

void get_solution_bool (Output &output, TempSolution &sol, size_t nSol);

// functions to use in Local_Search
void sort_k_bad_nets (std::vector<std::pair<unsigned int, float> > &nets,
                        Input &input, Output &output, Instance &instance);

void sort_k_large_nets (std::vector<std::pair<unsigned int, float> > &nets,
                      Input &input, Instance &instance);

float get_cost_network (Instance &instance, size_t idx, Output &output);


//-----------------------------
// SECONDARY FUNCTIONS
//-----------------------------

void initialize_variables_pool(Instance &instance, Input &input, jarvis::Jarvis &jarvisObj,
                                TempSolution &sol);
void error_pointers_feasible_sol (jarvis::Jarvis &jarvisObj);
void error_pointers_shortest_path (jarvis::Jarvis &jarvisObj);
void error_no_path_in_shortest_path (jarvis::Jarvis &jarvisObj,int idx);
void show_shortest_path (TempSolution &sol);
void dummy_times (jarvis::Jarvis &jarvisObj);
void info_msg (jarvis::Jarvis &jarvisObj, const std::string &msg);

#endif //NEWHEURISTIC_HEURISTICII_H
