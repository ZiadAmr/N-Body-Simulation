#ifndef TREE_H
#define TREE_H

#include <vector>

#include "node.h"

class Tree {
    public:
        // allocates the node pool and creates the root node
        Tree(size_t particle_count, int length, double theta=0.5)
        :max_nodes(particle_count * 8)
        {
            pool.resize(max_nodes);

            int root = allocateNode();
            pool[root].length = length * 2;
        }

        void reset(int length) { 
            next_free = 0; 
            
            int root = allocateNode();
            pool[root].length = length * 2;
        }

        void add(double mass, double x, double y, double z);

    private:
        std::vector<Node> pool;
        size_t next_free = 0;
        size_t max_nodes;

        double theta;

        int allocateNode() { 
            int new_node = next_free++;
            // resetNode(pool[new_node]);
            return new_node; 
        }       
        int allocate8Nodes() {
            int old_val = next_free;
            next_free += 8;
            // for(int i = 0; i < 8; i++){
            //     resetNode(pool[old_val + i]);
            // }   
            return old_val;
        }
        
        void internalAdd(int root, double mass, double x, double y, double z);
};

#endif /* TREE_H */