#include "behaviors/attack.hpp"

namespace sp_decision
{
    BehaviorState AttackBehavior::Update()
    {
        if (blackboard_ptr_->action_status_ >= Blackboard::Action_Lock::ATTACK)
        {
            if (blackboard_ptr_->robot_hp_ > 400 && blackboard_ptr_->match_remainder > 180)
            {
                ROS_INFO("attack");
                attack_point_1();
                log_exe_ptr_->info("behavior: attack");
                blackboard_ptr_->action_status_ = Blackboard::Action_Lock::ATTACK;
                return BehaviorState::SUCCESS;
            }
            blackboard_ptr_->action_status_ = Blackboard::Action_Lock::JUDGING;
        }
        return BehaviorState::FAILURE;
    }

    void AttackBehavior::attack_point_1()
    {
        chassis_exe_ptr_->Move(blackboard_ptr_->attack_pos[0].x, blackboard_ptr_->attack_pos[0].y);
        if (chassis_exe_ptr_->GetMoveStatus())
        {
            chassis_exe_ptr_->VelIdle();
        }
    }
}
