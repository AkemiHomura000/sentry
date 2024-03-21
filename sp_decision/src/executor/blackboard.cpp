#include "executor/blackboard.hpp"

namespace sp_decision
{
    Blackboard::Blackboard() : match_state_received_(false), robot_odom_received_(false)
    {
        match_status_sub_ = nh_.subscribe("match_status", 1,
                                          &Blackboard::MatchStatusCallback, this);
        robot_odom_sub_ =
            nh_.subscribe("localization", 1, &Blackboard::RobotPoseCallback, this);
        referee_info_sub_ =
            nh_.subscribe("referee_info", 1, &Blackboard::RefereeInfoCallback, this);
        referee_data_sub_ = nh_.subscribe("referee_data", 1,
                                          &Blackboard::RefereeDataCallback, this);
        cmd_vel_sub_ = nh_.subscribe("cmd_vel", 10, &Blackboard::CmdVelDataCallback, this);
        goal_status_sub_ = nh_.subscribe("move_base/status", 10, &Blackboard::GoalStatusCallback, this);
        armor_sub_ = nh_.subscribe("/armor", 10, &Blackboard::ArmorCallback, this);
        enemy_hp_sub_ = nh_.subscribe("Enemy_robot_HP", 10, &Blackboard::EnemyCallback, this);
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
                // ROS_INFO("move_base ok");
                plan_get_ = true;
            }
        }
        goal_status_mutex.unlock();
    }
    void Blackboard::MatchStatusCallback(const robot_msg::MatchMsg::ConstPtr msg)
    {
        match_status_cbk_mutex.lock();
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
    void Blackboard::RefereeInfoCallback(const robot_msg::RefereeInfoMsg::ConstPtr &msg)
    {
        referee_info_mutex.lock();
        // 判断比赛进程
        if (game_progress < 4)
        {
            game_status_ = MatchSatuts::TO_BEGIN;
        }
        if (game_progress == 5)
        {
            game_status_ = MatchSatuts::AFTER_MATCH;
        }
        if (game_progress == 4 || (stage_remain_time < 299 && stage_remain_time > 0))
        {
            game_status_ = MatchSatuts::AT_MATCH;
        }
        if (stage_remain_time == 0)
        {
            game_status_ = MatchSatuts::AFTER_MATCH;
        }
        // 更新基地受击状态?
        if (base_HP_ > msg->base_HP || base_attacked_)
        {
            // ROS_INFO("base :%f", msg->base_HP);
            base_attacked_ = true;
            if (base_HP_ == msg->base_HP)
            {
                ros::Time time = ros::Time::now();
                if ((time.sec - current_time.sec) > 5) // 频率待确定?
                {
                    base_attacked_ = false;
                    current_time = ros::Time::now();
                }
            }
            if (base_HP_ > msg->base_HP)
            {
                current_time = ros::Time::now();
            }
        }
        base_HP_ = msg->base_HP;
        game_progress = msg->game_progress;
        stage_remain_time = msg->stage_remain_time;
        // 更新可用血量?
        if (robot_hp_ < msg->robot_HP)
        {
            available_hp_ -= (msg->robot_HP - robot_hp_);
        }
        // 更新烧饼受击状态?
        if (robot_hp_ > msg->robot_HP || status_init || attacked_violently_)
        {
            if (!status_init)
            {
                time_1 = ros::Time::now();
                current_hp = msg->robot_HP;
                status_init = 1;
            }
            if (ros::Time::now().sec - time_1.sec > 3) // 3s更新一次?
            {
                if (current_hp - msg->robot_HP > 90)
                {
                    attacked_violently_ = true;
                }
                else
                {
                    attacked_violently_ = false;
                }
                status_init = 0;
            }
        }
        robot_hp_ = msg->robot_HP;

        referee_info_mutex.unlock();
    }
    void Blackboard::ArmorCallback(const robot_msg::Armor::ConstPtr &msg)
    {
        armor_mutex.lock();
        armor_received_ = 1;
        time_received_armor_ = ros::Time::now();
        armor_ = *msg;
        armor_mutex.unlock();
    }
    void Blackboard::CmdVelDataCallback(const geometry_msgs::Twist &msg)
    {
        nav_cmd_vel_cbk_mutex.lock();
        vel_msg_sub_.linear.x = msg.linear.x;
        vel_msg_sub_.linear.y = msg.linear.y;
        vel_msg_sub_.angular.z = msg.angular.z;
        nav_cmd_vel_cbk_mutex.unlock();
    }
    void Blackboard::EnemyCallback(const robot_msg::RobotHP::ConstPtr &msg)
    {
        enemy_hp_mutex.lock();
        Sentry_HP_ = msg->Sentry_HP; // TODO:敌方烧饼血量
        enemy_hp_mutex.unlock();
    }
} // namespace sp_decision