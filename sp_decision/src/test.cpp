#include <ros/ros.h>
#include "robot_msg/EnemyStage.h" // 导入自定义消息类型
#include "robot_msg/CmdGimbal.h" // 导入自定义消息类型

int main(int argc, char **argv)
{
    // 初始化 ROS 节点
    ros::init(argc, argv, "enemy_publisher_node");
    ros::NodeHandle nh;

    // 创建发布者对象，发布类型为 robot_msg::EnemyStage 的消息，话题名为 "/enemy_stage"，队列大小为 1
    ros::Publisher enemy_pub = nh.advertise<robot_msg::EnemyStage>("/enemy_stage", 1);
    ros::Publisher gimbal_pub = nh.advertise<robot_msg::CmdGimbal>("/sentry/cmd_gimbal", 1);
    // 创建一个自定义消息对象
    robot_msg::EnemyStage enemy_msg;
    robot_msg::CmdGimbal gimbal;
    std::string ss = "0,1,3";
    enemy_msg.ss = ss;
    gimbal.yaw_min=0;
    gimbal.yaw_max=90;
    gimbal.pitch_min=-10;
    gimbal.pitch_max=20;
    // 设置发布频率
    ros::Rate loop_rate(1); // 1Hz

    while (ros::ok())
    {
        // 发布消息
        enemy_pub.publish(enemy_msg);
        gimbal_pub.publish(gimbal);
        // 输出发布的消息内容（可选）
        ROS_INFO("Published enemy information");

        // 循环等待
        ros::spinOnce();

        // 按照设定的频率延时
        loop_rate.sleep();
    }

    return 0;
}
