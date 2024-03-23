#ifndef ATTACK_H
#define ATTACK_H

#include "node/behavior_tree.h"
#include "ros/ros.h"
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
    void attack_point_3(); // 冲家
    void attack_point_4(); // 冲家
    void attack_point_5(); // 冲家
    int num_3 = 0;         // 冲家阶段，1表示前往1点，2表示到达1点，3表示前往2点，4表示到达2点，5表示前往3，6表示到达3
    ros::Time last_time;   // 计时观察时间
  };
} // namespace robot_decision

#endif