#ifndef ATTACK_H
#define ATTACK_H

#include "node/behavior_tree.h"

namespace sp_decision
{
  class AttackBehavior : public ActionNode
  {
  public:
    AttackBehavior(std::string name, int level,
                   const Blackboard::Ptr &blackboard_ptr,
                   ChassisExecutor::Ptr &chassis_exe_ptr,
                   LogExecutor::Ptr &log_exe_ptr)
        : ActionNode::ActionNode(name, level, blackboard_ptr, chassis_exe_ptr, log_exe_ptr) {}
    BehaviorState Update();

  private:
    int num_1 = 0;
    int num_2 = 0;
    void attack_point_1(); // 保护我方补给点
    void attack_point_2(); // 二号点位，看中心增益点
  };
} // namespace robot_decision

#endif