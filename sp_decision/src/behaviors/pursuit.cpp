#include "behaviors/pursuit.hpp"

namespace sp_decision
{
    BehaviorState PursuitBehavior::Update()
    {
        armor_status_update();
        if (blackboard_ptr_->action_status_ >= Blackboard::Action_Lock::PURSUIT)
        {
            if (armor_tracked_ && distance < 9)
            {
                ROS_INFO("pursuit");
                pursuit();
                log_exe_ptr_->info("behavior: pursuit");
                return BehaviorState::SUCCESS;
            }
            else
            {
                return BehaviorState::FAILURE;
            }
        }
        return BehaviorState::FAILURE;
    }
    void PursuitBehavior::pursuit()
    {
        chassis_exe_ptr_->Pursuit(xyz_map[0], xyz_map[1]);
    }
    // 更新装甲板信息
    void PursuitBehavior::armor_status_update()
    {
        if (blackboard_ptr_->armor_received_)
        {
            if ((ros::Time::now().sec - blackboard_ptr_->time_received_armor_.sec) < 2) // 两秒内接收到的消息算作有效
            {
                if (blackboard_ptr_->armor_.track_status == 1)
                {
                    armor_tracked_ = true;
                    if (sqrt(pow(xyz_map[0] - blackboard_ptr_->robot_pose_.pose.pose.position.x, 2) +
                             pow(xyz_map[1] - blackboard_ptr_->robot_pose_.pose.pose.position.y, 2)) > 0.1)
                    {
                        xyz_map[0] = blackboard_ptr_->armor_.pose.position.x;
                        xyz_map[1] = blackboard_ptr_->armor_.pose.position.y;
                        xyz_map[2] = blackboard_ptr_->armor_.pose.position.z;
                        distance = sqrt(pow(xyz_map[0] - blackboard_ptr_->robot_pose_.pose.pose.position.x, 2) +
                                        pow(xyz_map[1] - blackboard_ptr_->robot_pose_.pose.pose.position.y, 2));
                    }
                }
                else
                {
                    armor_tracked_ = 0;
                }
            }
            else
            {
                armor_tracked_ = 0;
            }
        }
        else
        {
            armor_tracked_ = 0;
        }
    }
}
