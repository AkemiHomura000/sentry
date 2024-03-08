#ifndef PATROL_H
#define PATROL_H

#include "node/behavior_tree.h"

namespace sp_decision
{
  class PatrolBehavior : public ActionNode
  {
  public:
    PatrolBehavior(std::string name, int level,
                   const Blackboard::Ptr &blackboard_ptr,
                   ChassisExecutor::Ptr &chassis_exe_ptr,
                   LogExecutor::Ptr &log_exe_ptr)
        : ActionNode::ActionNode(name, level, blackboard_ptr, chassis_exe_ptr, log_exe_ptr) {}
    BehaviorState Update();

  private:
    void patrol();
  };
} // namespace robot_decision

#endif