#ifndef TREE_H
#define TREE_H

#include <vector>
#include <math.h>
#include <iostream>

#include "node.h"

class Tree {
    public:
        // allocates the node pool and creates the root node
        Tree(std::vector<Node>& poolRef, size_t particle_count, int length, double eps2, double theta=0.5)
        :max_nodes(particle_count * 8), eps2(eps2), pool(poolRef), theta2(theta*theta)
        {
            // pool.resize(max_nodes);

            next_free = 0;
            int root = allocateNode();
            pool[root].length = length * 2;
        }

        void reset(int length) { 
            next_free = 0; 
            
            int root = allocateNode();
            pool[root].length = length * 2;
        }

        void add(double mass, double x, double y, double z);
        std::tuple<double, double, double> traverse(double mass, double x, double y, double z);

    private:
        std::vector<Node>& pool;

        size_t next_free = 0;
        size_t max_nodes;
        double min_cell_size = 1e-6;

        double eps2;
        double theta2;

        int allocateNode() { 
            int new_node = next_free++;
            resetNode(pool[new_node]);
            return new_node; 
        }       

        int allocate8Nodes() {
            // std::cout << "Allocating 8 nodes at " << next_free << std::endl; 
            if (next_free + 8 > max_nodes) {
                std::cerr << "Tree capacity exceeded\n";
                exit(1);
            }

            int start = next_free;
            next_free += 8;

            for(int i = 0; i < 8; i++) resetNode(pool[start + i]);
                // pool[start + i].state = NodeState::Empty;

            return start;
        }
        
        void internalAdd(int root, double mass, double x, double y, double z, int depth = 0);
        void internalTraverse(int root, double mass, double x, double y, double z, double& ax, double& ay, double& az);
        int getChildIndex(double x, double y, double z, const Node& node);
};

#endif /* TREE_H */