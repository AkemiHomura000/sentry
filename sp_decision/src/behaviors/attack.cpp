#include "behaviors/attack.hpp"

namespace sp_decision
{
    BehaviorState AttackBehavior::Update()
    {
        if (blackboard_ptr_->action_status_ >= Blackboard::Action_Lock::ATTACK)
        {
            if (blackboard_ptr_->action_status_ != Blackboard::Action_Lock::ATTACK)
            {
                num = 0;//重置标识符
            }
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
        if (num == 0)
        {
            chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[1].x, blackboard_ptr_->attack_pos[1].y);
            num++;
        }
        if (chassis_exe_ptr_->GetMoveStatus())
        {
            ROS_INFO("zaaaaa");
            chassis_exe_ptr_->VelIdle();
        }
    }
}
