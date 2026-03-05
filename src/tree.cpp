#include "tree.h"

#include <iostream>
#include <cassert>

/*
    STEPS:
        - Start at root
        - one by one load particles
        - if any two particles fall inth esame cell, divide it into 8 cubical subcells (length l/2)

        root is always at index 0 in the node pool
*/

void Tree::add(double mass, double x, double y, double z) {
    this->internalAdd(0, mass, x, y, z);
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

void Tree::internalAdd(int root, double mass, double x, double y, double z, int depth) {

    if(depth > 1000) {
        std::cerr << "Too deep" << std::endl;
        exit(1);
    }

    Node& rootNode = pool[root];

    if(rootNode.length <= min_cell_size) {
        double total_mass = rootNode.mass + mass;
        rootNode.com_x = (rootNode.com_x * rootNode.mass + x * mass) / total_mass;
        rootNode.com_y = (rootNode.com_y * rootNode.mass + y * mass) / total_mass;
        rootNode.com_z = (rootNode.com_z * rootNode.mass + z * mass) / total_mass;
        rootNode.mass = total_mass;
        rootNode.state = NodeState::Leaf;
        return;
    }

    double dx = rootNode.com_x - x;
    double dy = rootNode.com_y - y;
    double dz = rootNode.com_z - z;
    double dist2 = dx*dx + dy*dy + dz*dz;
    
    if(dist2 < 1e-12) {
        rootNode.mass += mass;
        rootNode.com_x = (rootNode.com_x * (rootNode.mass - mass) + x * mass) / rootNode.mass;
        rootNode.com_y = (rootNode.com_y * (rootNode.mass - mass) + y * mass) / rootNode.mass;
        rootNode.com_z = (rootNode.com_z * (rootNode.mass - mass) + z * mass) / rootNode.mass;
        rootNode.state = NodeState::Leaf;
        return;
    }

    if(rootNode.state == NodeState::Empty) { // add to current node
        rootNode.mass = mass;
        
        rootNode.com_x = x;
        rootNode.com_y = y;
        rootNode.com_z = z;

        rootNode.state = NodeState::Leaf;
    } else if (rootNode.state == NodeState::Leaf) {
        // create children
        int first_child = allocate8Nodes();
        double quarter = rootNode.length / 4.0;
        
        for (int i = 0; i < 8; i++) {
            // std::cout << "Accessing child " << first_child + i << "in length " << max_nodes << std::endl;
            assert(first_child + i >= 0 && first_child + i < pool.size());
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

        // add particles to children: use same principle as bitwise and
        // add the new particle
        int child_index = getChildIndex(x, y, z, rootNode);
        internalAdd(first_child + child_index, mass, x, y, z, depth+1);

        // add the old particle
        child_index = getChildIndex(old_x, old_y, old_z, rootNode); 
        internalAdd(first_child + child_index, old_mass, old_x, old_y, old_z, depth + 1);

        double new_mass = 0.0, cx=0, cy=0, cz=0;
        for(int i = 0; i < 8; i++) {
            assert(first_child + i >= 0 && first_child + i < pool.size());
            Node& c = pool[first_child+i];
            if(c.state == NodeState::Empty) continue;

            new_mass += c.mass;
            cx += c.mass * c.com_x;
            cy += c.mass * c.com_y;
            cz += c.mass * c.com_z;
        }
        rootNode.mass = new_mass;
        rootNode.com_x = cx / new_mass;
        rootNode.com_y = cy / new_mass;
        rootNode.com_z = cz / new_mass;

        rootNode.first_child = first_child;
        rootNode.state = NodeState::Internal;
    } else if (rootNode.state == NodeState::Internal) {
        int child_index = getChildIndex(x, y, z, rootNode);

        internalAdd(rootNode.first_child + child_index, mass, x, y, z, depth + 1);

        double new_mass = 0.0, cx=0, cy=0, cz=0;
        for(int i = rootNode.first_child; i < rootNode.first_child + 8; i++){
                assert(i>= 0 && i < pool.size());
                Node& c = pool[i];

                if(c.state == NodeState::Empty) continue;

                new_mass += c.mass;
                cx += c.mass * c.com_x;
                cy += c.mass * c.com_y;
                cz += c.mass * c.com_z;
        }

        rootNode.mass = new_mass;
        rootNode.com_x = cx / new_mass;
        rootNode.com_y = cy / new_mass;
        rootNode.com_z = cz / new_mass;
    }
}

int Tree::getChildIndex(double x, double y, double z, const Node &node) {
    int child_index = 0;
    if(x > node.center_x) child_index += 1;
    if(y > node.center_y) child_index += 2;
    if(z > node.center_z) child_index += 4;

    return child_index;
};

std::tuple<double, double, double> Tree::traverse(double mass, double x, double y, double z)
{   
    double ax = 0, ay = 0, az = 0;

    internalTraverse(0, mass, x, y, z, ax, ay, az);

    return {ax, ay, az};
}

void Tree::internalTraverse(int root, double mass, double x, double y, double z, double& ax, double& ay, double& az)
{
    Node& rootNode = pool[root];
    
    double l = rootNode.length;

    double dx = rootNode.com_x - x;
    double dy = rootNode.com_y - y;
    double dz = rootNode.com_z - z;
    
    double r2 = dx*dx + dy*dy + dz*dz + eps2;

    if(rootNode.state == NodeState::Empty) return;

    if( l * l < theta2 * r2 || rootNode.state == NodeState::Leaf) {
        // if not the same particle, calculate
        if(r2 != eps2){
            double inv_r = 1.0 / sqrt(r2);
            double inv_r3 = inv_r * inv_r * inv_r;

            double s = rootNode.mass * inv_r3;
            
            ax += s * dx;
            ay += s * dy;
            az += s * dz;
        }
    } else {
        int firstChild = rootNode.first_child;
        if (firstChild == -1)
            return;

        for (int i = 0; i < 8; ++i){
            if(pool[rootNode.first_child + i].state != NodeState::Empty)
                internalTraverse(rootNode.first_child + i, mass, x, y, z, ax, ay, az);   
        }
    }
}

