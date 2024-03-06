#include "behaviors/attack.hpp"

namespace sp_decision
{
    BehaviorState AttackBehavior::Update()
    {
        if (blackboard_ptr_->test_id == 2)
        {
            //ROS_INFO("attack");
            attack_outpost();
            log_exe_ptr_->info("behavior: attack");
            return BehaviorState::SUCCESS;
        }
        return BehaviorState::FAILURE;
    }

    void AttackBehavior::attack_outpost()
    {
        chassis_exe_ptr_->QueueMove(blackboard_ptr_->attack_pos);
    }
}
