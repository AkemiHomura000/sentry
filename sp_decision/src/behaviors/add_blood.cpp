#include "behaviors/add_blood.hpp"

namespace sp_decision
{
    BehaviorState AddBloodBehavior::Update()
    {
        ROS_INFO("hp: %d  ,time :%d ,available :%d", blackboard_ptr_->robot_hp_, blackboard_ptr_->stage_remain_time, blackboard_ptr_->available_hp_);
        if (blackboard_ptr_->action_status_ >= Blackboard::Action_Lock::ADD_BLOOD)
        {
            if (blackboard_ptr_->action_status_ == Blackboard::Action_Lock::ADD_BLOOD || (blackboard_ptr_->robot_hp_ < 120 && blackboard_ptr_->stage_remain_time > 60) || blackboard_ptr_->test_id == 1)
            {
                if (blackboard_ptr_->action_status_ != Blackboard::Action_Lock::ADD_BLOOD) // 从其他状态进入会初始化
                {
                    status = 0;
                }
                blackboard_ptr_->action_status_ = Blackboard::Action_Lock::ADD_BLOOD;
                if (blackboard_ptr_->robot_hp_ < 600 && blackboard_ptr_->available_hp_ > 0 && blackboard_ptr_->stage_remain_time > 60)
                {
                    ROS_INFO("add_blood");
                    Go2Buff();
                   
                    log_exe_ptr_->info("behavior: add blood");
                    return BehaviorState::SUCCESS;
                }
                else if (blackboard_ptr_->available_hp_ <= 0 || blackboard_ptr_->robot_hp_ >= 600 || blackboard_ptr_->stage_remain_time <= 60)
                {
                    blackboard_ptr_->action_status_ = Blackboard::Action_Lock::JUDGING;
                    return BehaviorState::FAILURE;
                }
            }
            return BehaviorState::FAILURE;
        }
    }

    void AddBloodBehavior::Go2Buff()
    {
        switch (status)
        {
        case 0:
        {
            if (chassis_exe_ptr_->Move(blackboard_ptr_->buff_pos_[0].x,
                                       blackboard_ptr_->buff_pos_[0].y) != 2)
            {
                chassis_exe_ptr_->VelIdle();
                ros::Duration(1.5).sleep();
                status = 1;
            }
            else
            {
                chassis_exe_ptr_->VelIdle();
                ros::Duration(1.5).sleep();
                status = 2;
            }
             ROS_INFO("status-------------%d",status);
            break;
        }
        case 1:
        {
            if (chassis_exe_ptr_->Move(blackboard_ptr_->buff_pos_[0].x,
                                       blackboard_ptr_->buff_pos_[0].y) == 0)
            {
                status = 1;
            }
            else if (chassis_exe_ptr_->Move(blackboard_ptr_->buff_pos_[0].x,
                                            blackboard_ptr_->buff_pos_[0].y) == 1)
            {
                status = 3;
            }
            else if (chassis_exe_ptr_->Move(blackboard_ptr_->buff_pos_[0].x,
                                           blackboard_ptr_->buff_pos_[0].y) == 2)
            {
                status = 2;
            }
             ROS_INFO("status-------------%d",status);
            break;
        }
        case 2:
        {
            if (chassis_exe_ptr_->Move(blackboard_ptr_->buff_pos_[1].x,
                                       blackboard_ptr_->buff_pos_[1].y) == 0)
            {
                status = 2;
            }
            else if (chassis_exe_ptr_->Move(blackboard_ptr_->buff_pos_[1].x,
                                            blackboard_ptr_->buff_pos_[1].y) == 1)
            {
                status = 4;
            }
            else if (chassis_exe_ptr_->Move(blackboard_ptr_->buff_pos_[1].x,
                                           blackboard_ptr_->buff_pos_[1].y) == 2)
            {
                status = 2; // 无法到达重复发送
            }
             ROS_INFO("status-------------%d",status);
            break;
        }
        case 3:
        {
            chassis_exe_ptr_->Stop();
            break;
        }
        case 4:
        {
            if (chassis_exe_ptr_->Move(blackboard_ptr_->buff_pos_[0].x,
                                       blackboard_ptr_->buff_pos_[0].y) == 1)
            {
                status = 3;
            }
            else  if (chassis_exe_ptr_->Move(blackboard_ptr_->buff_pos_[0].x,
                                       blackboard_ptr_->buff_pos_[0].y) == 2){
                chassis_exe_ptr_->Stop();
                status=1;
            }
             ROS_INFO("status-------------%d",status);
            break;
        }
        default:
        {
            std::cout << "Unknown status" << std::endl;
            break;
        }
        }

        // if (chassis_exe_ptr_->Move(blackboard_ptr_->buff_pos_[0].x,
        //                            blackboard_ptr_->buff_pos_[0].y) == 2)
        // {
        //     status = 2;
        //     if (chassis_exe_ptr_->Move(blackboard_ptr_->buff_pos_[1].x,
        //                                blackboard_ptr_->buff_pos_[1].y) == 2)
        // }
    }
}