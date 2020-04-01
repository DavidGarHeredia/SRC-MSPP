//
// Created by David Garcia-Heredia on 7/26/19.
//

#include "HeuristicII.h"


//-----------------------------
// FUNCTIONS CONFORMING THE ALGORITHM
//-----------------------------

// given the input of the problem, this function tries to generate the best solution
void heuristic_algorithm(Instance &instance, Input &input, jarvis::Jarvis &jarvisObj,
                        Output &output) {

    // add some info to export afterwards and initialize required variables
    std::string tab = jarvisObj.tableNames[0];
    jarvisObj.dictResults[tab].back().add_result_numeric("nThreads", input.numThreads);
    jarvisObj.dictResults[tab].back().add_result_numeric("sizePool", input.numSolInHeuristic);
    output.refSet.resize(input.numSolInHeuristic);
    int returnVal;

    //---------POOL OF SOLUTIONS-----------//
    info_msg(jarvisObj, "Creating elements in pool");

    jarvisObj.timer.start_time("feasibleSol");
    compute_lb_cost(instance, jarvisObj, input);// computing lower bound cost
    // parallel for to create the pool
    #pragma omp parallel for default(none) shared(instance, jarvisObj, input, output)
    for (size_t nSol = 0; nSol < input.numSolInHeuristic; nSol++)
        get_feasible_solution(instance, input, jarvisObj, output, nSol);
    jarvisObj.timer.end_time("feasibleSol");

    // checking if the solution is optimal
    bool optimalSol = false;
    is_solution_optimal(jarvisObj, input, output, optimalSol, instance.costLB);

    //---------COMBINING & LOCAL SEARCH----------//
    if (!optimalSol) {
        // combining all the solutions in the pool
        info_msg(jarvisObj, "Combining solutions");
        returnVal = combine_solutions(output, jarvisObj, instance, input);

        // applying local search
        if (returnVal != -1) {
            info_msg(jarvisObj, "Local search");
            local_search(instance, input, jarvisObj, output);
        } else {
            std::cout << "Skipping local search due to infeasibility in combine_solutions()"
                      << '\n';
        }
    }

    // save info to export afterwards
    load_output_to_jarvis(output, jarvisObj, input.numSolInHeuristic);
}


// This function tries to generate a feasible solution for the problem
void get_feasible_solution (Instance &instance, Input &input, jarvis::Jarvis &jarvisObj,
                           Output &output, size_t nSol) {

    // create the required (private) variables
    bool useExact = true;
    size_t numNets = static_cast<int>(input.percToUseGurobi * instance.gProjects.size());
    float cost;
    unsigned int numIter = 0, numIter2 = 0;
    std::unordered_set<unsigned int> netsPenalized;
    TempSolution sol;
    initialize_variables_pool(instance, input, jarvisObj, sol);

    // generate an starting solution: shortest path
    shortest_path(instance, sol, jarvisObj);

    // check if the shortest path solution is feasible, and penalize arcs in case it is not
    sol.feasible = penalize_arcs(instance, sol, input, netsPenalized, nSol, numIter);

    // ALGORITHM
    while (!sol.feasible && numIter <= input.maxIterSearching) {
        // we start if - else if - else for the 3 possible situations
        if (netsPenalized.empty()) { // allow more penalization
            std::fill(sol.pPenalized, sol.pPenalized + input.numArcs, false);
        } else if (input.useGurobi && useExact &&
                   netsPenalized.size() <= numNets) { // obtain a new solution using Gurobi
            // add some extra nets to Gurobi so the problem is no so tight
            if (nSol%2 == 0)
                select_k_worst_nets(netsPenalized, input, instance, sol);
            else
                select_k_random_nets(netsPenalized, input, instance);

            // solve
            sol.feasible = feasible_with_gurobi(instance, sol, input,
                                               netsPenalized, jarvisObj);
            if (sol.feasible) { // the solution is feasible
                break;
            } else { // the solution is no feasible
                // recompute the shortest path to no penalize again
                //  without having obtained a new solution
                shortest_path_penalized_nets(instance, sol, jarvisObj, netsPenalized);
            }
            useExact = false;
            numIter2 = 0;
        } else { // obtain a new solution recomputing shortest_path
            shortest_path_penalized_nets(instance, sol, jarvisObj, netsPenalized);
        }

        // check feasibility of the new solution, penalize arcs (if necessary)
        // and start the loop again
        numIter++;
        numIter2++;
        sol.feasible = penalize_arcs(instance, sol, input, netsPenalized, nSol, numIter);
        if (numIter2 >= input.numIterAfterGurobi) { useExact = true; }
    } // end while loop

    // At this point, either we have a feasible solution, or we have skipped the while loop
    // due to a limit in the number of iterations

    // save info of the solution
    cost = get_solution_cost(instance, sol);
    get_solution_bool(output, sol, nSol);

    *(output.pFobjPool + nSol) = cost;
    output.refSet[nSol].Fobj   = cost;
    *(output.pNumIter + nSol)  = numIter;
    *(output.status + nSol)    = ((sol.feasible) ? 'F' :  'I'); // Feasible or Infeasible

    delete[] sol.pCost;
    delete[] sol.pPenalized;
}


// Function local_search() tries to improve the solution of the bad networks at expense of
// worsen other better. Obviously, the final trade-off results in a global improvement.
// The nets to improve and to worsen are selected in 2 steps. Afterwards, Gurobi is used.
void local_search (Instance &instance, Input &input, jarvis::Jarvis &jarvisObj,
                    Output &output) {

    jarvisObj.timer.start_time("localSearch");

    // create variables required
    std::vector<std::pair<unsigned int, float> > nets;
    std::unordered_set<unsigned int> badNets, relatedNets;
    std::unordered_set<unsigned int>::iterator it;
    std::string nameCase = jarvisObj.dictResults[jarvisObj.tableNames[0]].back().get_instanceName();


    // First step: getting bad nets
    // sort nets according to the objective function
    sort_k_bad_nets(nets, input, output, instance);
    // get the nets
    for (auto &n : nets) {
        badNets.emplace(n.first);
        relatedNets.emplace(n.first); // we recycle the container
    }

    // Second step: get related nets
    // calculate the number of related nets
    size_t numNets = static_cast<int>(input.percRelatedNets * instance.gProjects.size());
    numNets += relatedNets.size(); // we add to know when to stop considering networks

    for (auto &itC : instance.constraints) {
        for (auto &itN : itC.nets) { // see the nets in that constraint
            it = badNets.find(itN);
            if (it != badNets.end()) { // load all the nets
                for (auto &itN2 : itC.nets)
                    relatedNets.emplace(itN2);
                break;
            }
        }
        if (relatedNets.size() >= numNets) { break; }
    }

    // SOLVE THE PROBLEM
    std::cout << "num bad nets = "      << badNets.size()      << "\n";
    std::cout << "num related nets = "  << relatedNets.size()  << "\n";
    badNets.insert(relatedNets.begin(), relatedNets.end());

    gurobi_local_search(instance, input, output, badNets, jarvisObj);

    jarvisObj.timer.end_time("localSearch");
}







//-----------------------------
// AUXILIARY IMPORTANT FUNCTIONS
//-----------------------------

// to save the output
void load_output_to_jarvis(Output &output, jarvis::Jarvis &jarvisObj, size_t nSols) {

    std::string name, status, nameT = jarvisObj.tableNames[0];
    double time;

    // POOL OF SOLUTIONS
    for (size_t n = 0; n < nSols; n++) {
        name = "fObjSol" + std::to_string(n);
        jarvisObj.dictResults[nameT].back().add_result_numeric(name, *(output.pFobjPool + n));
        name = "nIterSol" + std::to_string(n);
        jarvisObj.dictResults[nameT].back().add_result_numeric(name, *(output.pNumIter + n));
        name = "statusSol" + std::to_string(n);
        status = *(output.status + n);
        jarvisObj.dictResults[nameT].back().add_result_string(name, status);
    }

    time = jarvisObj.timer.get_difTime(jarvis::Timer::seconds, "feasibleSol");
    jarvisObj.dictResults[nameT].back().add_result_numeric("timeFeasibleSol", time);

    // COMBINING SOLUTIONS
    jarvisObj.dictResults[nameT].back().add_result_numeric("fObjCombined", output.fObjCombined);
    time = jarvisObj.timer.get_difTime(jarvis::Timer::seconds, "combiningSol");
    jarvisObj.dictResults[nameT].back().add_result_numeric("timeCombiningSol", time);

    // LOCAL SEARCH STEP
    jarvisObj.dictResults[nameT].back().add_result_numeric("fObjLocalSearch", output.fObjLocalSearch);
    time = jarvisObj.timer.get_difTime(jarvis::Timer::seconds, "localSearch");
    jarvisObj.dictResults[nameT].back().add_result_numeric("timeLocalSearch", time);
}


// compute the lower bound costs for the algorithm
void compute_lb_cost(Instance &instance, jarvis::Jarvis &jarvisObj, Input &input)
{
    t_node sink;
    instance.costLB = 0.0;
    instance.pLB = new (std::nothrow) float [instance.gProjects.size()];

    TempSolution sol; // In this case, dummy variable needed.
    sol.pCost = new (std::nothrow) float [input.numArcs];
    sol.arcs.resize(instance.gProjects.size());
    #pragma omp simd
      for (size_t i = 0; i < input.numArcs; i++)
          *(sol.pCost + i) = *(instance.pCost + i);

    for (size_t g = 0; g < instance.gProjects.size(); g++) {
        sink = static_cast<t_node>(instance.gProjects[g].get_numNodes() - 1);
        *(instance.pLB + g) = shortest_path_acyclic (0, sink, instance.gProjects[g], jarvisObj, sol, g);
        instance.costLB += *(instance.pLB + g);
    }

    delete[] sol.pCost;
}


// for the pool of solutions
void is_solution_optimal(jarvis::Jarvis &jarvisObj, Input &input, Output &output,
                          bool &optimalSol, float optCost) {

    int nFeasibleSol = 0;
    float fobjComp = *(output.pFobjPool + 0);
    // go through all the solutions in the pool
    for (size_t nSol = 0; nSol < input.numSolInHeuristic; nSol++) {
        if (*(output.status + nSol) == 'F') { // it is feasible
            nFeasibleSol++;
            output.refSet[nSol].feasible = true;
            if (fobjComp > *(output.pFobjPool + nSol)) { // check if it is the best so far
                output.posBestSol = nSol;
                fobjComp = *(output.pFobjPool + nSol);
            }
        } else {
            output.refSet[nSol].feasible = false;
        }
    }

    if (fobjComp == optCost) {
            std::cout << "Optimal solution found\n";
            output.fObjCombined = optCost;
            output.fObjLocalSearch = optCost;
            optimalSol = true;
        } else {
            std::cout << "num of Feasible solution in the pool = " << nFeasibleSol
                      << " out of " << input.numSolInHeuristic << "\n";
            std::cout << "Best (feasible) solution: cost = " << fobjComp
                      << " Niter = " << *(output.pNumIter + output.posBestSol) << '\n';
        }

    // we initialize the wall times of the algorithm to no have problems
    // if future phases are not performed (e.g.: due to infeasibility)
    dummy_times(jarvisObj);
}


// get cost of the solution obtained at the pool
float get_solution_cost (Instance &instance, TempSolution &sol) {
    float cost = 0.0;

    for (auto &arc : sol.arcs) {
        for (auto &a : arc)
            cost += *(instance.pCost + a);
    }

    return cost;
}


// to penalize arcs contributing to the infeasibility of the solution
bool penalize_arcs (Instance &instance, TempSolution &sol, Input &input,
                    std::unordered_set<unsigned int> &netsPenalized, size_t nSol,
                    unsigned int numIter) {

    // variables required
    bool feasible = true, noPenalized;
    unsigned int rhsAux;
    int counter = 0;
    int idThread = omp_get_thread_num();
    size_t idxN;
    float randNum;

    auto itSet = sol.arcs[0].begin(); // unordered set
    std::vector<unsigned int> idxInSol;
    netsPenalized.clear();

    // go through all the constraints
    for (auto &itC : instance.constraints) {
        rhsAux = 0;
        idxInSol.clear();
        idxN = 0;
        // 1st: find out if it is infeasible
        for (auto &itN : itC.nets) {
            for (auto &itArc : itC.arcs[idxN]) {
                itSet = sol.arcs[itN].find(itArc);
                if (itSet != sol.arcs[itN].end()) {
                    idxInSol.emplace_back(idxN);
                    rhsAux++;
                    break;
                }
            }
            idxN++;
        }
        // 2nd: penalize (at random) if the constraint is infeasible
        if (rhsAux > itC.rhs) {
            feasible = false; // the current solution is no feasible
            counter++;
            // go through all the nets that are currently consuming the resource of the constraint
            for (unsigned int idxP : idxInSol) {
                // decide at random if we penalize the arcs or not
                randNum = (float) rand_r(&*(input.pSeed + idThread)) / RAND_MAX;
                if (randNum < input.probCoin) { // flip coin
                    noPenalized = false;
                    // check if the arcs have been penalized before. If ALL have been, don't penalize
                    for (auto &itArc : itC.arcs[idxP]) {
                        if (!*(sol.pPenalized + itArc)) { // not all the arcs have been penalized
                            noPenalized = true;
                            break;
                        }
                    }
                    // no ALL the arcs have been penalize before, so we penalize
                    if (noPenalized) {
                        rhsAux--;
                        netsPenalized.emplace(itC.nets[idxP]);
                        for (auto &itArc : itC.arcs[idxP]) {
                            *(sol.pCost + itArc) += input.arcPenalization;
                            *(sol.pPenalized + itArc) = true;
                        }
                        if (rhsAux <= itC.rhs) { break; }
                    }
                }
            }
        } // end of 2nd step
    } // end of for loop

    if (numIter % 50 == 0) { // show each 50 to avoid overhead
        #pragma omp critical
        std::cout << "nSol = " << nSol << ", thread = " << idThread
                  << ": Num Infeasible constraints = "  << counter
                  << " Nets penalized = " << netsPenalized.size() << "\n";
    }

    return feasible;
}


// function to compute the shortest path in acyclic networks
float shortest_path_acyclic (t_node source, t_node sink, graph::Graph &g,
                             jarvis::Jarvis &jarvisObj, TempSolution &sol, size_t idx) {

    // create variables
    sol.arcs[idx].clear(); // let it free for the new arcs
    size_t numNodes = g.get_numNodes();
    size_t tam  = numNodes - source;
    size_t tam2 = sink - source;
    unsigned int pos;
    graph::Node node;
    float cost;
    bool cond;
    auto *pPred      = new (std::nothrow) int   [tam];
    auto *pCostLabel = new (std::nothrow) float [tam];

    if (pPred == nullptr || pCostLabel == nullptr) {
        error_pointers_shortest_path(jarvisObj);
        return -1;
    }

    std::fill(pPred, pPred + tam2 + 1, -1);
    std::fill(pCostLabel, pCostLabel + tam2 + 1, std::numeric_limits<float>::max());
    *(pCostLabel + 0) = 0.0;

    // algorithm
    for (size_t i = 0; i < tam2; i++) {
        node = g.get_node(i + source);
        for (auto &arc : node.get_vector_headNode()) {
            pos  = arc.node - source;
            cond = *(pCostLabel + pos) > *(pCostLabel + i) + *(sol.pCost + arc.nArc);
            if (cond) {
                *(pCostLabel + pos) = *(pCostLabel + i) + *(sol.pCost + arc.nArc);
                *(pPred + pos) = i;
            }
        }
    }

    // getting the shortest path
    int head = sink - source;
    int tail = *(pPred + head);
    cost = *(pCostLabel + head);

    while (tail != -1) {
        node = g.get_node(tail);
        for (auto &h : node.get_vector_headNode()) {
            if (h.node == head) {
                sol.arcs[idx].emplace(h.nArc);
                break;
            }
        }
        head = tail;
        tail = *(pPred + head);
    }

    if (head != 0) { // because 0 is the source
        error_no_path_in_shortest_path(jarvisObj, idx);
        return -1;
    }

    delete[] pCostLabel;
    delete[] pPred;

    return cost;
}


// function to compute the SP in each net
void shortest_path (Instance &instance, TempSolution &sol, jarvis::Jarvis &jarvisObj) {

    t_node sink;
    for (size_t g = 0; g < instance.gProjects.size(); g++) {
        sink = static_cast<t_node>(instance.gProjects[g].get_numNodes() - 1);
        shortest_path_acyclic (0, sink, instance.gProjects[g], jarvisObj, sol, g);
    }
}


// function to compute the SP in each PENALIZED net
void shortest_path_penalized_nets (Instance &instance, TempSolution &sol,
        jarvis::Jarvis &jarvisObj, std::unordered_set<unsigned int> &setNetsPenalized) {

    t_node sink;
    auto it = setNetsPenalized.begin();

    for (; it != setNetsPenalized.end(); it++) {
        sink = static_cast<t_node>(instance.gProjects[*it].get_numNodes() - 1);
        shortest_path_acyclic (0, sink, instance.gProjects[*it], jarvisObj, sol, *it);
    }

}


void select_k_worst_nets (std::unordered_set<unsigned int> &netsPenalized,
                          Input &input, Instance &instance, TempSolution &sol) {

    size_t numGraphs = instance.gProjects.size();
    size_t numNets = static_cast<int>(input.percExtraNets * numGraphs);
    std::unordered_set<unsigned int>::iterator it;
    std::list<std::pair<unsigned int, float> > nets;
    float cost;

    // Get the firsts numNets that are not already in netsPenalized
    size_t pos = 0;
    while (nets.size() < numNets && pos < numGraphs) {
        it = netsPenalized.find(pos);
        if (it == netsPenalized.end()) {
            cost = 0.0;
            for (auto &arc : sol.arcs[pos])
                cost += *(instance.pCost + arc);
            nets.emplace_back(std::make_pair(pos, cost));
        }
        pos++;
    }

    // sort by increasing cost
    nets.sort([]( const std::pair<unsigned int, float> &left,
                  const std::pair<unsigned int, float> &right )
              { return ( std::get<1>(left) > std::get<1>(right) ); });

    // go through the rest of the project and maintain an order
    for (size_t i = pos; i < numGraphs; i++) {
        it = netsPenalized.find(i);
        if (it == netsPenalized.end()) {
            cost = 0.0;
            for (auto &arc : sol.arcs[i])
                cost += *(instance.pCost + arc);
            // check if it can enter to the list
            if (cost > nets.back().second) {
                nets.back() = std::make_pair(i, cost);
                nets.sort([]( const std::pair<unsigned int, float> &left,
                              const std::pair<unsigned int, float> &right )
                          { return ( std::get<1>(left) > std::get<1>(right) ); });

            }
        }
    }

    // add them
    for (auto &n : nets)
        netsPenalized.emplace(n.first);
}


void select_k_random_nets (std::unordered_set<unsigned int> &netsPenalized,
                           Input &input, Instance &instance) {

    size_t upperBound = instance.gProjects.size();
    size_t numNets = static_cast<int>(input.percExtraNets * upperBound);
    int idThread = omp_get_thread_num();
    float randNum;
    unsigned int net;

    for (size_t i = 0; i < numNets; i++) {
        randNum = (float) rand_r(&*(input.pSeed + idThread)) / RAND_MAX;
        net = static_cast<unsigned int>(upperBound * randNum);
        netsPenalized.emplace(net);
    }
}


void get_solution_bool (Output &output, TempSolution &sol, size_t nSol) {

    output.refSet[nSol].arcsSol.clear();
    for (auto &arc : sol.arcs) {
        for (auto &a : arc) {
            output.refSet[nSol].arcsSol.emplace_back(a);
            if (!*(output.pSol + a)) {
                #pragma omp atomic write
                *(output.pSol + a) = true;
            }
        }
    }
}


// functions to use in Local_Search

void sort_k_bad_nets (std::vector<std::pair<unsigned int, float> > &nets,
                        Input &input, Output &output, Instance &instance) {

    std::vector<graph::Node>::iterator itNode, endNode;
    std::vector<graph::NodeElement>::iterator itArc, endArc;
    float cost;
    unsigned int pos = 0;
    size_t numNets = static_cast<int>(input.percBadNets * instance.gProjects.size());

    for (auto &g : instance.gProjects) {
        cost = -*(instance.pLB + pos);
        itNode  = g.get_iteratorToNodes();
        endNode = g.get_endToNodes();
        for (; itNode != endNode; itNode++) {
            itArc  = itNode->get_iterator_headNode();
            endArc = itNode->get_endIt_headNode();
            for (; itArc != endArc; itArc++) {
                if (*(output.pSol + itArc->nArc)) {
                    cost += *(instance.pCost + itArc->nArc);
                }
            }
        }
        nets.emplace_back(std::make_pair(pos, cost));
        pos++;
    }

    std::partial_sort (nets.begin(), nets.begin() + numNets, nets.end(),
            []( const std::pair<unsigned int, float> &left,
                    const std::pair<unsigned int, float> &right )
                 { return ( std::get<1>(left) > std::get<1>(right) ); });

    nets.erase(nets.begin() + numNets, nets.end());

}


float get_cost_network (Instance &instance, size_t idx, Output &output) {

    auto g = instance.gProjects[idx];
    float cost = 0.0;
    auto itNode  = g.get_iteratorToNodes();
    auto endNode = g.get_endToNodes();
    std::vector<graph::NodeElement>::iterator itArc, endArc;

    for (; itNode != endNode; itNode++) {
        itArc  = itNode->get_iterator_headNode();
        endArc = itNode->get_endIt_headNode();
        for (; itArc != endArc; itArc++) {
            if (*(output.pSol + itArc->nArc)) {
                cost += *(instance.pCost + itArc->nArc);
            }
        }
    }
    return cost;
}







// =======================
// SECONDARY FUNCTIONS
// =======================

void initialize_variables_pool(Instance &instance, Input &input, jarvis::Jarvis &jarvisObj,
                                TempSolution &sol) {
    sol.pCost      = new (std::nothrow) float [input.numArcs];
    sol.pPenalized = new (std::nothrow) bool  [input.numArcs];
    sol.arcs.resize(instance.gProjects.size());

    if (sol.pCost == nullptr || sol.pPenalized == nullptr) {
        error_pointers_feasible_sol(jarvisObj);
    }

    std::fill(sol.pPenalized, sol.pPenalized + input.numArcs, false);

    #pragma omp simd
    for (size_t i = 0; i < input.numArcs; i++)
        *(sol.pCost + i) = *(instance.pCost + i);
}

void error_pointers_feasible_sol (jarvis::Jarvis &jarvisObj) {
    int id = omp_get_thread_num();
    std::string msg = "Memory allocation failed in initialize_solutions()";
    msg += " in thread = " + std::to_string(id);

    #pragma omp critical
    {
        std::cout << "[ERROR]: " << msg << "\n";
        jarvisObj.logFile.write_log(msg, jarvis::Log::Error);
    }
}

void error_pointers_shortest_path (jarvis::Jarvis &jarvisObj) {
    int id = omp_get_thread_num();
    std::string msg = "Memory allocation failed in shortest_path_acyclic()";
    msg += " in thread = " + std::to_string(id);

    #pragma omp critical
    {
        std::cout << "[ERROR]: " << msg << "\n";
        jarvisObj.logFile.write_log(msg, jarvis::Log::Error);
    }


}

void error_no_path_in_shortest_path (jarvis::Jarvis &jarvisObj, int idx) {
    int id = omp_get_thread_num();
    std::string msg = "The path does not exist in shortest_path_acyclic(), net = ";
    msg += std::to_string(idx) + " in thread = " + std::to_string(id);

    #pragma omp critical
    {
        std::cout << "[ERROR]: " << msg << "\n";
        jarvisObj.logFile.write_log(msg, jarvis::Log::Error);
    }

}


void show_shortest_path(TempSolution &sol) {

    std::cout <<"\n\n ===============================\n";

    auto it = sol.arcs[0].begin();
    for (size_t i = 0; i < sol.arcs.size(); i++) {
        it = sol.arcs[i].begin();
        std::cout << "Net = " << i << "\n";
        for (; it != sol.arcs[i].end(); it++) {
            std::cout << *it << " ";
        }
        std::cout << "\n";
    }
    std::cout <<"\n\n ===============================\n\n\n";
}


void dummy_times(jarvis::Jarvis &jarvisObj) {
    // dummy times
    jarvisObj.timer.start_time("combiningSol");
    jarvisObj.timer.end_time("combiningSol");
    jarvisObj.timer.start_time("localSearch");
    jarvisObj.timer.end_time("localSearch");
}

void info_msg(jarvis::Jarvis &jarvisObj, const std::string &msg) {
    std::cout << "\n";
    print_info(jarvisObj, msg);
    std::cout << "\n";
}
