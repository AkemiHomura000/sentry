#include "behaviors/pursuit.hpp"

namespace sp_decision
{
    BehaviorState PursuitBehavior::Update()
    {
        armor_status_update();
        if (blackboard_ptr_->action_status_ >= Blackboard::Action_Lock::PURSUIT)
        {

            if (armor_tracked_ && distance < 6 && blackboard_ptr_->Sentry_HP_ < 100 && blackboard_ptr_->Sentry_HP_ > 0 || blackboard_ptr_->test_id == 6)
            {
                ROS_INFO("pursuit");
                pursuit();
                log_exe_ptr_->info("behavior: pursuit");
                blackboard_ptr_->action_status_ = Blackboard::Action_Lock::PURSUIT;
                return BehaviorState::SUCCESS;
            }
            if (blackboard_ptr_->action_status_ == Blackboard::Action_Lock::PURSUIT)
            {

                blackboard_ptr_->action_status_ = Blackboard::Action_Lock::JUDGING;
                return BehaviorState::FAILURE;
            }
        }
        // blackboard_ptr_->action_status_ = Blackboard::Action_Lock::JUDGING;
        return BehaviorState::FAILURE;
    }
    void PursuitBehavior::pursuit()
    {
        if (chassis_exe_ptr_->FastMove(xyz_target_map[0], xyz_target_map[1]) == 2)
        {
            chassis_exe_ptr_->Stop();
        }
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
                    if (sqrt(pow(xyz_map[0] - blackboard_ptr_->armor_.pose.position.x, 2) +
                             pow(xyz_map[1] - blackboard_ptr_->armor_.pose.position.y, 2)) > 0.05)
                    {
                        xyz_map[0] = blackboard_ptr_->armor_.pose.position.x;
                        xyz_map[1] = blackboard_ptr_->armor_.pose.position.y;
                        xyz_map[2] = blackboard_ptr_->armor_.pose.position.z;
                        distance = sqrt(pow(xyz_map[0] - blackboard_ptr_->robot_pose_.pose.pose.position.x, 2) +
                                        pow(xyz_map[1] - blackboard_ptr_->robot_pose_.pose.pose.position.y, 2));
                        double k = 2 / distance;
                        if (sqrt(pow(xyz_target_map[0] - (xyz_map[0] + k * (blackboard_ptr_->robot_pose_.pose.pose.position.x - xyz_map[0])), 2) +
                                 pow(xyz_target_map[1] - (xyz_map[1] + k * (blackboard_ptr_->robot_pose_.pose.pose.position.y - xyz_map[1])), 2)) > 0.5) // 新旧目标点相差0.5米才更新，防止目标点更新太快导致路径失效
                        {
                            xyz_target_map[1] = xyz_map[1] + k * (blackboard_ptr_->robot_pose_.pose.pose.position.y - xyz_map[1]);
                            xyz_target_map[0] = xyz_map[0] + k * (blackboard_ptr_->robot_pose_.pose.pose.position.y - xyz_map[0]);
                        }
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
