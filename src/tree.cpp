#include "tree.h"

/*
    STEPS:
        - Start at root
        - one by one load particles
        - if any two particles fall inth esame cell, divide it into 8 cubical subcells (length l/2)

        root is always at index 0 in the node pool
*/

void Tree::add(double mass, double x, double y, double z) {
    int root = 0;
    this->internalAdd(root, mass, x, y, z);
}


/*
    How the BITWISE AND works

     i     i&1     i&2     i&4
    000     0       0       0
    001     1       0       0
    010     0       2       0
    011     1       2       0
    100     0       0       4
    101     1       0       4
    110     0       2       4
    111     1       2       4

    Where:
        i&1 == 0 means left (x-quarter)
        i&2 == 0 means down (y-quarter)
        i&4 == 9 means back (z-quarter)

    Adding particles to children:
        if(x > rootNode.center_x) child_index += 1;
        if(y > rootNode.center_y) child_index += 2;
        if(z > rootNode.center_z) child_index += 4;

    0 <= child_index <= 7, corresponding to the child
*/

void Tree::internalAdd(int root, double mass, double x, double y, double z) {
    Node& rootNode = pool[root];
    if(rootNode.state == NodeState::Empty) { // add to current node
        
        rootNode.mass = mass;
        
        rootNode.com_x = x;
        rootNode.com_y = y;
        rootNode.com_z = z;

        rootNode.state = NodeState::Leaf;
    } else if (rootNode.state == NodeState::Leaf) {
        // create children
        int first_child = this->allocate8Nodes();
        double quarter = rootNode.length / 4;
        
        for (int i = 0; i < 8; i++) {

            Node& child = pool[first_child + i];

            child.length = rootNode.length / 2.0;

            // bitwise and operation
            child.center_x = rootNode.center_x + ((i & 1) ? quarter : -quarter);
            child.center_y = rootNode.center_y + ((i & 2) ? quarter : -quarter);
            child.center_z = rootNode.center_z + ((i & 4) ? quarter : -quarter);
        }

        // previous particle:
        double old_mass = rootNode.mass;
        double old_x = rootNode.com_x;
        double old_y = rootNode.com_y;
        double old_z = rootNode.com_z;

        // set up the new values for the root
        rootNode.mass = old_mass + mass;
        rootNode.com_x = (old_x * old_mass + x * mass) / rootNode.mass;
        rootNode.com_y = (old_y * old_mass + y * mass) / rootNode.mass;
        rootNode.com_z = (old_z * old_mass + z * mass) / rootNode.mass;

        // add particles to children: use same principle as bitwise and
        // add the new particle
        int child_index = getChildIndex(x, y, z, rootNode);
        
        this->internalAdd(first_child + child_index, mass, x, y, z);

        // add the old particle
        child_index = getChildIndex(old_x, old_y, old_z, rootNode); 

        this->internalAdd(first_child + child_index, old_mass, old_x, old_y, old_z);

        rootNode.first_child = first_child;
        rootNode.state = NodeState::Internal;
    } else if (rootNode.state == NodeState::Internal) {
        int child_index = getChildIndex(x, y, z, rootNode);

        this->internalAdd(rootNode.first_child + child_index, mass, x, y, z);

        double new_mass = 0;
        double new_com_x = 0;
        double new_com_y = 0;
        double new_com_z = 0;
        for(int i = rootNode.first_child; i < rootNode.first_child + 8; i++){
            if(pool[i].state == NodeState::Empty) continue;;

            new_mass += pool[i].mass;
            new_com_x += pool[i].mass * pool[i].com_x;
            new_com_y += pool[i].mass * pool[i].com_y;
            new_com_z += pool[i].mass * pool[i].com_z;
        }

        rootNode.mass = new_mass;
        rootNode.com_x = new_com_x / new_mass;
        rootNode.com_y = new_com_y / new_mass;
        rootNode.com_z = new_com_z / new_mass;
    }
}

int Tree::getChildIndex(double x, double y, double z, const Node &node) {
    int child_index = 0;
    if(x > node.center_x) child_index += 1;
    if(y > node.center_y) child_index += 2;
    if(z > node.center_z) child_index += 4;

    return child_index;
};