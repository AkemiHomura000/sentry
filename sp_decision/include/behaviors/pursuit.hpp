#ifndef PURSUIT_HPP
#define PURSUIT_HPP

#include "node/behavior_tree.h"
#include "math.h"
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
        void armor_status_update();
        bool armor_tracked_ = 0;
        double xyz_map[3];
         double xyz_target_map[3];
        double distance = 0;
    };
} // namespace robot_decision

#endif