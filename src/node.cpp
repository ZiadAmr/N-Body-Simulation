#include "node.h"


void resetNode(Node& node){
    node.com_x = 0;
    node.com_y = 0;
    node.com_z = 0;

    node.center_x = 0;
    node.center_y = 0;
    node.center_z = 0;

    node.mass = 0;

    node.first_child = -1;

    node.state = NodeState::Empty;
};