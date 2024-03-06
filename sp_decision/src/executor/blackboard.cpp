#include "executor/blackboard.hpp"

namespace sp_decision
{
    Blackboard::Blackboard() : match_state_received_(false), robot_odom_received_(false)
    {
        match_status_sub_ = nh_.subscribe("match_status", 1,
                                          &Blackboard::MatchStatusCallback, this);
        robot_odom_sub_ =
            nh_.subscribe("localization", 1, &Blackboard::RobotPoseCallback, this);

        referee_data_sub_ = nh_.subscribe("referee_data", 1,
                                          &Blackboard::RefereeDataCallback, this);
        cmd_vel_sub_ = nh_.subscribe("cmd_vel", 10, &Blackboard::CmdVelDataCallback, this);
        goal_status_sub_ = nh_.subscribe("move_base/status", 10, &Blackboard::GoalStatusCallback, this);

        nh_.param("min_hp", min_hp_, 200);
        nh_.param("min_bullet", min_bullet_, 100);
        nh_.param("min_outpost", min_outpost_, 300);
        nh_.param("distance_tolerance", distance_tolerance_, float(0.2));
    }

    void Blackboard::ResetFlag()
    {
        robot_odom_received_ = false;
        match_state_received_ = false;
    }
    void Blackboard::GoalStatusCallback(const actionlib_msgs::GoalStatusArray::ConstPtr &msg)
    {
        goal_status_mutex.lock();
        plan_get_ = false;
        for (const auto &status : msg->status_list)
        {
            if (status.status == 1)
            {
                //ROS_INFO("move_base ok");
                plan_get_ = true;
            }
        }
        goal_status_mutex.unlock();
    }
    void Blackboard::MatchStatusCallback(const robot_msg::MatchMsg::ConstPtr msg)
    {
        match_status_cbk_mutex.lock();
        robot_hp_ = msg->robot_hp;
        robot_bullet_ = msg->robot_bullet;
        outpost_hp_ = msg->outpost_hp;
        game_type = msg->game_type;
        game_progress = msg->game_progress;
        match_remainder = msg->match_remainder;
        test_id = msg->test_id;
        match_state_received_ = true;
        match_status_cbk_mutex.unlock();
    }

    void Blackboard::RobotPoseCallback(const nav_msgs::Odometry::ConstPtr msg)
    {
        robot_odom_cbk_mutex.lock();
        robot_pose_ = *msg;
        robot_odom_received_ = true;
        robot_odom_cbk_mutex.unlock();
    }
    void Blackboard::RefereeDataCallback(const geometry_msgs::Point::ConstPtr &msg)
    {
        referee_data_cbk_mutex.lock();
        posx_x_ = msg->x;
        posy_y_ = msg->y;
        key_z_ = msg->z;
        referee_data_cbk_mutex.unlock();
    }
    void Blackboard::CmdVelDataCallback(const geometry_msgs::Twist &msg)
    {
        nav_cmd_vel_cbk_mutex.lock();
        vel_msg_sub_.linear.x = msg.linear.x;
        vel_msg_sub_.linear.y = msg.linear.y;
        vel_msg_sub_.angular.z = msg.angular.z;
        nav_cmd_vel_cbk_mutex.unlock();
    }
} // namespace sp_decision