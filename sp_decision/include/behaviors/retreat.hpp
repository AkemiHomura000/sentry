#ifndef RETREAT_H
#define RETREAT_H

#include "node/behavior_tree.h"
namespace sp_decision
{
  class RetreatBehavior : public ActionNode
  {
  public:
    RetreatBehavior(std::string name, int level,
                    const Blackboard::Ptr &blackboard_ptr,
                    ChassisExecutor::Ptr &chassis_exe_ptr,
                    LogExecutor::Ptr &log_exe_ptr)
        : ActionNode::ActionNode(name, level, blackboard_ptr, chassis_exe_ptr,
                                 log_exe_ptr) {}
    BehaviorState Update();

  private:
    int random_mode_pos_count = 0;

    void Go2Init();
    void RandomMode();
    // enum class Retreat_Status{
    //         SUCCESS,
    //         FAIL
    //     };
    // Retreat_Status status=Retreat_Status::FAIL;
  };
} // namespace sp_decision

#endif