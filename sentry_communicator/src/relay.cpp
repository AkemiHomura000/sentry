/*
该节点用于单独调试导航程序，订阅cmd_vel话题并发布sentry/cmd_vel话题
*/
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

class VelocityRepublisher
{
public:
    VelocityRepublisher(ros::NodeHandle &nh)
    {
        cmd_vel_sub_ = nh.subscribe("/cmd_vel", 1, &VelocityRepublisher::cmdVelCallback, this);
        sentry_cmd_vel_pub_ = nh.advertise<geometry_msgs::Twist>("/sentry/cmd_vel", 1);
    }
    void cmdVelCallback(const geometry_msgs::Twist::ConstPtr &msg)
    {
        sentry_cmd_vel_pub_.publish(msg);
         ROS_INFO("linear.x=%f,linear.y=%f,linear.z=%f, angular.z=%f", msg->linear.x, msg->linear.y,msg->linear.z,msg->angular.z);
    }

private:
    ros::Subscriber cmd_vel_sub_;
    ros::Publisher sentry_cmd_vel_pub_;
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "velocity_republisher_node");
    ros::NodeHandle nh;
    VelocityRepublisher velocity_republisher(nh);
    ros::spin();
    return 0;
}
