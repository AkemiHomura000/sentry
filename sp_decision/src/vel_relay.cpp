#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <robot_msg/RobotStateMsg.h>
#include "robot_msg/CmdGimbal.h" // 导入自定义消息类型
ros::Publisher sentry_cmd_vel_pub;
ros::Publisher gimbal_pub_;
geometry_msgs::Twist current_cmd_vel;
enum RobotState
{
    MOVE,
    CRUISR,
    IDLE,
    FAST,
    STOP,
    ROTATE,
    PURSUIT,
};
robot_msg::RobotStateMsg state;
void cmdVelCallback(const geometry_msgs::Twist::ConstPtr &msg)
{
    current_cmd_vel = *msg;

    // if (state.robot_state == RobotState::ROTATE)
    // {
    //     current_cmd_vel.linear.x = 0.0;
    //     current_cmd_vel.linear.y = 0.0;
    //     current_cmd_vel.angular.z = 5;
    // }
    // if (state.robot_state == RobotState::STOP)
    // {
    //     current_cmd_vel.linear.x = 0.0;
    //     current_cmd_vel.linear.y = 0.0;
    //     current_cmd_vel.angular.z = 0;
    // }
    // if (state.robot_state == RobotState::MOVE)
    // {
    //     current_cmd_vel.linear.x = msg->linear.x * 0.5;
    //     current_cmd_vel.linear.y = msg->linear.y * 0.5;
    //     current_cmd_vel.angular.z = 5;
    // }
    current_cmd_vel.linear.x = msg->linear.x * 0.4;
    current_cmd_vel.linear.y = msg->linear.y * 0.4;
    current_cmd_vel.angular.z = -5;
    sentry_cmd_vel_pub.publish(current_cmd_vel);
    robot_msg::CmdGimbal gimbal;
    gimbal.yaw_min = -180;
    gimbal.yaw_max = 180;
    gimbal.pitch_max = 0;
    gimbal.pitch_min = 0;
    gimbal_pub_.publish(gimbal);
}

void robotStateCallback(const robot_msg::RobotStateMsg::ConstPtr &state_msg)
{
    // 根据RobotStateMsg的内容修改current_cmd_vel的值
    // 示例：如果机器人状态为停止，则将线速度和角速度都设置为0
    state.robot_state = state_msg->robot_state;
}

int main(int argc, char **argv)
{
    // 初始化ROS节点
    ros::init(argc, argv, "cmd_vel_relay_node");
    ros::NodeHandle nh;

    // 创建一个订阅者，订阅/cmd_vel话题
    ros::Subscriber cmd_vel_sub = nh.subscribe("/cmd_vel", 1, cmdVelCallback);

    // 创建一个发布者，发布到/sentry/cmd_vel话题
    sentry_cmd_vel_pub = nh.advertise<geometry_msgs::Twist>("/sentry/cmd_vel", 1);
    gimbal_pub_ = nh.advertise<robot_msg::CmdGimbal>("/sentry/cmd_gimbal", 1);
    // 创建一个订阅者，订阅/robot_state话题
    ros::Subscriber robot_state_sub = nh.subscribe<robot_msg::RobotStateMsg>("/robot_state", 1, robotStateCallback);

    // 循环等待回调函数
    ros::spin();

    return 0;
}
