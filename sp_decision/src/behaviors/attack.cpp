#include "behaviors/attack.hpp"

namespace sp_decision
{
    BehaviorState AttackBehavior::Update()
    {
        if (blackboard_ptr_->action_status_ >= Blackboard::Action_Lock::ATTACK)
        {
            if (blackboard_ptr_->action_status_ != Blackboard::Action_Lock::ATTACK)
            {
                ROS_INFO("status-----------%d", blackboard_ptr_->action_status_);
                num_1 = 0; // 重置标识符
                num_2 = 0;
                num_3 = 0;
            }
            if (blackboard_ptr_->base_attacked_ == 1) // 基地受到攻击到启动区
            {
                std::stringstream str;
                str << "behavior: backward_defence";
                blackboard_ptr_->LogPub(str.str());
                ROS_INFO("backward_defence");
                attack_point_3();
                blackboard_ptr_->action_status_ = Blackboard::Action_Lock::ATTACK;
                return BehaviorState::SUCCESS;
            }
            else
            {
                std::stringstream str;
                str << "behavior: attack_1";
                blackboard_ptr_->LogPub(str.str());
                ROS_INFO("attack_1");
                attack_point_1();
                blackboard_ptr_->action_status_ = Blackboard::Action_Lock::ATTACK;
                return BehaviorState::SUCCESS;
            }
            // else if (blackboard_ptr_->stage_remain_time < 290 && blackboard_ptr_->enemy_number[0] == 1) // 假设对方有英雄则前往二号点
            // {
            //     std::stringstream str;
            //     str << "behavior: attack_2";
            //     blackboard_ptr_->LogPub(str.str());
            //     ROS_INFO("attack_2");
            //     attack_point_2();
            //     blackboard_ptr_->action_status_ = Blackboard::Action_Lock::ATTACK;
            //     return BehaviorState::SUCCESS;
            // }
        }
        return BehaviorState::FAILURE;
    }

    void AttackBehavior::attack_point_1() // TODO：补充云台方向控制——————————守补给点
    {
        if (chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[0].x, blackboard_ptr_->attack_pos[0].y) == 1)
        {
            chassis_exe_ptr_->observe(-10, 10);
        }
    }
    void AttackBehavior::attack_point_2() // TODO：补充云台方向控制———————————占据启动区
    {
        if (chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[1].x, blackboard_ptr_->attack_pos[1].y) == 2)
        {
            chassis_exe_ptr_->Stop();
        }
        else if (chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[1].x, blackboard_ptr_->attack_pos[1].y) == 1)
        {
            chassis_exe_ptr_->Stop();
            chassis_exe_ptr_->observe(-180, 180);
        }
    }
    void AttackBehavior::attack_point_3() // TODO：补充云台方向控制———————————冲对面家,点1
    {
        ROS_INFO("backward_defence");
        if (chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[1].x, blackboard_ptr_->attack_pos[1].y) == 2)
        {
            chassis_exe_ptr_->Stop();
        }
        else if (chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[1].x, blackboard_ptr_->attack_pos[1].y) == 1)
        {
            chassis_exe_ptr_->Stop();
            chassis_exe_ptr_->observe(-180, 180);
        }
    }
    void AttackBehavior::attack_point_4() // TODO：补充云台方向控制———————————冲对面家,点2
    {
        ROS_INFO("attack_4");
        if (chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[3].x, blackboard_ptr_->attack_pos[3].y) == 2)
        {
            chassis_exe_ptr_->Stop();
        }
        else if (chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[3].x, blackboard_ptr_->attack_pos[3].y) == 1)
        {
            chassis_exe_ptr_->Stop();
            num_3 = 4; // 到达2点
            last_time = ros::Time::now();
        }
    }
    void AttackBehavior::attack_point_5() // TODO：补充云台方向控制———————————冲对面家,点3
    {
        ROS_INFO("attack_5");
        if (chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[4].x, blackboard_ptr_->attack_pos[4].y) == 2)
        {
            chassis_exe_ptr_->Stop();
        }
        else if (chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[4].x, blackboard_ptr_->attack_pos[4].y) == 1)
        {
            chassis_exe_ptr_->Stop();
            num_3 = 6; // 到达3点
            last_time = ros::Time::now();
        }
    }
}
