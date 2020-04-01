//
// Created by David Garcia-Heredia on 7/26/19.
//

#include "Reading.h"
#include "HeuristicII.h"

#include <stdlib.h>
#include <zconf.h>


int main () {

    // ELEMENTS REQUIRED FOR THE PROBLEM
    std::string pathInstances = "./data/instances.txt";

    // INITIALIZE JARVIS ELEMENTS
    jarvis::Jarvis jarvisObj = jarvis::Jarvis();
    jarvisObj.logFile.set_logfilename("./output/logFileJarvis.txt");
    jarvisObj.logFile.set_logstatus(jarvis::Log::Info);
    jarvisObj.tableNames.emplace_back("./output/Results.csv");
    jarvisObj.tableNames.emplace_back("./output/ArcsInSol.csv");

    // DECLARE ELEMENTS NEEDED
    Input input;
    Instance instance;
    std::string nameCase;
    Output output;
    std::string nameMethod = "Heuristic";

    // READING INSTANCES
    read_paths_instances_to_solve(input, pathInstances, jarvisObj);

    // EXECUTING INSTANCES
    for (size_t Inst_i = 0; Inst_i < input.graphPath.size(); Inst_i++) {
        // READING INPUT & GRAPH
        read_input(Inst_i, input, jarvisObj, instance);
        read_graph(instance, input, Inst_i, jarvisObj);

        // PREPARING POINTERS
        prepare_pointers(output, jarvisObj, input);
        omp_set_num_threads(input.numThreads);

        // SOLVE PROBLEM FOR EACH RHS PENALIZATION
        for (size_t rhs_j = 0; rhs_j < input.rhsFolders.size(); rhs_j++) {
            // ADD CASE NAME TO OUTPUT TABLES
            nameCase = instance.name + "/" + input.rhsFolders[rhs_j];
            jarvisObj.dictResults[jarvisObj.tableNames[0]].emplace_back(nameCase);
            jarvisObj.dictResults[jarvisObj.tableNames[1]].emplace_back(nameCase);
            print_frame_subtitle("CASE = " + nameCase);

            // INITIALIZE SEEDS FOR THE POOL AND THE POINTER TO SAVE THE SOL
            // std::iota(input.pSeed, input.pSeed + input.numThreads, 0); // for no random seeds
            initialize_seeds(jarvisObj, input); // for random seeds
            std::fill(output.pSol, output.pSol + input.numArcs, false);

            // READ CONSTRAINTS
            read_constraints(instance, input, Inst_i, rhs_j, jarvisObj);

            // SOLVING USING EXACT METHODS
            // nameMethod = "Gurobi";
            // solve_whole_problem_with_gurobi(instance, input, jarvisObj, output);

            // SOLVING USING THE MATHEURISTIC
            heuristic_algorithm(instance, input, jarvisObj, output);

            // saving in Jarvis arcs in the solution
            export_arcs_solution(output, input.numArcs, jarvisObj, nameMethod);
        } // end rhs_j

        // Note: vectors are cleared in the reading functions.
        // CLEAR INPUT
        delete [] input.pSeed;

        // CLEAR OUTPUT
        delete [] output.pNumIter;
        delete [] output.pFobjPool;
        delete [] output.pSol;
        delete [] output.status;

        // CLEAR INSTANCE
        delete [] instance.pCost;

    } // end Inst_i

    // EXPORT INFO
    for (size_t i = 0; i < jarvisObj.tableNames.size(); i++) {
        jarvisObj.table.set_tableName(jarvisObj.tableNames[i]);
        jarvisObj.table.export_table(jarvisObj.dictResults[jarvisObj.tableNames[i]]);
    }

    return 0;
}
