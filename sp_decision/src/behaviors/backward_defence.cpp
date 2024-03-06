#include "behaviors/backward_defence.hpp"

namespace sp_decision
{
    BehaviorState DenfenceBehavior::Update()
    {
        if (blackboard_ptr_->test_id == 3)
        {
            ROS_INFO("backward_defnece");
            backward_defnece();
            log_exe_ptr_->info("behavior: defence");
            return BehaviorState::SUCCESS;
        }
        return BehaviorState::FAILURE;
    }
    // 接入自瞄执行
    void DenfenceBehavior::backward_defnece()
    {
    }
}
