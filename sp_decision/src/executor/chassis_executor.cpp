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
    gimbal_pub_ = nh_.advertise<robot_msg::CmdGimbal>("/sentry/cmd_gimbal", 1);
    target_pose_.pose.position.x = 100000;
    target_pose_.pose.position.y = 100000; // 确保初值不会和第一个点冲突
}
void ChassisExecutor::robotStatePub(RobotState robot_state)
{
    robot_msg::RobotStateMsg robot_state_msg;
    robot_state_msg.robot_state = static_cast<int8_t>(robot_state);
    robot_state_pub_.publish(robot_state_msg);
}
Implementation ChassisExecutor::SendDataToPlan(double pos_x, double pos_y)
{

    // double distance = sqrt(pow(blackboard_->robot_pose_.pose.pose.position.x - pos_x, 2) + pow(blackboard_->robot_pose_.pose.pose.position.y - pos_y, 2));
    // if (target_pose_.pose.position.x != pos_x || target_pose_.pose.position.y != pos_y || (pos_x == 0 && pos_y == 0)) // 持续发布目标点，否则动态避障不生效
    // {
    //     last_time = ros::Time::now();
    //     target_pose_.header.frame_id = "map";
    //     target_pose_.header.stamp = ros::Time::now();
    //     target_pose_.pose.position.x = pos_x;
    //     target_pose_.pose.position.y = pos_y;
    //     target_pose_.pose.orientation.x = 0.0;
    //     target_pose_.pose.orientation.y = 0.0;
    //     target_pose_.pose.orientation.z = 0.0;
    //     target_pose_.pose.orientation.w = 1.0;
    //     goal_.target_pose = target_pose_;
    //     set_goal_pub_.publish(goal_);
    // }

    if (target_pose_.pose.position.x != pos_x || target_pose_.pose.position.y != pos_y) // 不是上一次的目标点
    {
        blackboard_->last_position.x = blackboard_->robot_pose_.pose.pose.position.x;
        blackboard_->last_position.y = blackboard_->robot_pose_.pose.pose.position.y;
        blackboard_->last_position.z = blackboard_->robot_pose_.pose.pose.position.z; // 初始化开始时位置
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
        ros::Duration(0.2).sleep(); // 等待0.1s？
        if (blackboard_->vel_msg_sub_.linear.x == 0 && blackboard_->vel_msg_sub_.linear.y == 0)
        {
            exec_stauts = Implementation::FAILED;
            return Implementation::FAILED;
        }
        exec_stauts = Implementation::IN_PROGRESS;
        return Implementation::IN_PROGRESS;
    }
    else
    {
        if (GetMoveStatus()) // 距离小于0.15米认为到达
        {
            exec_stauts = Implementation::FINISHED;
            return Implementation::FINISHED;
        }
        else
        {
            // 计算位移变化量
            double delta_distance = sqrt(pow(blackboard_->robot_pose_.pose.pose.position.x - blackboard_->last_position.x, 2) + pow(blackboard_->robot_pose_.pose.pose.position.y - blackboard_->last_position.y, 2));
            if (delta_distance > 0.01 || exec_stauts == Implementation::FAILED)
            {
                blackboard_->last_position.x = blackboard_->robot_pose_.pose.pose.position.x;
                blackboard_->last_position.y = blackboard_->robot_pose_.pose.pose.position.y;
                blackboard_->last_position.z = blackboard_->robot_pose_.pose.pose.position.z; // 更新位置
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
                double v_x = blackboard_->vel_msg_sub_.linear.x;
                double v_y = blackboard_->vel_msg_sub_.linear.y;
                ros::Duration(0.2).sleep(); // 等待0.2s,判断速度是否相等（获取点失败后可能不更新速度，也可能速度为0）
                if (blackboard_->vel_msg_sub_.linear.x == v_x && blackboard_->vel_msg_sub_.linear.y == v_y)
                {
                    exec_stauts = Implementation::FAILED;
                    return Implementation::FAILED;
                }
                exec_stauts = Implementation::IN_PROGRESS;
                return Implementation::IN_PROGRESS;
            }
            else
            {
                exec_stauts = Implementation::IN_PROGRESS;
                return Implementation::IN_PROGRESS;
            }
        }
    }
}
bool ChassisExecutor::GetMoveStatus()
{
    double distance = sqrt(pow(blackboard_->robot_pose_.pose.pose.position.x - target_pose_.pose.position.x, 2) + pow(blackboard_->robot_pose_.pose.pose.position.y - target_pose_.pose.position.y, 2));
    ROS_INFO("distance %f", distance);
    if (distance < 0.20)
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
Implementation ChassisExecutor::Move(double pos_x, double pos_y)
{
    Implementation status;
    robotStatePub(RobotState::MOVE);
    status = SendDataToPlan(pos_x, pos_y);
    ros::Duration(0.2).sleep();
    return status;
}
Implementation ChassisExecutor::QueueMove(std::vector<sp_decision::Blackboard::Point> points, sp_decision::Blackboard::Action_Lock action, int stay_time)
{
    Implementation status;
    robotStatePub(RobotState::CRUISR);
    if (action_status != action)
    {
        num = -1;
    }
    if (num == -1)
    {
        action_status = action;
        for (int i = 0; i < 200; i++)
        {
            status = SendDataToPlan(points[0].x, points[0].y);
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
            status = SendDataToPlan(points[num].x, points[num].y);
            ros::Duration(0.2).sleep();
            if (blackboard_->plan_get_)
            {
                break;
            }
        }
    }
    return status;
}
Implementation ChassisExecutor::QueueMoveSlow(std::vector<sp_decision::Blackboard::Point> points, sp_decision::Blackboard::Action_Lock action)
{
    Implementation status;
    robotStatePub(RobotState::SlOW);
    if (action_status != action)
    {
        num = -1;
    }
    if (num == -1)
    {
        action_status = action;
        for (int i = 0; i < 200; i++)
        {
            status = SendDataToPlan(points[0].x, points[0].y);
            ros::Duration(0.2).sleep();
            num++;
            if (blackboard_->plan_get_)
                break;
        }
    }
    else if (GetMoveStatus())
    {
        for (int i = num + 1; i < 1000; i++)
        {
            num = i % points.size();
            status = SendDataToPlan(points[num].x, points[num].y);
            ros::Duration(0.2).sleep();
            if (blackboard_->plan_get_)
            {
                break;
            }
        }
    }
    return status;
}
Implementation ChassisExecutor::FastMove(double pos_x, double pos_y)
{
    Implementation status;
    robotStatePub(RobotState::FAST);
    status = SendDataToPlan(pos_x, pos_y);
    return status;
}
Implementation ChassisExecutor::Cruisr(double pos_x, double pos_y)
{
    Implementation status;
    robotStatePub(RobotState::CRUISR);
    SendDataToPlan(pos_x, pos_y);
    return status;
}
Implementation ChassisExecutor::Pursuit(double pos_x, double pos_y)
{
    Implementation status;
    robotStatePub(RobotState::PURSUIT);
    SendDataToPlan(pos_x, pos_y);
    return status;
}
Implementation ChassisExecutor::VelIdle()
{
    Implementation status;
    robotStatePub(RobotState::ROTATE);
    return status;
}
Implementation ChassisExecutor::VelStop()
{
}
Implementation ChassisExecutor::Stop()
{
    Implementation status;
    robotStatePub(RobotState::STOP);
    return status;
}
Implementation ChassisExecutor::Idle()
{
    Implementation status;
    robotStatePub(RobotState::IDLE);
    VelIdle();
    return status;
}
/**
 *@brief 控制云台角度,角度以上电时为0，逆时针为正，-180到180
 */
void ChassisExecutor::observe(double min_angle, double max_angle)
{
    gimbal_mutex.lock();
    robot_msg::CmdGimbal gimbal;
    gimbal.yaw_min = min_angle;
    gimbal.yaw_max = max_angle;
    gimbal.pitch_max = 0;
    gimbal.pitch_min = 0;
    gimbal_pub_.publish(gimbal);
    control_gimbal = 1;
    gimbal_mutex.unlock();
}