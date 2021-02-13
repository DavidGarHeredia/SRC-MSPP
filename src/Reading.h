//
// Created by David Garcia-Heredia on 7/5/19.
//
// DESCRIPTION:
// Functions to read all the data needed for the problem:
// 1) list of instances to solve (e.g.: Flight plan of 2019/Jan/16)
// 2) input of the algorithm (e.g: size of pool of solutions)
// 3) graph conforming the instance,
// 4) the right-hand sides for the problem

#ifndef NEWHEURISTIC_READING_H
#define NEWHEURISTIC_READING_H

#include "MasterHead.h"

//-----------------------------
// READING FUNCTIONS
//-----------------------------

int read_graph (Instance &instance, Input &input, size_t Inst_i,
                jarvis::Jarvis &jarvisObj);

void read_constraints (Instance &instance, Input &input, size_t Inst_i,
                       size_t rhs_j, jarvis::Jarvis &jarvisObj);


//-----------------------------
// SECONDARY FUNCTIONS
//-----------------------------

void dummy_check (Instance &instance, Input &input);
void print_info2(jarvis::Jarvis &jarvisObj, const std::string &msg);
void print_frame_title(const std::string &msg);
void print_frame_subtitle(const std::string &msg);
void error_pointers_seeds (jarvis::Jarvis &jarvisObj);
void export_arcs_solution (Output &output, size_t numArcs, jarvis::Jarvis &jarvisObj,
                            const std::string& nameMethod);
int prepare_pointers(Output &output, jarvis::Jarvis &jarvisObj, Input& input);
void initialize_seeds(jarvis::Jarvis &jarvisObj, Input& input);

#endif //NEWHEURISTIC_READING_H
