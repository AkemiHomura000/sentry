#include "behaviors/backward_defence.hpp"

namespace sp_decision
{
    BehaviorState DenfenceBehavior::Update()
    {
        if (blackboard_ptr_->action_status_ >= Blackboard::Action_Lock::BACKWARD_DEFENCE)
        {
             if (blackboard_ptr_->action_status_!= Blackboard::Action_Lock::BACKWARD_DEFENCE){
                status = Backward_Defence_Status::Init;
             }
            if (blackboard_ptr_->base_attacked_)
            {
                blackboard_ptr_->action_status_ = Blackboard::Action_Lock::BACKWARD_DEFENCE;
                ROS_INFO("backward_defnece");
                backward_defnece();
                log_exe_ptr_->info("behavior: defence");
                return BehaviorState::SUCCESS;
            }
            if (blackboard_ptr_->action_status_ == Blackboard::Action_Lock::BACKWARD_DEFENCE)
            {
                if (!blackboard_ptr_->base_attacked_)
                {
                    blackboard_ptr_->action_status_ = Blackboard::Action_Lock::JUDGING;
                    status=Backward_Defence_Status::Init;
                    return BehaviorState::FAILURE;
                }
            }
        }
        return BehaviorState::FAILURE;
    }
    // TODO:接入自瞄执行
    void DenfenceBehavior::backward_defnece() // 区分不同动作执行状态
    {
        if (status == Backward_Defence_Status::Init)
        {
            if (chassis_exe_ptr_->FastMove(blackboard_ptr_->backward_defence_pos_[0].x, blackboard_ptr_->backward_defence_pos_[0].y))
            {
                status = Backward_Defence_Status::First;
            }
            else if (chassis_exe_ptr_->FastMove(blackboard_ptr_->backward_defence_pos_[1].x, blackboard_ptr_->backward_defence_pos_[1].y))
            {
                status = Backward_Defence_Status::Second;
            }
        }
        if (status == Backward_Defence_Status::First)
        {
            if (chassis_exe_ptr_->GetMoveStatus())
            {
                chassis_exe_ptr_->VelIdle();
                ros::Duration(3).sleep(); // 等待时间待调整
                chassis_exe_ptr_->QueueMove(blackboard_ptr_->backward_defence_queue_pos_,blackboard_ptr_->action_status_,3);
                status = Backward_Defence_Status::Third;
            }
        }
        if (status == Backward_Defence_Status::Second)
        {
            chassis_exe_ptr_->VelIdle();
            if (chassis_exe_ptr_->FastMove(blackboard_ptr_->backward_defence_pos_[0].x, blackboard_ptr_->backward_defence_pos_[0].y))
            {
                status = Backward_Defence_Status::First;
            }
        }
        if (status == Backward_Defence_Status::Third)
        {
            chassis_exe_ptr_->QueueMove(blackboard_ptr_->backward_defence_queue_pos_,blackboard_ptr_->action_status_, 3);
        }
    }
}
