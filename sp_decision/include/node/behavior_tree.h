#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "executor/blackboard.hpp"
#include "executor/chassis_executor.hpp"
#include "executor/log_executor.hpp"

namespace sp_decision
{
  enum class BehaviorState
  {
    SUCCESS,
    FAILURE,
    RUNNING
  };
  enum class BehaviorPriority
  {
    HIGH,
    MID,
    LOW
  };
  class TreeNode
  {
  public:
    TreeNode() {}
    TreeNode(std::string name, int level, const Blackboard::Ptr &blackboard_ptr,
             ChassisExecutor::Ptr &chassis_exe_ptr, LogExecutor::Ptr &log_exe_ptr)
        : name_(name),
          level_(level),
          blackboard_ptr_(blackboard_ptr),
          chassis_exe_ptr_(chassis_exe_ptr),
          log_exe_ptr_(log_exe_ptr) {}
    virtual ~TreeNode() = default;
    virtual BehaviorState Update() = 0;
    virtual void print() = 0;
    virtual void print_tree() = 0;
    BehaviorState Run();
    Blackboard::Ptr blackboard_ptr_;
    ChassisExecutor::Ptr chassis_exe_ptr_;
    LogExecutor::Ptr log_exe_ptr_;
    int level_;
    std::string name_;
    BehaviorState behavior_state_;
    BehaviorPriority behavior_priority;
  };
  class ActionNode : public TreeNode
  {
  public:
    ActionNode() : TreeNode::TreeNode() {}
    ActionNode(std::string name, int level, const Blackboard::Ptr &blackboard_ptr,
               ChassisExecutor::Ptr &chassis_exe_ptr,
               LogExecutor::Ptr &log_exe_ptr)
        : TreeNode::TreeNode(name, level, blackboard_ptr, chassis_exe_ptr,
                             log_exe_ptr) {}
    virtual ~ActionNode() = default;
    void print();
    void print_tree();
    void set_priority(BehaviorPriority priority);
  };
  class SequenceNode : public TreeNode
  {
  public:
    SequenceNode() : TreeNode::TreeNode() {}
    SequenceNode(std::string name, int level,
                 const Blackboard::Ptr &blackboard_ptr,
                 ChassisExecutor::Ptr &chassis_exe_ptr,
                 LogExecutor::Ptr &log_exe_ptr)
        : TreeNode::TreeNode(name, level, blackboard_ptr, chassis_exe_ptr,
                             log_exe_ptr) {}
    virtual ~SequenceNode() = default;
    void addChild(TreeNode *child_node_ptr, BehaviorPriority pri);
    virtual BehaviorState Update();
    void print();
    void print_tree();

  private:
    std::vector<TreeNode *> child_node_ptr_list_;
  };

} // namespace sp_decision

#endif