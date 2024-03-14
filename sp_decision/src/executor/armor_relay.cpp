#include "executor/armor_relay.hpp"
int main(int argc, char **argv)
{
    ros::init(argc, argv, "armor_transformer");
    ros::NodeHandle nh;
    tf2_ros::Buffer tfBuffer;
    tf2_ros::TransformListener tfListener(tfBuffer);
    ros::Publisher armor_pub_ = nh.advertise<robot_msg::Armor>("/armor", 1);
    robot_msg::Armor armor_sentry;
    armor_sentry.number = 7;
    armor_sentry.type = "sentry";

    geometry_msgs::PoseStamped src_pose;
    src_pose.header.frame_id = "world_in_auto_aim"; // 源坐标系
    src_pose.pose.orientation.x = 0.0;
    src_pose.pose.orientation.y = 0.0;
    src_pose.pose.orientation.z = 0.0;
    src_pose.pose.orientation.w = 1.0;
    geometry_msgs::PoseStamped dst_pose;

    TCPServer server;
    if (!server.start())
    {
        std::cerr << "启动服务器失败" << std::endl;
        return 1;
    }
    // 接受客户端连接
    if (server.acceptConnection())
    {
        while (ros::ok)
        {
            std::string receivedData = server.receive();
            std::vector<std::string> splittedData = server.splitString(receivedData, ',');
            std::cout << "Received data: ";
            std::vector<double> data;
            for (const auto &str : splittedData)
            {
                data.push_back(std::stod(str)); // 分别为跟踪状态，x,y,z坐标
            }
            armor_sentry.track_status = data[0];
            src_pose.pose.position.x = data[1];
            src_pose.pose.position.y = data[2];
            src_pose.pose.position.z = data[3];
            if (data.size() == 4)
            {
                try
                {
                    geometry_msgs::TransformStamped body2camera_init;
                    body2camera_init = tfBuffer.lookupTransform("camera_init", "body", ros::Time(0));
                    body2camera_init.transform.rotation.w = 1;
                    body2camera_init.transform.rotation.x = 0;
                    body2camera_init.transform.rotation.y = 0;
                    body2camera_init.transform.rotation.z = 0;
                    tf2::doTransform(src_pose, dst_pose, body2camera_init);
                    geometry_msgs::TransformStamped camera_init2map;
                    camera_init2map = tfBuffer.lookupTransform("map", "camera_init", ros::Time(0));
                    tf2::doTransform(dst_pose, dst_pose, body2camera_init);
                    // 输出转换后的姿态
                    armor_sentry.pose.position.x = dst_pose.pose.position.x;
                    armor_sentry.pose.position.y = dst_pose.pose.position.y;
                    armor_sentry.pose.position.z = dst_pose.pose.position.z;
                    ROS_INFO("Transformed pose: x=%f, y=%f, z=%f",
                             armor_sentry.pose.position.x,
                             armor_sentry.pose.position.y,
                             armor_sentry.pose.position.z);
                    armor_pub_.publish(armor_sentry);
                }
                catch (tf2::TransformException &ex)
                {
                    ROS_WARN("Failed to transform pose: %s", ex.what());
                }
            }
        }
    }

    return 0;
}