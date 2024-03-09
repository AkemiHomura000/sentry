#include "behaviors/patrol.hpp"

namespace sp_decision
{
    BehaviorState PatrolBehavior::Update()
    {
        ROS_INFO("patrol_judge");
        ROS_INFO("-------------------- %d", blackboard_ptr_->action_status_);
        if (blackboard_ptr_->action_status_ >= Blackboard::Action_Lock::PATROL)
        {
            ROS_INFO("patrol");
            patrol();
            log_exe_ptr_->info("behavior: patrol");
            blackboard_ptr_->action_status_ = Blackboard::Action_Lock::PATROL;
            return BehaviorState::SUCCESS;
        }
        blackboard_ptr_->action_status_ = Blackboard::Action_Lock::JUDGING;
        return BehaviorState::FAILURE;
    }

    void PatrolBehavior::patrol()
    {
        chassis_exe_ptr_->FastMove(blackboard_ptr_->patrol_pos[0].x, blackboard_ptr_->patrol_pos[0].y);
    }
}
