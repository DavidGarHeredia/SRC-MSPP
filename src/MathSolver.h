//
// Created by David Garcia-Heredia on 7/29/19.
//
// Description:
// This file contains all the functions that have calls to Gurobi, as well as
// the functions required to create the problems, save the solution etc.
// The description of each function is in the cpp file

#ifndef NEWHEURISTIC_MATHSOLVER_H
#define NEWHEURISTIC_MATHSOLVER_H


#include "MasterHead.h"

void solve_whole_problem_with_gurobi (Instance &instance, Input &input,
                                        jarvis::Jarvis &jarvisObj, Output &output);

bool feasible_with_gurobi (Instance &instance, TempSolution &sol, Input &input,
                          std::unordered_set<unsigned int> &netsPenalized,
                          jarvis::Jarvis &jarvisObj);

int combine_solutions (Output &output, jarvis::Jarvis &jarvisObj,
                        Instance &instance, Input &input);


void gurobi_local_search(Instance &instance, Input &input, Output &output,
                         std::unordered_set<unsigned int> &netsPenalized,
                         jarvis::Jarvis &jarvisObj);

 
void fill_hash_table (std::unordered_set<unsigned int> &netsPenalized,
                      Instance &instance, GRBVar*& variable,
                      std::unordered_map<t_node, t_node> &varsHash);

size_t get_num_variables (std::unordered_set<unsigned int> &netsPenalized,
                          Instance &instance);

int initialize_elements_constraint (size_t tam, std::string msg, double*& coeffs,
                                    GRBVar*& vars, jarvis::Jarvis &jarvisObj);

void error_in_hash_table (jarvis::Jarvis &jarvisObj, std::string msg);

void error_getting_solution (jarvis::Jarvis &jarvisObj, std::string msg);

void print_info(jarvis::Jarvis &jarvisObj, const std::string &msg);

#endif //NEWHEURISTIC_MATHSOLVER_H
