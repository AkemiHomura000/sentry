#include "behaviors/add_blood.hpp"

namespace sp_decision
{
    BehaviorState AddBloodBehavior::Update()
    {
        std::stringstream str;
        str << "status: "
            << "hp: " << blackboard_ptr_->robot_hp_ << "\t remainder:" << blackboard_ptr_->stage_remain_time
            << "\t available_hp:" << blackboard_ptr_->available_hp_ << "\tbase_attacked:" << blackboard_ptr_->base_attacked_
            << std::endl;
        blackboard_ptr_->LogPub(str.str());
        if (chassis_exe_ptr_->control_gimbal == 0) // 上一次循环未调用则恢复扫描模式
        {
            chassis_exe_ptr_->observe(0, 0);
        }
        chassis_exe_ptr_->control_gimbal = 0;
        if (blackboard_ptr_->stage_remain_time == 0)
        {
            //ROS_INFO("stay (time==0)");
            chassis_exe_ptr_->Stop();
            return BehaviorState::SUCCESS;
        }
        ROS_INFO("hp: %d  ,time :%d ,available :%d", blackboard_ptr_->robot_hp_, blackboard_ptr_->stage_remain_time, blackboard_ptr_->available_hp_);
        if (blackboard_ptr_->action_status_ >= Blackboard::Action_Lock::ADD_BLOOD)
        {
            if (blackboard_ptr_->action_status_ == Blackboard::Action_Lock::ADD_BLOOD ||
                (blackboard_ptr_->robot_hp_ >0 &&blackboard_ptr_->robot_hp_ < 300 && blackboard_ptr_->stage_remain_time > 60) ||
                (blackboard_ptr_->robot_hp_ >0 &&blackboard_ptr_->robot_hp_ < 600 && blackboard_ptr_->stage_remain_time < 80) || blackboard_ptr_->test_id == 1) // 补血条件：1.低于300血且时间>60且补血次数不超过2次;2.低于600血，且时间<90且补血次数不超过2次
            {
                if (blackboard_ptr_->action_status_ != Blackboard::Action_Lock::ADD_BLOOD) // 从其他状态进入会初始化
                {
                    status = 0;
                    count = 0;
                }
                blackboard_ptr_->action_status_ = Blackboard::Action_Lock::ADD_BLOOD;
                if (count == 1 && (ros::Time::now().sec - last_time.sec) > 5) // 在补血区停留5秒
                {
                    number++;
                    count = 2;
                }
                if (blackboard_ptr_->robot_hp_ < 600 && blackboard_ptr_->stage_remain_time > 60 && count != 2 && number < 2)
                {
                    ROS_INFO("add_blood");
                    Go2Buff();
                    if ((count == 0 && blackboard_ptr_->available_hp_ < 600 && number == 0) || (count == 0 && blackboard_ptr_->available_hp_ < last_available_hp_ && number == 1))
                    {
                        last_available_hp_ = blackboard_ptr_->available_hp_;
                        last_time = ros::Time::now();
                        count++;
                    }
                    std::stringstream str;
                    str << "behavior: add blood";
                    blackboard_ptr_->LogPub(str.str());
                    return BehaviorState::SUCCESS;
                }
                else if (number == 2 || count == 2 || blackboard_ptr_->robot_hp_ >= 600 || blackboard_ptr_->stage_remain_time <= 60)
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
            std::stringstream str;
            str << "\nbehavior: add blood status-------------" << status;
            blackboard_ptr_->LogPub(str.str());
            ROS_INFO("status-------------%d", status);
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
            std::stringstream str;
            str << "\nbehavior: add blood status-------------" << status;
            blackboard_ptr_->LogPub(str.str());
            ROS_INFO("status-------------%d", status);
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
            std::stringstream str;
            str << "\nbehavior: add blood status-------------" << status;
            blackboard_ptr_->LogPub(str.str());
            ROS_INFO("status-------------%d", status);
            break;
        }
        case 3:
        {
            chassis_exe_ptr_->observe(-90, 0);
            chassis_exe_ptr_->Stop();
            std::stringstream str;
            str << "\nbehavior: add blood status-------------" << status;
            blackboard_ptr_->LogPub(str.str());
            break;
        }
        case 4:
        {
            chassis_exe_ptr_->observe(-80, 80);
            if (chassis_exe_ptr_->Move(blackboard_ptr_->buff_pos_[0].x,
                                       blackboard_ptr_->buff_pos_[0].y) == 1)
            {
                status = 3;
            }
            else if (chassis_exe_ptr_->Move(blackboard_ptr_->buff_pos_[0].x,
                                            blackboard_ptr_->buff_pos_[0].y) == 2)
            {
                chassis_exe_ptr_->Stop();
                status = 1;
            }
            std::stringstream str;
            str << "\nbehavior: add blood status-------------" << status;
            blackboard_ptr_->LogPub(str.str());
            ROS_INFO("status-------------%d", status);
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