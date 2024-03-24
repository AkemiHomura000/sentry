#ifndef ADD_BLOOG_H
#define ADD_BLOOD_H

#include "node/behavior_tree.h"
#include <unistd.h>
unsigned int sleep(unsigned int seconds);

namespace sp_decision
{
  class AddBloodBehavior : public ActionNode
  {
  public:
    AddBloodBehavior(std::string name, int level,
                     const Blackboard::Ptr &blackboard_ptr,
                     ChassisExecutor::Ptr &chassis_exe_ptr,
                     LogExecutor::Ptr &log_exe_ptr)
        : ActionNode::ActionNode(name, level, blackboard_ptr, chassis_exe_ptr, log_exe_ptr) {}
    BehaviorState Update();

  private:
    void Go2Buff();
    int status = 0; // 0表示初始化，1表示前往一号点，2表示前往二号点，3表示到达一号点，4表示到达二号点
    int count = 0;  // 加血次数限制，只加一次血,0为未开始加血，1为8秒内，2为完成
    ros::Time last_time;
  };
} // namespace robot_decision

#endif