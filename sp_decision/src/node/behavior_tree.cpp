#include "node/behavior_tree.h"

namespace sp_decision
{
    BehaviorState TreeNode::Run()
    {
        behavior_state_ = Update();
        return behavior_state_;
    }
    void ActionNode::print()
    {
        std::stringstream str;
        for (int i = 0; i < level_; i++)
        {
            str << "  ";
        }
        str << "- " << name_.data();
        log_exe_ptr_->info(str);
    }
    void ActionNode::print_tree() { print(); }
    void ActionNode::set_priority(BehaviorPriority priority)
    {
        behavior_priority = priority;
    }
    void SequenceNode::addChild(TreeNode *child_node_ptr, BehaviorPriority pri)
    {
        child_node_ptr->behavior_priority = pri;
        child_node_ptr_list_.push_back(child_node_ptr);
        for (int i = child_node_ptr_list_.size() - 1; i > 0; i--)
        {
            if (child_node_ptr_list_[i - 1]->behavior_priority < pri)
            {
                TreeNode *temp = child_node_ptr_list_[i - 1];
                child_node_ptr_list_[i - 1] = child_node_ptr_list_[i];
                child_node_ptr_list_[i] = temp;
            }
        }
    }
    BehaviorState SequenceNode::Update()
    {
        if (child_node_ptr_list_.size() == 0)
        {
            return BehaviorState::FAILURE;
        }
        else
        {
            for (int i = 0; i < child_node_ptr_list_.size(); i++)
            {
                if (child_node_ptr_list_[i]->Run() == BehaviorState::SUCCESS)
                {
                    return BehaviorState::SUCCESS;
                }
            }
            return BehaviorState::FAILURE;
        }
    }
    int SequenceNode::num(){
        return child_node_ptr_list_.size();
    }
    void SequenceNode::print()
    {
        std::stringstream str;
        for (int i = 0; i < level_; i++)
        {
            str << "  ";
        }
        str << "+ " << name_.data();
        log_exe_ptr_->info(str);
    }
    void SequenceNode::print_tree()
    {
        print();
        for (int i = 0; i < child_node_ptr_list_.size(); i++)
        {
            child_node_ptr_list_[i]->print_tree();
        }
    }
} // namespace sp_decision