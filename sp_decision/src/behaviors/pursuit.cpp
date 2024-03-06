#include "behaviors/pursuit.hpp"

namespace sp_decision
{
    BehaviorState PursuitBehavior::Update()
    {
        if (blackboard_ptr_->test_id == 4)
        {
            ROS_INFO("pursuit");
            pursuit();
            log_exe_ptr_->info("behavior: pursuit");
            return BehaviorState::SUCCESS;
        }
        return BehaviorState::FAILURE;
    }
    // 接入自瞄执行
    void PursuitBehavior::pursuit()
    {
    }
}
