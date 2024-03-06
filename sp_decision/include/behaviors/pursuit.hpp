#ifndef PURSUIT_HPP
#define PURSUIT_HPP

#include "node/behavior_tree.h"

namespace sp_decision
{
    class PursuitBehavior : public ActionNode
    {
    public:
        PursuitBehavior(std::string name, int level,
                        const Blackboard::Ptr &blackboard_ptr,
                        ChassisExecutor::Ptr &chassis_exe_ptr,
                        LogExecutor::Ptr &log_exe_ptr)
            : ActionNode::ActionNode(name, level, blackboard_ptr, chassis_exe_ptr, log_exe_ptr) {}
        BehaviorState Update();

    private:
        void pursuit();
    };
} // namespace robot_decision

#endif