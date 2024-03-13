#include "executor/chassis_executor.hpp"

ChassisExecutor::ChassisExecutor(const sp_decision::Blackboard::Ptr &blackboard_ptr)
{
    blackboard_ = blackboard_ptr;
    nh_.param<double>("/max_vel_theta", max_vel_theta_, 3.14);
    set_goal_pub_ =
        nh_.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1);
    robot_state_pub_ =
        nh_.advertise<robot_msg::RobotStateMsg>("/robot_state", 1);
    sentry_cmdvel_pub_ =
        nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
}
void ChassisExecutor::robotStatePub(RobotState robot_state)
{
    robot_msg::RobotStateMsg robot_state_msg;
    robot_state_msg.robot_state = static_cast<int8_t>(robot_state);
    robot_state_pub_.publish(robot_state_msg);
}
void ChassisExecutor::SendDataToPlan(double pos_x, double pos_y)
{
    if (GetMoveStatus())
    {
        ROS_INFO("reached");
    }
    if (target_pose_.pose.position.x != pos_x || target_pose_.pose.position.y != pos_y)
    {
        target_pose_.header.frame_id = "map";
        target_pose_.header.stamp = ros::Time::now();
        target_pose_.pose.position.x = pos_x;
        target_pose_.pose.position.y = pos_y;
        target_pose_.pose.orientation.x = 0.0;
        target_pose_.pose.orientation.y = 0.0;
        target_pose_.pose.orientation.z = 0.0;
        target_pose_.pose.orientation.w = 1.0;
        goal_.target_pose = target_pose_;
        set_goal_pub_.publish(goal_);
    }
}
bool ChassisExecutor::GetMoveStatus()
{
    double distance = sqrt(pow(blackboard_->robot_pose_.pose.pose.position.x - target_pose_.pose.position.x, 2) + pow(blackboard_->robot_pose_.pose.pose.position.y - target_pose_.pose.position.y, 2));
    ROS_INFO("distance %f", distance);
    if (distance < 0.15)
    {
        move_status = true;
        return 1;
    }
    else
    {
        move_status = false;
        return 0;
    }
}
bool ChassisExecutor::Move(double pos_x, double pos_y)
{
    robotStatePub(RobotState::MOVE);
    SendDataToPlan(pos_x, pos_y);
    ros::Duration(0.2).sleep();
    return blackboard_->plan_get_;
}
void ChassisExecutor::QueueMove(std::vector<sp_decision::Blackboard::Point> points, sp_decision::Blackboard::Action_Lock action, int stay_time)
{
    robotStatePub(RobotState::CRUISR);
    if (action_status != action)
    {
        num = -1;
    }
    if (num == -1)
    {
        action_status = action;
        for (int i = 0; i < points.size(); i++)
        {
            SendDataToPlan(points[0].x, points[0].y);
            ros::Duration(0.2).sleep();
            num++;
            if (blackboard_->plan_get_)
                break;
        }
    }
    else if (GetMoveStatus())
    {
        // 在目标点停留时间
        if (stay_time > 0)
        {
            VelIdle();
            ros::Duration(stay_time).sleep();
        }
        for (int i = num + 1; i < 1000; i++)
        {
            num = i % points.size();
            SendDataToPlan(points[num].x, points[num].y);
            ros::Duration(0.2).sleep();
            if (blackboard_->plan_get_)
            {
                break;
            }
        }
    }
}
bool ChassisExecutor::FastMove(double pos_x, double pos_y)
{

    robotStatePub(RobotState::FAST);
    SendDataToPlan(pos_x, pos_y);
    ros::Duration(0.2).sleep();
    return blackboard_->plan_get_;
}
void ChassisExecutor::Cruisr(double pos_x, double pos_y)
{
    robotStatePub(RobotState::CRUISR);
    SendDataToPlan(pos_x, pos_y);
}
void ChassisExecutor::Pursuit(double pos_x, double pos_y)
{
    robotStatePub(RobotState::PURSUIT);
    SendDataToPlan(pos_x, pos_y);
}
void ChassisExecutor::VelIdle()
{
    robotStatePub(RobotState::ROTATE);
    SendDataToPlan(0, 0);
}
void ChassisExecutor::VelStop()
{
}
void ChassisExecutor::Stop()
{
    robotStatePub(RobotState::STOP);
    VelStop();
}
void ChassisExecutor::Idle()
{
    robotStatePub(RobotState::IDLE);
    VelIdle();
}
