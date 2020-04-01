//
// Created by david on 6/30/19.
//

#include "Graph.h"

namespace graph {

    void Graph::write_log (std::string msg, logInfo level) {

        if (level <= _logstatus) {
            std::fstream fs;
            fs.open (_logfilename, std::fstream::out | std::fstream::app);
            fs << msg << '\n';
            fs.close();
        }

    }


    void Graph::show_adjacency_list (bool all, size_t numRows) {

        size_t counter = 0;

        for (auto &n : _nodes) {
            std::cout << "Node = " << n.get_node() << "\n";

            std::cout << "==> ingoing: ";
            for (auto &nIn : n.get_vector_tailNode()) {
//                std::cout << "[" << nIn.node << "-" << nIn.nArc << "]" << " ";
                std::cout << "[" << nIn.node << "]" << " ";
            }

            std::cout << "==> outgoing: ";
            for (auto &nOut : n.get_vector_headNode()) {
//                std::cout << "[" << nOut.node << "-" << nOut.nArc << "]" << " ";
                std::cout << "[" << nOut.node << "]" << " ";
            }

            std::cout << "\n";

            counter++;
            if (!all && counter >= numRows) { break; }
        }

    }





}