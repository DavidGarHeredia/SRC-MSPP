//
// Created by David Garcia-Heredia on 7/29/19.
//

#include "MathSolver.h"

//=================================
// FOR THE WHOLE PROBLEM
//=================================

void fill_variables (Instance &instance, GRBVar*& variable) {
    t_node nArc = 0;

    std::vector<graph::Node>::iterator itNode, endNode;
    std::vector<graph::NodeElement>::iterator itArc, endArc;

    for (auto &g : instance.gProjects) {
        itNode  = g.get_iteratorToNodes();
        endNode = g.get_endToNodes();
        for (; itNode != endNode; itNode++) {
            itArc  = itNode->get_iterator_headNode();
            endArc = itNode->get_endIt_headNode();
            for (; itArc != endArc; itArc++) {
                nArc = itArc->nArc;
                variable[nArc].set(GRB_DoubleAttr_Obj, *(instance.pCost + nArc));
            }
        }
    }
}


void fill_variables_lp (Instance &instance, GRBVar*& variable) {
    t_node nArc = 0;

    std::vector<graph::Node>::iterator itNode, endNode;
    std::vector<graph::NodeElement>::iterator itArc, endArc;

    for (auto &g : instance.gProjects) {
        itNode  = g.get_iteratorToNodes();
        endNode = g.get_endToNodes();
        for (; itNode != endNode; itNode++) {
            itArc  = itNode->get_iterator_headNode();
            endArc = itNode->get_endIt_headNode();
            for (; itArc != endArc; itArc++) {
                nArc = itArc->nArc;
                variable[nArc].set(GRB_DoubleAttr_Obj, *(instance.pCost + nArc));
                variable[nArc].set(GRB_DoubleAttr_UB, 1);
                variable[nArc].set(GRB_DoubleAttr_LB, 0);
            }
        }
    }
}


int create_flow_eq_whole_problem (Instance &instance, GRBModel& model,
                                GRBVar*& variable, jarvis::Jarvis &jarvisObj) {

    std::vector<graph::Node>::iterator itNode, endNode;
    std::vector<graph::NodeElement>::iterator itArc, endArc;

    size_t tam, nodeSize;
    int rhs, count, pos;
    GRBLinExpr expr = 0;

    for (auto &g : instance.gProjects) {
        itNode   = g.get_iteratorToNodes();
        endNode  = g.get_endToNodes();
        nodeSize = g.get_numNodes();
        count = 0;
        for (; itNode != endNode; itNode++) {
            expr = 0, pos = 0;
            tam = itNode->get_num_headNode() + itNode->get_num_tailNode();
            rhs = (count == 0) ? (1) : ((count == nodeSize -1) ? (-1) : (0));

            double *coeffs;
            GRBVar *vars;
            initialize_elements_constraint (tam, "flow eq()", coeffs, vars, jarvisObj);

            // OUT GOING NODES
            std::fill(coeffs, coeffs + itNode->get_num_headNode(), 1);
            itArc = itNode->get_iterator_headNode(), endArc  = itNode->get_endIt_headNode();
            for (; itArc != endArc; itArc++) {
                vars[pos] = variable[itArc->nArc];
                pos++;
            }

            // IN GOING NODES
            std::fill(coeffs + itNode->get_num_headNode(), coeffs + tam, -1);
            itArc = itNode->get_iterator_tailNode(), endArc  = itNode->get_endIt_tailNode();
            for (; itArc != endArc; itArc++) {
                vars[pos] = variable[itArc->nArc];
                pos++;
            }

            // CREATE CONSTRAINT
            expr.addTerms(coeffs, vars, tam);
            model.addConstr(expr, GRB_EQUAL, rhs);

            delete[] coeffs;
            delete[] vars;
            count++;
        }
    }
    return 0;
}


void create_rhs_constraints_whole_problem (Instance &instance, GRBModel& model,
        jarvis::Jarvis &jarvisObj, GRBVar*& variable) {

    unsigned int pos;
    size_t tam;
    GRBLinExpr expr = 0;

    for (auto &itC : instance.constraints) {

        tam = 0;
        for (auto &arcs : itC.arcs)
            tam += arcs.size();

        // CREATE THE CONSTRAINT
        expr = 0;
        double *coeffs;
        GRBVar *vars;

        initialize_elements_constraint (tam, "constraint eq()",
                                        coeffs, vars, jarvisObj);

        std::fill(coeffs, coeffs + tam, 1);

        // variables
        pos = 0;
        for (auto & arc : itC.arcs)  {
            for (auto &a : arc) {
                vars[pos] = variable[a];
                pos++;
            }
        }

        expr.addTerms(coeffs, vars, tam);
        model.addConstr(expr, GRB_LESS_EQUAL, itC.rhs);

        delete[] coeffs;
        delete[] vars;
    }
}


float save_solution_whole_problem (Instance &instance, GRBVar*& variable, Output &output) {

    t_node nArc = 0;
    float cost = 0.0;

    std::vector<graph::Node>::iterator itNode, endNode;
    std::vector<graph::NodeElement>::iterator itArc, endArc;

    for (auto &g : instance.gProjects) {
        itNode  = g.get_iteratorToNodes();
        endNode = g.get_endToNodes();
        for (; itNode != endNode; itNode++) {
            itArc  = itNode->get_iterator_headNode();
            endArc = itNode->get_endIt_headNode();
            for (; itArc != endArc; itArc++) {
                nArc = itArc->nArc;
                if (variable[nArc].get(GRB_DoubleAttr_X) > 0.99) {
//                    cost += *(instance.pCost + nArc);
                    *(output.pSol + nArc) = true;
                }
            }
        }
    }

    return cost;

}


void load_output_exact_to_jarvis (OutputGur &out, jarvis::Jarvis &jarvisObj) {

    double time;
    std::string nameT = jarvisObj.tableNames[0];
    time = jarvisObj.timer.get_difTime(jarvis::Timer::seconds, "gurobi");
    std::cout << "Time Jarvis = " << time << "\n";

    jarvisObj.dictResults[nameT].back().add_result_numeric("fObj", out.fObj);
    jarvisObj.dictResults[nameT].back().add_result_numeric("timeGur", out.timeGur);
    jarvisObj.dictResults[nameT].back().add_result_numeric("totalTime", time);
    jarvisObj.dictResults[nameT].back().add_result_numeric("gap", out.gap);
    jarvisObj.dictResults[nameT].back().add_result_numeric("numNodes", out.numNodes);
    jarvisObj.dictResults[nameT].back().add_result_numeric("status", out.status);

}


void solve_whole_problem_with_gurobi (Instance &instance, Input &input,
                                    jarvis::Jarvis &jarvisObj, Output &output) {

    std::cout << "\n";
    print_info(jarvisObj, "GUROBI");
    std::cout << "\n";

    // BASICS OF THE MODEL
    jarvisObj.timer.start_time("gurobi");
    OutputGur out;
    std::cout << "Creating the model \n";
    GRBEnv env = GRBEnv("logFileGurobi.log");
    GRBModel model = GRBModel(env);
    model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
    model.set(GRB_DoubleParam_TimeLimit, 14400);
    model.set(GRB_DoubleParam_MIPGap, 0.005);
    model.set(GRB_IntParam_Threads, input.numThreadsGurOpti);
    jarvisObj.dictResults[jarvisObj.tableNames[0]].back().add_result_numeric("nThreads", input.numThreadsGurOpti);
    model.set(GRB_IntParam_LogToConsole, 1);

    // VARIABLES
    std::cout << "Creating the variables \n";
    GRBVar* variable = model.addVars(input.numArcs, GRB_BINARY);
    fill_variables (instance, variable);

    // Changing the previous 2 lines by the 2 below should solve the LP instead of the IP
    // Note: The solution export when solving the LP will be wrong because
    // only variables such that x > 0.99 are exported (this works for the IP)
    // GRBVar* variable = model.addVars(input.numArcs, GRB_CONTINUOUS);
    // fill_variables_lp(instance, variable);

    // CONSTRAINTS
    std::cout << "Creating flow constraints \n";
    create_flow_eq_whole_problem(instance, model, variable, jarvisObj);
    model.update();

    std::cout << "Creating capacity constraints \n";
    create_rhs_constraints_whole_problem(instance, model, jarvisObj, variable);
    model.update();

    // SOLVING THE MODEL
    std::cout << "Solving the model \n";
    model.optimize();

    // SAVING SOLUTION
    std::cout << "Saving solution \n";
    out.status = model.get(GRB_IntAttr_Status);
    out.timeGur = model.get(GRB_DoubleAttr_Runtime);
    jarvisObj.timer.end_time("gurobi");

    if (out.status != GRB_INFEASIBLE) {
        try {
            out.fObj = model.get(GRB_DoubleAttr_ObjVal);
            out.gap = model.get(GRB_DoubleAttr_MIPGap);
            out.numNodes = model.get(GRB_DoubleAttr_NodeCount);
            save_solution_whole_problem(instance, variable, output);
        } catch (...) {
            error_getting_solution (jarvisObj, "In solve_whole_problem_with_gurobi(), ");
        }
    }

    load_output_exact_to_jarvis(out, jarvisObj);

    delete [] variable;

}






//=================================
// FOR THE POOL OF SOLUTIONS
//=================================


int create_flow_eq (std::unordered_set<unsigned int> &netsPenalized,
                     Instance &instance, std::unordered_map<t_node, t_node> &varsHash,
                     GRBModel& model, GRBVar*& variable, jarvis::Jarvis &jarvisObj) {

    std::vector<graph::Node>::iterator itNode, endNode;
    std::vector<graph::NodeElement>::iterator itArc, endArc;
    std::unordered_map<t_node, t_node>::iterator itHash;

    size_t tam, nodeSize;
    int rhs, count, pos;
    GRBLinExpr expr = 0;

    auto itNet = netsPenalized.begin();
    for (; itNet != netsPenalized.end(); itNet++) {
        itNode   = instance.gProjects[*itNet].get_iteratorToNodes();
        endNode  = instance.gProjects[*itNet].get_endToNodes();
        nodeSize = instance.gProjects[*itNet].get_numNodes();
        count = 0;
        for (; itNode != endNode; itNode++) {
            expr = 0, pos = 0;
            tam = itNode->get_num_headNode() + itNode->get_num_tailNode();
            rhs = (count == 0) ? (1) : ((count == nodeSize -1) ? (-1) : (0));

            double *coeffs;
            GRBVar *vars;
            initialize_elements_constraint (tam, "flow eq()", coeffs, vars, jarvisObj);

            // OUT GOING NODES
            std::fill(coeffs, coeffs + itNode->get_num_headNode(), 1);
            itArc = itNode->get_iterator_headNode(), endArc  = itNode->get_endIt_headNode();
            for (; itArc != endArc; itArc++) {
                itHash = varsHash.find(itArc->nArc);
                vars[pos] = variable[itHash->second];
                pos++;
            }

            // IN GOING NODES
            std::fill(coeffs + itNode->get_num_headNode(), coeffs + tam, -1);
            itArc = itNode->get_iterator_tailNode(), endArc  = itNode->get_endIt_tailNode();
            for (; itArc != endArc; itArc++) {
                itHash = varsHash.find(itArc->nArc);
                vars[pos] = variable[itHash->second];
                pos++;
            }

            // CREATE CONSTRAINT
            expr.addTerms(coeffs, vars, tam);
            model.addConstr(expr, GRB_EQUAL, rhs);

            delete[] coeffs;
            delete[] vars;
            count++;
        }
    }
    return 0;
}


bool create_rhs_constraints (Instance &instance, TempSolution &sol, GRBModel& model,
                             std::unordered_set<unsigned int> &netsPenalized,
                             jarvis::Jarvis &jarvisObj, GRBVar*& variable,
                             std::unordered_map<t_node, t_node> &varsHash) {

    size_t idxN;
    unsigned int pos;
    int auxRhs;
    auto itSet = sol.arcs[0].begin();
    std::unordered_set<unsigned int>::iterator itNGur;
    std::unordered_map<t_node, t_node>::iterator itHash;
    std::vector<t_node> arcs;

    GRBLinExpr expr = 0;

    for (auto &itC : instance.constraints) {
        auxRhs = itC.rhs;
        idxN = 0;
        arcs.clear();

        // SEARCH THE INFO NEEDED TO CREATE THE CONSTRAINT
        for (auto &itN : itC.nets) {
            itNGur = netsPenalized.find(itN);
            if (itNGur == netsPenalized.end()) { // net to fix: update rhs
                for (auto &itArc : itC.arcs[idxN]) {
                    itSet = sol.arcs[itN].find(itArc);
                    if (itSet != sol.arcs[itN].end()) {
                        auxRhs--;
                        break; // just one can be in the solution
                    }
                }
            } else { // net for Gurobi: save variables
                for (auto &itArc : itC.arcs[idxN])
                    arcs.emplace_back(itArc);
            }
            idxN++;
        }

        if (auxRhs < 0 ) {
            return false; // no feasible solution exists
        }

        // CREATE THE CONSTRAINT
        expr = 0;
        double *coeffs;
        GRBVar *vars;

        initialize_elements_constraint (arcs.size(), "constraint eq()",
                coeffs, vars, jarvisObj);

        std::fill(coeffs, coeffs + arcs.size(), 1);

        // variables
        pos = 0;
        for (auto &a : arcs) {
            itHash = varsHash.find(a);
            vars[pos] = variable[itHash->second];
            pos++;
        }

        expr.addTerms(coeffs, vars, arcs.size());
        model.addConstr(expr, GRB_LESS_EQUAL, auxRhs);

        delete[] coeffs;
        delete[] vars;
    }

    return true; // feasible solution might exist
}


void save_solution (TempSolution &sol, Instance &instance, GRBVar*& variable,
                    std::unordered_set<unsigned int> &netsPenalized) {

    t_node nArc = 0;

    std::vector<graph::Node>::iterator itNode, endNode;
    std::vector<graph::NodeElement>::iterator itArc, endArc;

    auto itNet = netsPenalized.begin();
    for (; itNet != netsPenalized.end(); itNet++) {
        itNode  = instance.gProjects[*itNet].get_iteratorToNodes();
        endNode = instance.gProjects[*itNet].get_endToNodes();
        sol.arcs[*itNet].clear();
        for (; itNode != endNode; itNode++) {
            itArc  = itNode->get_iterator_headNode();
            endArc = itNode->get_endIt_headNode();
            for (; itArc != endArc; itArc++) {
                if (variable[nArc].get(GRB_DoubleAttr_X) > 0.99)
                    sol.arcs[*itNet].emplace(itArc->nArc);
                nArc++;
            }
        }
    }

}


bool feasible_with_gurobi(Instance &instance, TempSolution &sol, Input &input,
                          std::unordered_set<unsigned int> &netsPenalized,
                          jarvis::Jarvis &jarvisObj) {

    // BASICS OF THE MODEL
    bool solFeasible = true;
    GRBEnv env = GRBEnv();
    GRBModel model = GRBModel(env);
    model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
    model.set(GRB_DoubleParam_TimeLimit, 600);
    model.set(GRB_DoubleParam_MIPGap, 0.01);
    model.set(GRB_IntParam_LogToConsole, 0);

    // VARIABLES
    size_t numVar = get_num_variables(netsPenalized, instance);
    GRBVar* variable = model.addVars(numVar, GRB_BINARY);

    std::unordered_map<t_node, t_node> varsHash;
    varsHash.reserve(numVar);
    fill_hash_table(netsPenalized, instance, variable, varsHash);

    // CONSTRAINTS
    create_flow_eq(netsPenalized, instance, varsHash, model,
                   variable, jarvisObj);

    solFeasible = create_rhs_constraints(instance, sol, model, netsPenalized,
                                        jarvisObj, variable, varsHash);

    // SOLVING THE MODEL
    if (!solFeasible) {
        #pragma omp critical
        std::cout << "Thread = " << omp_get_thread_num()
                  << " Solution infeasible with gurobi because rhs < 0 \n";
        return false;
    }

    model.update();
    model.optimize();

    // SAVING SOLUTION
    int status = model.get(GRB_IntAttr_Status);

    if (status == GRB_INFEASIBLE) {
        #pragma omp critical
        std::cout << "Thread = " << omp_get_thread_num()
                  << " Solution infeasible with gurobi \n";
        solFeasible = false;
    } else {
        try {
            save_solution(sol, instance, variable, netsPenalized);
        } catch (...) {
            error_getting_solution (jarvisObj, "In feasible_with_gurobi(), ");
        }
    }

    delete [] variable;

    return solFeasible;

}



//=================================
// TO COMBINE SOLUTIONS
//=================================

void save_solution_of_combination (Instance &instance, GRBVar*& variable, Output &output,
                                   std::unordered_map<t_node, t_node> &varsHash) {

    // In this function, only the arcs in the sol remain with value TRUE

    t_node nArc = 0;
    std::unordered_map<t_node, t_node>::iterator it;

    std::vector<graph::Node>::iterator itNode, endNode;
    std::vector<graph::NodeElement>::iterator itArc, endArc;

    for (auto &g : instance.gProjects) {
        itNode  = g.get_iteratorToNodes();
        endNode = g.get_endToNodes();
        for (; itNode != endNode; itNode++) {
            itArc  = itNode->get_iterator_headNode();
            endArc = itNode->get_endIt_headNode();
            for (; itArc != endArc; itArc++) {
                nArc = itArc->nArc;
                if (*(output.pSol + nArc)) { // the arc was considered
                    it = varsHash.find(nArc);
                    if (variable[it->second].get(GRB_DoubleAttr_X) < 0.99) {
                        // arc not in the solution
                        *(output.pSol + nArc) = false;
                    }
                }
            }
        }
    }
}

void create_rhs_constraints_combination (Instance &instance, GRBModel& model,
                            Output &output, jarvis::Jarvis &jarvisObj, GRBVar*& variable,
                             std::unordered_map<t_node, t_node> &varsHash) {

    size_t idxN;
    unsigned int pos;
    std::unordered_map<t_node, t_node>::iterator itHash;
    std::vector<t_node> arcs;

    GRBLinExpr expr = 0;

    for (auto &itC : instance.constraints) {
        idxN = 0;
        arcs.clear();

        // SEARCH THE INFO NEEDED TO CREATE THE CONSTRAINT
        for (auto &itN : itC.nets) {
            for (auto &itArc : itC.arcs[idxN])
                if (*(output.pSol + itArc))
                    arcs.emplace_back(itArc);
            idxN++;
        }

        // CREATE THE CONSTRAINT
        expr = 0;
        double *coeffs;
        GRBVar *vars;

        initialize_elements_constraint (arcs.size(), "constraint eq()",
                                        coeffs, vars, jarvisObj);

        std::fill(coeffs, coeffs + arcs.size(), 1);

        // variables
        pos = 0;
        for (auto &a : arcs) {
            itHash = varsHash.find(a);
            vars[pos] = variable[itHash->second];
            pos++;
        }

        expr.addTerms(coeffs, vars, arcs.size());
        model.addConstr(expr, GRB_LESS_EQUAL, itC.rhs);

        delete[] coeffs;
        delete[] vars;
    }
}


void get_tam_combine (size_t &tamOut, size_t &tamIn, Output &output,
                      std::vector<graph::Node>::iterator &itNode) {

    std::vector<graph::NodeElement>::iterator itArc, endArc;
    tamOut = 0, tamIn = 0;

    // OUT GOING NODES
    itArc = itNode->get_iterator_headNode(), endArc  = itNode->get_endIt_headNode();
    for (; itArc != endArc; itArc++) {
        if(*(output.pSol + itArc->nArc))
            tamOut++;
    }

    // IN GOING NODES
    itArc = itNode->get_iterator_tailNode(), endArc  = itNode->get_endIt_tailNode();
    for (; itArc != endArc; itArc++) {
        if(*(output.pSol + itArc->nArc))
            tamIn++;
    }

}

int create_flow_eq_combine (Output &output, Instance &instance,
                            std::unordered_map<t_node, t_node> &varsHash,
                            GRBModel& model, GRBVar*& variable, jarvis::Jarvis &jarvisObj) {

    std::vector<graph::Node>::iterator itNode, endNode;
    std::vector<graph::NodeElement>::iterator itArc, endArc;
    std::unordered_map<t_node, t_node>::iterator itHash;

    size_t tamOut, tamIn, tam, nodeSize;
    int rhs, count, pos;
    GRBLinExpr expr = 0;

    for (auto &g : instance.gProjects) {
        itNode   = g.get_iteratorToNodes();
        endNode  = g.get_endToNodes();
        nodeSize = g.get_numNodes();
        count = 0;
        for (; itNode != endNode; itNode++) {
            expr = 0, pos = 0;
            get_tam_combine(tamOut, tamIn, output, itNode);
            tam = tamOut + tamIn;
            if (tam == 0) {
                count++;
                continue;
            }

            rhs = (count == 0) ? (1) : ((count == nodeSize -1) ? (-1) : (0));

            double *coeffs;
            GRBVar *vars;
            initialize_elements_constraint (tam, "flow eq()", coeffs, vars, jarvisObj);

            // OUT GOING NODES
            std::fill(coeffs, coeffs + tamOut, 1);
            itArc = itNode->get_iterator_headNode(), endArc  = itNode->get_endIt_headNode();
            for (; itArc != endArc; itArc++) {
                if(*(output.pSol + itArc->nArc)) {
                    itHash = varsHash.find(itArc->nArc);
                    vars[pos] = variable[itHash->second];
                    pos++;
                }
            }

            // IN GOING NODES
            std::fill(coeffs + tamOut, coeffs + tam, -1);
            itArc = itNode->get_iterator_tailNode(), endArc  = itNode->get_endIt_tailNode();
            for (; itArc != endArc; itArc++) {
                if(*(output.pSol + itArc->nArc)) {
                    itHash = varsHash.find(itArc->nArc);
                    vars[pos] = variable[itHash->second];
                    pos++;
                }
            }

            // CREATE CONSTRAINT
            expr.addTerms(coeffs, vars, tam);
            model.addConstr(expr, GRB_EQUAL, rhs);

            delete[] coeffs;
            delete[] vars;
            count++;
        }
    }
    return 0;
}

size_t get_num_variables_pointer (Output &output, size_t numArcs) {

    size_t numVars = 0;

    #pragma omp parallel for simd default (none) shared (numArcs, output) reduction (+:numVars)
    for (size_t i = 0; i < numArcs; i++) {
        if (*(output.pSol + i))
            numVars++;
    }

    return numVars;
}

void fill_hash_table_with_pointer (Instance &instance, GRBVar*& variable,
                                   size_t numArcs, Output &output,
                                   std::unordered_map<t_node, t_node> &varsHash) {

    size_t numVar = 0;
    for (size_t i = 0; i < numArcs; i++) {
        if (*(output.pSol + i)) {
            varsHash.emplace(i, numVar);
            variable[numVar].set(GRB_DoubleAttr_Obj, *(instance.pCost + i));
            variable[numVar].set(GRB_DoubleAttr_Start, 0); // starting solution
            numVar++;
        }
    }

    std::unordered_map<t_node, t_node>::iterator itHash;
    for (auto &a : output.refSet[output.posBestSol].arcsSol) {
        itHash = varsHash.find(a);
        variable[itHash->second].set(GRB_DoubleAttr_Start, 1); // starting solution
    }

}


int combine_solutions (Output &output, jarvis::Jarvis &jarvisObj,
                        Instance &instance, Input &input) {

    jarvisObj.timer.start_time("combiningSol");
    int returnVal = 0;

    // BASICS OF THE MODEL
    GRBEnv env = GRBEnv();
    GRBModel model = GRBModel(env);
    model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
    model.set(GRB_IntParam_Threads, input.numThreadsGurOpti);
    model.set(GRB_DoubleParam_TimeLimit, 2700);
    model.set(GRB_DoubleParam_MIPGap, 0.01);
    model.set(GRB_IntParam_LogToConsole, 1);

    // VARIABLES
    size_t numVar = get_num_variables_pointer(output, input.numArcs);
    GRBVar* variable = model.addVars(numVar, GRB_BINARY);

    std::unordered_map<t_node, t_node> varsHash;
    varsHash.reserve(numVar);
    fill_hash_table_with_pointer(instance, variable, input.numArcs,
            output, varsHash);

    // CONSTRAINTS
    create_flow_eq_combine(output, instance, varsHash, model,
                   variable, jarvisObj);

    create_rhs_constraints_combination (instance, model, output,
            jarvisObj, variable, varsHash);

    // SOLVING THE MODEL
    model.update();
    model.optimize();

    // SAVING SOLUTION
    int status = model.get(GRB_IntAttr_Status);

    if (status == GRB_INFEASIBLE) {
        std::string msg = "No feasible solution found in the combination";
        std::cout << "ERROR: " << msg << "\n";
        jarvisObj.logFile.write_log(msg, jarvis::Log::Error);
        returnVal = -1;
    } else {
        try {
            output.fObjCombined = model.get(GRB_DoubleAttr_ObjVal);
            save_solution_of_combination(instance, variable, output, varsHash);
        } catch (...) {
            error_getting_solution (jarvisObj, "In combine_solutions(), ");
        }
    }

    delete [] variable;

    jarvisObj.timer.end_time("combiningSol");

    return returnVal;
}




//=================================
// FOR THE LOCAL SEARCH
//=================================

void save_solution_local (Output &output, Instance &instance, GRBVar*& variable,
                    std::unordered_set<unsigned int> &netsPenalized, Input &input) {

    t_node nArc = 0;
    std::vector<graph::Node>::iterator itNode, endNode;
    std::vector<graph::NodeElement>::iterator itArc, endArc;

    auto itNet = netsPenalized.begin();
    for (; itNet != netsPenalized.end(); itNet++) {
        itNode  = instance.gProjects[*itNet].get_iteratorToNodes();
        endNode = instance.gProjects[*itNet].get_endToNodes();
        for (; itNode != endNode; itNode++) {
            itArc  = itNode->get_iterator_headNode();
            endArc = itNode->get_endIt_headNode();
            for (; itArc != endArc; itArc++) {
                *(output.pSol + itArc->nArc) = false;
                if (variable[nArc].get(GRB_DoubleAttr_X) > 0.99)
                    *(output.pSol + itArc->nArc) = true;
                nArc++;
            }
        }
    }

    output.fObjLocalSearch = 0.0;
    for (size_t i = 0; i < input.numArcs; i++)
        if (*(output.pSol + i))
            output.fObjLocalSearch += *(instance.pCost + i);

    std::cout << "Final cost = " << output.fObjLocalSearch << "\n";

}



bool create_rhs_constraints_local (Instance &instance, GRBModel& model, Output &output,
                             std::unordered_set<unsigned int> &netsPenalized,
                             jarvis::Jarvis &jarvisObj, GRBVar*& variable,
                             std::unordered_map<t_node, t_node> &varsHash) {

    size_t idxN;
    unsigned int pos;
    int auxRhs;
    std::unordered_set<unsigned int>::iterator itNGur;
    std::unordered_map<t_node, t_node>::iterator itHash;
    std::vector<t_node> arcs;

    GRBLinExpr expr = 0;

    for (auto &itC : instance.constraints) {
        auxRhs = itC.rhs;
        idxN = 0;
        arcs.clear();

        // SEARCH THE INFO NEEDED TO CREATE THE CONSTRAINT
        for (auto &itN : itC.nets) {
            itNGur = netsPenalized.find(itN);
            if (itNGur == netsPenalized.end()) { // net to fix: update rhs
                for (auto &itArc : itC.arcs[idxN]) {
                    if (*(output.pSol + itArc)) {
                        auxRhs--;
                        break; // just one can be in the solution
                    }
                }
            } else { // net for Gurobi: save variables
                for (auto &itArc : itC.arcs[idxN])
                    arcs.emplace_back(itArc);
            }
            idxN++;
        }

        if (auxRhs < 0) {
            return false;
        }

        // CREATE THE CONSTRAINT
        expr = 0;
        double *coeffs;
        GRBVar *vars;

        initialize_elements_constraint (arcs.size(), "constraint eq()",
                                        coeffs, vars, jarvisObj);

        std::fill(coeffs, coeffs + arcs.size(), 1);

        // variables
        pos = 0;
        for (auto &a : arcs) {
            itHash = varsHash.find(a);
            vars[pos] = variable[itHash->second];
            pos++;
        }

        expr.addTerms(coeffs, vars, arcs.size());
        model.addConstr(expr, GRB_LESS_EQUAL, auxRhs);

        delete[] coeffs;
        delete[] vars;
    }

    return true;
}


void fill_hash_table_localSearch (std::unordered_set<unsigned int> &netsPenalized,
                      Instance &instance, GRBVar*& variable, Output &output,
                      std::unordered_map<t_node, t_node> &varsHash) {
    t_node nArc = 0;

    std::vector<graph::Node>::iterator itNode, endNode;
    std::vector<graph::NodeElement>::iterator itArc, endArc;

    auto itNet = netsPenalized.begin();
    for (; itNet != netsPenalized.end(); itNet++) {
        itNode  = instance.gProjects[*itNet].get_iteratorToNodes();
        endNode = instance.gProjects[*itNet].get_endToNodes();
        for (; itNode != endNode; itNode++) {
            itArc  = itNode->get_iterator_headNode();
            endArc = itNode->get_endIt_headNode();
            for (; itArc != endArc; itArc++) {
                varsHash.emplace(itArc->nArc, nArc);
                variable[nArc].set(GRB_DoubleAttr_Obj, *(instance.pCost + itArc->nArc));
                if (*(output.pSol + itArc->nArc)) { // arc is in the current sol
                    variable[nArc].set(GRB_DoubleAttr_Start, 1);
                } else {variable[nArc].set(GRB_DoubleAttr_Start, 0);}
                nArc++;
            }
        }
    }
}


void gurobi_local_search(Instance &instance, Input &input, Output &output,
                          std::unordered_set<unsigned int> &netsPenalized,
                          jarvis::Jarvis &jarvisObj) {

    // BASICS OF THE MODEL
    bool feasible = true;
    GRBEnv env = GRBEnv();
    GRBModel model = GRBModel(env);
    model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
    model.set(GRB_DoubleParam_MIPGap, 0.005);
    model.set(GRB_DoubleParam_TimeLimit, 2700);
    model.set(GRB_IntParam_Threads, input.numThreadsGurOpti);
    model.set(GRB_IntParam_LogToConsole, 1);

    // VARIABLES
    size_t numVar = get_num_variables(netsPenalized, instance);
    GRBVar* variable = model.addVars(numVar, GRB_BINARY);

    std::unordered_map<t_node, t_node> varsHash;
    varsHash.reserve(numVar);
    fill_hash_table_localSearch(netsPenalized, instance, variable, output, varsHash);

    // CONSTRAINTS
    create_flow_eq(netsPenalized, instance, varsHash, model,
                   variable, jarvisObj);

    feasible = create_rhs_constraints_local (instance, model, output,netsPenalized,
                                            jarvisObj, variable,varsHash);

    // SOLVING THE MODEL
    if (feasible) {
        model.update();
        model.optimize();

        // SAVING SOLUTION
        int status = model.get(GRB_IntAttr_Status);

        if (status == GRB_INFEASIBLE) {
            std::cout << "Infeasible solution in Local Search \n";
        } else {
            try {
                save_solution_local (output, instance, variable, netsPenalized, input);
            } catch (...) {
                error_getting_solution (jarvisObj, "In gurobi_local_search(), ");
            }
        }
    } else {
        std::cout << "Infeasible solution in Local Search \n";
    }


    delete [] variable;

}





//=================================
// SECONDARY FUNCTIONS
//=================================




void fill_hash_table (std::unordered_set<unsigned int> &netsPenalized,
                      Instance &instance, GRBVar*& variable,
                      std::unordered_map<t_node, t_node> &varsHash) {
    t_node nArc = 0;

    std::vector<graph::Node>::iterator itNode, endNode;
    std::vector<graph::NodeElement>::iterator itArc, endArc;

    auto itNet = netsPenalized.begin();
    for (; itNet != netsPenalized.end(); itNet++) {
        itNode  = instance.gProjects[*itNet].get_iteratorToNodes();
        endNode = instance.gProjects[*itNet].get_endToNodes();
        for (; itNode != endNode; itNode++) {
            itArc  = itNode->get_iterator_headNode();
            endArc = itNode->get_endIt_headNode();
            for (; itArc != endArc; itArc++) {
                varsHash.emplace(itArc->nArc, nArc);
                variable[nArc].set(GRB_DoubleAttr_Obj, *(instance.pCost + itArc->nArc));
                nArc++;
            }
        }
    }
}


size_t get_num_variables (std::unordered_set<unsigned int> &netsPenalized,
                          Instance &instance) {

    size_t numVar = 0;
    auto it = netsPenalized.begin();
    for (; it != netsPenalized.end(); it++) {
        numVar += instance.gProjects[*it].get_numArcs();
    }
    return numVar;
}


int initialize_elements_constraint (size_t tam, std::string msg, double*& coeffs,
                                    GRBVar*& vars, jarvis::Jarvis &jarvisObj) {

    coeffs = new (std::nothrow) double [tam];
    vars   = new (std::nothrow) GRBVar [tam];

    if (coeffs == nullptr || vars == nullptr) {
        std::string msg2 = "Memory pointer allocation in ";
        #pragma omp critical
        {
            std::cout << "[ERROR]: " << msg2 << msg << "\n";
            jarvisObj.logFile.write_log(msg2 + msg, jarvis::Log::Error);
        }
        return -1;
    }

}

void error_in_hash_table (jarvis::Jarvis &jarvisObj, std::string msg) {

    std::string msg2 = " element does not exit in the hash table employed in Gurobi.";
    #pragma omp critical
    {
        std::cout << "[ERROR]: " << msg << msg2 << "\n";
        jarvisObj.logFile.write_log(msg + msg2, jarvis::Log::Error);
    }

}

void error_getting_solution (jarvis::Jarvis &jarvisObj, std::string msg) {
    std::string msg2 = " error when getting solution from Gurobi.";
    #pragma omp critical
    {
        std::cout << "[ERROR]: " << msg << msg2 << "\n";
        jarvisObj.logFile.write_log(msg + msg2, jarvis::Log::Error);
    }
}

void print_info(jarvis::Jarvis &jarvisObj, const std::string &msg) {
    jarvisObj.logFile.write_log(msg, jarvis::Log::Info);
    std::cout << msg << "\n";
    std::cout << "-------------------------" << "\n";
}
