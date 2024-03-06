#include "behaviors/retreat.hpp"

namespace sp_decision
{
    BehaviorState RetreatBehavior::Update()
    {
        /**
         * @brief: 哨兵预装弹，无弹回原点自旋
         */
        if (blackboard_ptr_->test_id == 5)
        {
            ROS_INFO("retreat");
            RandomMode();
            // log_exe_ptr_->info("behavior[retreat]:", "fast move");

            return BehaviorState::SUCCESS;
        }
        return BehaviorState::FAILURE;
    }
    void RetreatBehavior::Go2Init()
    {
        chassis_exe_ptr_->FastMove(blackboard_ptr_->buff_pos_[0].x,
                                   blackboard_ptr_->buff_pos_[0].y);
    }
    void RetreatBehavior::RandomMode()
    {
        if (std::hypot(blackboard_ptr_->robot_pose_.pose.pose.position.x -
                           blackboard_ptr_->random_mode_pos[random_mode_pos_count].x,
                       blackboard_ptr_->robot_pose_.pose.pose.position.y -
                           blackboard_ptr_->random_mode_pos[random_mode_pos_count].y) <
            blackboard_ptr_->distance_tolerance_)
        {
            random_mode_pos_count++;
            if (random_mode_pos_count == 4)
            {
                random_mode_pos_count = 0;
            }
        }
        chassis_exe_ptr_->FastMove(blackboard_ptr_->random_mode_pos[random_mode_pos_count].x,
                                   blackboard_ptr_->random_mode_pos[random_mode_pos_count].y);

    }
} // namespace sp_decision