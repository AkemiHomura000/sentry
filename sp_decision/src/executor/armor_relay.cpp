#include "executor/armor_relay.hpp"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "pose_transformer");
    ros::NodeHandle nh;
    tf2_ros::Buffer tfBuffer;
    tf2_ros::TransformListener tfListener(tfBuffer);
    geometry_msgs::PoseStamped src_pose;
    src_pose.header.frame_id = "body"; // 源坐标系
    src_pose.pose.position.x = 0.0;
    src_pose.pose.position.y = 0.0;
    src_pose.pose.position.z = 0.0;
    src_pose.pose.orientation.x = 0.0;
    src_pose.pose.orientation.y = 0.0;
    src_pose.pose.orientation.z = 0.0;
    src_pose.pose.orientation.w = 1.0;
    geometry_msgs::PoseStamped dst_pose;
    while (ros::ok)
    {
        try
        {
            geometry_msgs::TransformStamped body2map;
            body2map = tfBuffer.lookupTransform("map", "body", ros::Time(0));
            // 使用 tf2::doTransform() 函数进行坐标转换
            tf2::doTransform(src_pose, dst_pose, body2map);
            // 输出转换后的姿态
            ROS_INFO("Transformed pose: x=%f, y=%f, z=%f",
                     dst_pose.pose.position.x,
                     dst_pose.pose.position.y,
                     dst_pose.pose.position.z);
        }
        catch (tf2::TransformException &ex)
        {
            ROS_WARN("Failed to transform pose: %s", ex.what());
        }
    }
    return 0;
}