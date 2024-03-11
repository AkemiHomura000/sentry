#include "behaviors/add_blood.hpp"

namespace sp_decision
{
    BehaviorState AddBloodBehavior::Update()
    {
        ROS_INFO("hp: %d  ,time :%d ,available :%d",blackboard_ptr_->robot_hp_,blackboard_ptr_->stage_remain_time,blackboard_ptr_->available_hp_ );
        if (blackboard_ptr_->action_status_ >=  Blackboard::Action_Lock::ADD_BLOOD)
        {
            if (blackboard_ptr_->action_status_ == Blackboard::Action_Lock::ADD_BLOOD || (blackboard_ptr_->robot_hp_ < 120 && blackboard_ptr_->stage_remain_time > 60) || blackboard_ptr_->test_id == 1)
            {
                blackboard_ptr_->action_status_ = Blackboard::Action_Lock::ADD_BLOOD;
                if (blackboard_ptr_->robot_hp_ < 600 && blackboard_ptr_->available_hp_ > 0&& blackboard_ptr_->stage_remain_time > 60)
                {
                    ROS_INFO("add_blood");
                    Go2Buff();
                    log_exe_ptr_->info("behavior: add blood");
                    return BehaviorState::SUCCESS;
                }
                else if( blackboard_ptr_->available_hp_ <= 0||blackboard_ptr_->robot_hp_ >= 600|| blackboard_ptr_->stage_remain_time <=60)
                {
                    blackboard_ptr_->action_status_ = Blackboard::Action_Lock::JUDGING ;
                    return BehaviorState::FAILURE;
                }
            }
            return BehaviorState::FAILURE;
        }
    }

    void AddBloodBehavior::Go2Buff()
    {
        chassis_exe_ptr_->Move(blackboard_ptr_->buff_pos_[1].x,
                                 blackboard_ptr_->buff_pos_[1].y);
    }
}