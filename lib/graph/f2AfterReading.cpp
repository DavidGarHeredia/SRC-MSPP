//
// Created by david on 6/30/19.
//


#include "Graph.h"


namespace graph {

    void Graph::make_adjacency_list (std::vector<Arc>& arcs) {

        std::string msg = "[INFO]: Making adjacency list.";
        write_log(msg, Info);
        msg = "[WARNING]: It is responsibility of the user to make sure that ";
        msg += "no arc is repeated.";
        write_log(msg, Warning);

        // get the list of nodes
        struct Pair { std::vector<Arc> ingoing, outgoing; };
        std::map<t_node, Pair> mapNodes;

        // save nodes
        for (auto &a : arcs) {
            mapNodes[a.tail].outgoing.push_back(a);
            mapNodes[a.head].ingoing.push_back(a);
        }

        // load nodes in the list
        auto it = mapNodes.begin();
        std::vector<Arc>::iterator it2;

        for (; it != mapNodes.end(); it++) {
            _nodes.emplace_back(it->first);

            it2 = it->second.outgoing.begin();
            for (; it2 != it->second.outgoing.end(); it2++) {
                _nodes.back().set_headNode(it2->head, it2->numArc);
            }

            it2 = it->second.ingoing.begin();
            for (; it2 != it->second.ingoing.end(); it2++) {
                _nodes.back().set_tailNode(it2->tail, it2->numArc);
            }
        }

        sort_list_of_nodes();
        count_numArcs();

        msg = "[INFO]: Adjacency list created.";
        write_log(msg, Info);
    }


    void Graph::sort_list_of_nodes () {

        std::sort(_nodes.begin(), _nodes.end(),
                  [](Node &left, Node &right) {
                      return left.get_node() < right.get_node();}
                  );
    }

    size_t Graph::count_numArcs () {
        _numArcs = 0;
        for (auto &n : _nodes)
            _numArcs += n.get_num_headNode();

        return _numArcs;
    }


}
