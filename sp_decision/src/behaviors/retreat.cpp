#include "behaviors/retreat.hpp"

namespace sp_decision
{
    BehaviorState RetreatBehavior::Update()
    {
        /**
         * @brief: 哨兵掉血过快撤退
         */
        if (blackboard_ptr_->action_status_ >= Blackboard::Action_Lock::RETREAT)
        {
            if (blackboard_ptr_->attacked_violently_)
            {
                ROS_INFO("retreat");
                 RandomMode();
                //  log_exe_ptr_->info("behavior[retreat]:", "fast move");
                blackboard_ptr_->action_status_ = Blackboard::Action_Lock::RETREAT;
                return BehaviorState::SUCCESS;
            }
            if (blackboard_ptr_->action_status_ == Blackboard::Action_Lock::RETREAT)
            {
                if (!blackboard_ptr_->attacked_violently_)
                {
                    blackboard_ptr_->action_status_ = Blackboard::Action_Lock::JUDGING;
                    return BehaviorState::FAILURE;
                }
            }
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
        chassis_exe_ptr_->QueueMove(blackboard_ptr_->random_mode_pos,blackboard_ptr_->action_status_, 10);
    }
} // namespace sp_decision