#ifndef NODE_H
#define NODE_H

enum NodeState {
    Empty,
    Leaf,
    Internal
};

struct Node {
    double com_x = 0;
    double com_y = 0;
    double com_z = 0;

    double center_x = 0;
    double center_y = 0;
    double center_z = 0;

    double mass = 0;

    double length;

    int first_child = -1;

    NodeState state = NodeState::Empty;
};


// void resetNode(Node& node){
//     node.com_x = 0;
//     node.com_y = 0;
//     node.com_z = 0;

//     node.center_x = 0;
//     node.center_y = 0;
//     node.center_z = 0;

//     node.mass = 0;

//     node.first_child = -1;

//     node.state = NodeState::Empty;
// }
/*
    Why not to use new to allocate new node:
        - cache performance

*/

#endif