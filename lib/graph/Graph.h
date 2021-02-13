#ifndef GRAPHLIBRARY3_0_LIBRARY_H
#define GRAPHLIBRARY3_0_LIBRARY_H

#include<vector>
#include<map>
#include<queue>
#include<iterator>

#include<string>
#include<fstream>
#include<iostream>

#include <algorithm>
#include <limits>

#include <omp.h>



typedef unsigned int t_node;



namespace graph {

    struct Arc {
        t_node tail, head, numArc;
    };

    struct NodeElement {
        t_node node, nArc;
    };

    class Node {

    private:
        t_node _node;
        std::vector<NodeElement> _headNode, _tailNode;

    public:
        //---------------------------------
        // Constructor and destructor
        //---------------------------------
        Node () = default;

        explicit Node (t_node _node) : _node(_node) {}

        virtual ~Node () = default;

        //---------------------------------
        // Node
        //---------------------------------
        t_node get_node () const { return _node; }

        void set_node (t_node _node) { Node::_node = _node; }

        //---------------------------------
        // Head nodes
        //---------------------------------
        t_node get_headNode (size_t pos) { return _headNode[pos].node; }

        void set_headNode (t_node head, t_node nArc) { _headNode.push_back({head, nArc}); }

        void change_headNode (t_node node, size_t pos) { _headNode[pos].node = node; }

        t_node get_numArc_headNode (size_t pos) { return _headNode[pos].nArc; }

        size_t get_num_headNode () { return _headNode.size(); }

        const std::vector<NodeElement> &get_vector_headNode () const { return _headNode; }

        std::vector<NodeElement>::iterator get_iterator_headNode () {
            auto it = _headNode.begin();
            return it;
        }

        std::vector<NodeElement>::iterator get_endIt_headNode () {
            auto it = _headNode.end();
            return it;
        }

        //---------------------------------
        // Tail node
        //---------------------------------
        t_node get_tailNode (size_t pos) { return _tailNode[pos].node; }

        void set_tailNode (t_node tail, t_node nArc) { _tailNode.push_back({tail, nArc}); }

        void change_tailNode (t_node node, size_t pos) { _tailNode[pos].node = node; }

        t_node get_numArc_tailNode (size_t pos) { return _tailNode[pos].nArc; }

        size_t get_num_tailNode () { return _tailNode.size(); }

        const std::vector<NodeElement> &get_vector_tailNode () const { return _tailNode; }

        std::vector<NodeElement>::iterator get_iterator_tailNode () {
            auto it = _tailNode.begin();
            return it;
        }

        std::vector<NodeElement>::iterator get_endIt_tailNode () {
            auto it = _tailNode.end();
            return it;
        }

    };


    class Graph {

    public:
        enum logInfo {Nothing, Error, Warning, Info};

    private:
        size_t _numArcs{};
        std::vector<Node> _nodes;

        std::string _logfilename = "logGraph.txt";
        logInfo _logstatus = Nothing;

    public:
        Graph () = default;

        ~Graph () {
            write_log("\n\n", Error);
            write_log("=====================================================", Error);
            write_log("=====================================================", Error);
            write_log("=====================================================", Error);
            write_log("\n\n\n\n", Error);
        }


        //---------------------------------
        // getters
        //---------------------------------
        size_t get_numNodes () const { return _nodes.size(); }

        size_t get_numArcs  () const { return _numArcs; }

        size_t count_numArcs ();

        logInfo get_logstatus () const { return _logstatus; }

        const std::string &get_logfilename () const { return _logfilename; }

        std::vector<Node>::iterator get_iteratorToNodes () {
            auto it = _nodes.begin();
            return it;
        }

        std::vector<Node>::iterator get_endToNodes () {
            auto it = _nodes.end();
            return it;
        }

        Node get_node (size_t pos) { return _nodes[pos]; }

        //---------------------------------
        // setters
        //---------------------------------
        void set_logfilename (std::string logfilename) { _logfilename = logfilename; }

        void set_logstatus (logInfo logstatus) { _logstatus = logstatus; }

        void sort_list_of_nodes ();

        // functions to write and export data
        void write_log (std::string msg, logInfo level);

        void show_adjacency_list (bool all, size_t numRows);

        //---------------------------------
        // functions to use after reading the data
        //---------------------------------
        void make_adjacency_list (std::vector<Arc>& arcs);

    };
}


#endif
