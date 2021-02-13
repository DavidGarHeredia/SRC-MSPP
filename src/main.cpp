//
// Created by David Garcia-Heredia on 7/26/19.
//

#include "Reading.h"
#include "HeuristicII.h"

#include <stdlib.h>
#include <zconf.h>


int main (int argc, char const *argv[]) {

    // ELEMENTS REQUIRED FOR THE PROBLEM
    // std::string pathInstances = "./data/instances.txt";

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


    input.graphPath.push_back(argv[1]); 
    print_frame_title(input.graphPath[0]);
    print_info2(jarvisObj, "Reading Input");
    instance.name = argv[3];
    input.numArcs = std::stoi(argv[4]);
    input.numThreadsGurOpti = std::stoi(argv[5]);
    input.numSolInHeuristic = std::stoi(argv[6]);
    input.maxIterSearching = std::stoi(argv[7]);
    input.numThreads = std::stoi(argv[8]);
    input.arcPenalization = std::stof(argv[9]);
    input.probCoin = std::stof(argv[10]);
    input.useGurobi = std::stoi(argv[11]);
    input.percToUseGurobi = std::stof(argv[12]);
    input.percExtraNets = std::stof(argv[13]);
    input.numIterAfterGurobi = std::stoi(argv[14]);
    input.percBadNets = std::stof(argv[15]);
    input.percRelatedNets = std::stof(argv[16]);
    input.rhsFolders.push_back(argv[2]);
    dummy_check(instance, input);

    read_graph(instance, input, 0, jarvisObj);

    // // PREPARING POINTERS
    prepare_pointers(output, jarvisObj, input);
    omp_set_num_threads(input.numThreads);

    // SOLVE PROBLEM 
    // ADD CASE NAME TO OUTPUT TABLES
    nameCase = input.rhsFolders[0];
    jarvisObj.dictResults[jarvisObj.tableNames[0]].emplace_back(nameCase);
    jarvisObj.dictResults[jarvisObj.tableNames[1]].emplace_back(nameCase);
    print_frame_subtitle("CASE = " + nameCase);

    // INITIALIZE SEEDS FOR THE POOL AND THE POINTER TO SAVE THE SOL
    // std::iota(input.pSeed, input.pSeed + input.numThreads, 0); // for no random seeds
    initialize_seeds(jarvisObj, input); // for random seeds
    std::fill(output.pSol, output.pSol + input.numArcs, false);

    // READ CONSTRAINTS
    read_constraints(instance, input, 0, 0, jarvisObj);

    // SOLVING USING EXACT METHODS
    nameMethod = "Gurobi";
    solve_whole_problem_with_gurobi(instance, input, jarvisObj, output);

    // // SOLVING USING THE MATHEURISTIC
    // heuristic_algorithm(instance, input, jarvisObj, output);

    // saving in Jarvis arcs in the solution
    export_arcs_solution(output, input.numArcs, jarvisObj, nameMethod);

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


    // EXPORT INFO
    for (size_t i = 0; i < jarvisObj.tableNames.size(); i++) {
        jarvisObj.table.set_tableName(jarvisObj.tableNames[i]);
        jarvisObj.table.export_table(jarvisObj.dictResults[jarvisObj.tableNames[i]]);
    }

    return 0;
}
