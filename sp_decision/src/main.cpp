#include "node/decision_node.h"
#include "iostream"
#include "ros/ros.h"

int main(int argc, char **argv)
{
    ros::init(argc,argv,"sp_decision_node");
    sp_decision::DecisionNode decision;
    ros::spin();
    return 0;
}