//
// Created by David Garcia-Heredia  on 7/5/19.
//

#include "Reading.h"


//-----------------------------
// READING FUNCTIONS
//-----------------------------

// This function reads from "pathInstances" the instances to solve
void read_paths_instances_to_solve (Input &input, std::string pathInstances,
                                    jarvis::Jarvis &jarvisObj) {

    // print into screen what we are doing
    print_info2(jarvisObj, "Reading Instances");

    // open file to read
    std::ifstream afile(pathInstances, std::ios::in);
    std::string line;

    // read info
    if (afile.is_open()) {
        input.graphPath.clear(); // make sure it is clear
        while (!afile.eof()) {
            // path to the folder with the graphs
            std::getline(afile, line);
            input.graphPath.push_back(line);
        }
        input.graphPath.pop_back(); // because it reads a blank line
        afile.close();
    } else {
        std::cerr << "[ERROR]: Unable to open file to read the instances. \n";
    }
}


// This function reads the input of the algorithm associated to each instance
void read_input (size_t Inst_i, Input &input, jarvis::Jarvis &jarvisObj,
                 Instance &instance) {

    // print into screen what we are doing
    print_frame_title(input.graphPath[Inst_i]);
    print_info2(jarvisObj, "Reading Input");

    // open file to read
    std::string line;
    std::ifstream afile(input.graphPath[Inst_i] + "/input.txt", std::ios::in);

    // read info
    if (afile.is_open()) {
        // --------------- GENERAL INFORMATION --------------
        std::getline(afile, line); // title line

        // name of instance
        std::getline(afile, line); // header
        std::getline(afile, line); // name
        instance.name = line;

        // numArcs
        std::getline(afile, line); // header
        afile >> input.numArcs;
        std::getline(afile, line); // remove line

        // numThreadsGurOpti
        std::getline(afile, line); // header
        afile >> input.numThreadsGurOpti;
        std::getline(afile, line); // remove line

        // --------------- POOL OF SOLUTIONS --------------
        std::getline(afile, line); // title line

        // numSolInHeuristic
        std::getline(afile, line); // header
        afile >> input.numSolInHeuristic;
        std::getline(afile, line); // remove line

        // maxIterSearching
        std::getline(afile, line); // header
        afile >> input.maxIterSearching;
        std::getline(afile, line); // remove line

        // numThreads
        std::getline(afile, line); // header
        afile >> input.numThreads;
        std::getline(afile, line); // remove line

        // arcPenalization
        std::getline(afile, line); // header
        afile >> input.arcPenalization;
        std::getline(afile, line); // remove line

        // value for the coin flip
        std::getline(afile, line); // header
        afile >> input.probCoin;
        std::getline(afile, line); // remove line

        // use Gurobi in searching feasible solution
        std::getline(afile, line); // header
        afile >> input.useGurobi;
        std::getline(afile, line); // remove line

        // % of nets from which Gurobi is used searching feasible sol
        std::getline(afile, line); // header
        afile >> input.percToUseGurobi;
        std::getline(afile, line); // remove line

        // percentage of extra nets to use when employing Gurobi
        std::getline(afile, line); // header
        afile >> input.percExtraNets;
        std::getline(afile, line); // remove line

        // number of iters until using Gurobi again
        std::getline(afile, line); // header
        afile >> input.numIterAfterGurobi;
        std::getline(afile, line); // remove line

        // --------------- LOCAL SEARCH  --------------
        std::getline(afile, line); // title line

        // percentage of bad nets to use
        std::getline(afile, line); // header
        afile >> input.percBadNets;
        std::getline(afile, line); // remove line

        // percentage of related nets to really bad nets
        std::getline(afile, line); // header
        afile >> input.percRelatedNets;
        std::getline(afile, line); // remove line

        // --------------- rhsFolders --------------
        std::getline(afile, line); // header
        input.rhsFolders.clear(); // make sure it is clear

        while (!afile.eof()) {
            std::getline(afile, line);
            input.rhsFolders.push_back(line);
        }
        input.rhsFolders.pop_back(); // because it reads and extra line

        afile.close();

    } else {
        std::cerr << "[ERROR]: Unable to open file to read the input in instance: ";
        std::cerr << input.graphPath[Inst_i] << "\n";
    }

    // print into screen the info just read to check that everthing is fine
    dummy_check(instance, input);
}


// This function reads the 4d-graph for the current instance to solve
int read_graph (Instance &instance, Input &input, size_t Inst_i,
                jarvis::Jarvis &jarvisObj) {

    // print into screen what we are doing
    std::cout << "\n";
    print_info2(jarvisObj, "Reading Graph");

    // variables required
    std::string line;
    char coma;
    std::vector<graph::Arc> arcs;
    graph::Arc arc;
    float cost;
    unsigned int numNet = 0, netRead;

    // open file with the 4d networks
    std::ifstream afile(input.graphPath[Inst_i] + "/" + instance.name + ".csv", std::ios::in);

    // reading info
    if (afile.is_open()) {
        // pointer containing the costs of the arcs
        instance.pCost = new (std::nothrow) float [input.numArcs];
        if (instance.pCost == nullptr) {
            std::cerr << "[ERROR]: Allocation memory failed in read_graph \n";
            return -1;
        }

        getline(afile, line); // remove header
        instance.gProjects.clear(); // make sure it is clear

        // reading info: net | nArc | tail | head | cost
        while (!afile.eof()) {
            // read net and make sure we are not at the end
            afile >> netRead;
            if (afile.eof()) { break; }

            // if we are reading a new net, load all the previous info
            if (numNet != netRead) {
                numNet = netRead;
                instance.gProjects.emplace_back(graph::Graph());
                instance.gProjects.back().make_adjacency_list(arcs);
                arcs.clear();
            }

            // read nArc | tail | head | cost
            afile >> coma >> arc.numArc >> coma >> arc.tail
                  >> coma >> arc.head   >> coma >> cost;

            // remove rest of the info (e.g: sector employed) if any. We don't need it
            getline(afile, line);

            // save info in a temporary variable to load it afterwards
            arcs.push_back(arc);
            *(instance.pCost + arc.numArc) = cost;
        }

        // save for the last net read
        instance.gProjects.emplace_back(graph::Graph());
        instance.gProjects.back().make_adjacency_list(arcs);
        afile.close();

    } else {
        std::cerr << "[ERROR]: Unable to open file to read the graph in instance: ";
        std::cerr << input.graphPath[Inst_i] << "\n";
    }

    return 0;
}


// This function reads the constraints of the problem
void read_constraints (Instance &instance, Input &input, size_t Inst_i,
                       size_t rhs_j, jarvis::Jarvis &jarvisObj) {

    // print into screen what we are doing
    std::cout << '\n';
    print_info2(jarvisObj, "Reading Constraints");

    // variables required
    std::string line = input.graphPath[Inst_i] + "/" + input.rhsFolders[rhs_j] + ".csv";
    char coma;
    double rhs;
    Constraint constAux;
    unsigned int numNet, nArc;
    std::map<unsigned int, std::vector<unsigned int> > mapNetArcs;
    auto it = mapNetArcs.begin();

    // open file
    std::ifstream afile(line, std::ios::in);

    // read constraint: name | rhs | (net, arc) involved
    if (afile.is_open()) {
        instance.constraints.clear(); // make sure it is clear
        std::getline(afile, line); // header

        while (!afile.eof()) {
            // name of the constraint: read all the characters until finding a ','
            afile >> coma;
            if (afile.eof()) { break; }

            constAux.name = "";
            while (coma != ',') {
                constAux.name.push_back(coma);
                afile >> coma;
            }

            // rhs of the constraint
            afile >> rhs >> coma;
            constAux.rhs = static_cast<unsigned int>(std::trunc(rhs));
            afile >> coma; // I read char = |

            // arcs in the constraint
            mapNetArcs.clear(); // clear for the new constraint
            while (coma != '*') {
                afile >> numNet >> coma >> nArc;
                mapNetArcs[numNet].emplace_back(nArc);
                afile >> coma >> coma; // last coma is '+' or '*'
            }

            // load info of the constraint just read, but only if it is no redundant
            constAux.nets.clear(); // clear for the new restriction
            constAux.arcs.clear();
            it = mapNetArcs.begin();
            for (; it != mapNetArcs.end() ; it++) {
                constAux.nets.emplace_back(it->first);
                constAux.arcs.emplace_back(it->second);
            }

            if (constAux.nets.size() > constAux.rhs) // no redundant constraint
                instance.constraints.emplace_back(constAux);
        }
        // instance.constraints.pop_back(); // because it reads a blank line

        afile.close();

    } else {
        std::cerr << "[ERROR]: Unable to read constraints: ";
        std::cerr << input.graphPath[Inst_i] << "/" << input.rhsFolders[rhs_j] << "\n";
    }
}



//-----------------------------
// SECONDARY FUNCTIONS
//-----------------------------


void dummy_check (Instance &instance, Input &input) {

    std::cout << "\nShowing Input: \n";
    std::cout << "*] numArcs = " << input.numArcs << "\n";
    std::cout << "*] numThreadsGurOpti = " << input.numThreadsGurOpti << "\n";

    std::cout << '\n';

    std::cout << "*] numSolInHeuristic = " << input.numSolInHeuristic << "\n";
    std::cout << "*] maxIterSearching = " << input.maxIterSearching << "\n";
    std::cout << "*] numThreads = " << input.numThreads << "\n";
    std::cout << "*] arcPenalization = " << input.arcPenalization << "\n";
    std::cout << "*] probCoin = " << input.probCoin << "\n";
    std::cout << "*] useGurobi = " << input.useGurobi << "\n";

    std::cout << "*] percToUseGurobi = " << input.percToUseGurobi << "\n";
    std::cout << "*] percExtraNets = " << input.percExtraNets << "\n";
    std::cout << "*] numIterAfterGurobi = " << input.numIterAfterGurobi << "\n";

    std::cout << '\n';

    std::cout << "*] percBadNets = " << input.percBadNets << "\n";
    std::cout << "*] percRelatedNets = " << input.percRelatedNets << "\n";

    std::cout << '\n';

    for (const auto &rhsFolder : input.rhsFolders) {
        std::cout << "*] " << rhsFolder << "\n";
    }
    std::cout << "\n";
}


void print_frame_title(const std::string &msg) {
    std::cout << "\n\n========================================================\n";
    std::cout << msg << "\n";
    std::cout << "========================================================\n\n";
}

void print_frame_subtitle(const std::string &msg) {
    std::cout << "\n========================\n";
    std::cout << msg << "\n";
    std::cout << "========================\n\n";
}

void error_pointers_seeds (jarvis::Jarvis &jarvisObj) {
    std::string msg = "Memory allocation failed when initializing pointers in main().";
    std::cout << "[ERROR]: " << msg << "\n";

    jarvisObj.logFile.write_log(msg, jarvis::Log::Error);
}

void export_arcs_solution (Output &output, size_t numArcs, jarvis::Jarvis &jarvisObj,
                            const std::string& nameMethod) {

    std::string nameT = jarvisObj.tableNames[1];

    std::string arcs = "";
    for (size_t i = 0; i < numArcs; i++) {
        if (*(output.pSol + i)){
            arcs += std::to_string(i);
            arcs += "/"; // inner sep
        }
    }

    arcs.pop_back(); // remove last sep
    jarvisObj.dictResults[nameT].back().add_result_string("method", nameMethod);
    jarvisObj.dictResults[nameT].back().add_result_string("arcs", arcs);
}

int prepare_pointers(Output &output, jarvis::Jarvis &jarvisObj, Input& input) {

    input.pSeed      = new (std::nothrow) unsigned [input.numThreads];
    output.pFobjPool = new (std::nothrow) float [input.numSolInHeuristic];
    output.pNumIter  = new (std::nothrow) unsigned int [input.numSolInHeuristic];
    output.status    = new (std::nothrow) char [input.numSolInHeuristic];
    output.pSol      = new (std::nothrow) bool [input.numArcs];

    if (input.pSeed == nullptr || output.pFobjPool == nullptr ||
        output.pNumIter == nullptr || output.pSol == nullptr  ||
        output.status == nullptr) {
        error_pointers_seeds(jarvisObj);
        return -1;
    }

    return 0;
}


void initialize_seeds(jarvis::Jarvis &jarvisObj, Input& input) {

    jarvisObj.randNum.set_lb(0);
    jarvisObj.randNum.set_ub(1000000);
    jarvisObj.randNum.set_option(jarvis::Random::RVUnifD);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    std::cout << "seeds are: \n";
    for (size_t i = 0; i < input.numThreads; i++) {
        *(input.pSeed + i) = jarvisObj.randNum.generate_random(generator);
        std::cout << *(input.pSeed + i) << " | ";
    }
    std::cout << "\n";
}


void print_info2(jarvis::Jarvis &jarvisObj, const std::string &msg) {
    jarvisObj.logFile.write_log(msg, jarvis::Log::Info);
    std::cout << msg << "\n";
    std::cout << "-------------------------" << "\n";
}
