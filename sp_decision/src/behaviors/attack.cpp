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
            if (blackboard_ptr_->stage_remain_time > 290) // 假设需要十秒买弹
            {
                ROS_INFO("attack_1");
                attack_point_1();
                log_exe_ptr_->info("behavior: attack");
                blackboard_ptr_->action_status_ = Blackboard::Action_Lock::ATTACK;
                return BehaviorState::SUCCESS;
            }
            else if (blackboard_ptr_->stage_remain_time > 270 && blackboard_ptr_->armor_.track_status == 1) // 假设在前30秒自瞄锁定到对方烧饼
            {
                ROS_INFO("attack_1");
                attack_point_1();
                log_exe_ptr_->info("behavior: attack");
                blackboard_ptr_->action_status_ = Blackboard::Action_Lock::ATTACK;
                return BehaviorState::SUCCESS;
            }
            else if (blackboard_ptr_->attack == 1 || blackboard_ptr_->stage_remain_time < 120) // 冲家
            {
                switch (num_3)
                {
                case 0:
                {
                    attack_point_3();
                    num_3 = 1;
                    break;
                }
                case 1:
                {
                    attack_point_3();
                    break;
                }
                case 2:
                {
                    chassis_exe_ptr_->observe(-30, 90);
                    if ((ros::Time::now().sec - last_time.sec) > 4) // 观察四秒
                    {
                        if (blackboard_ptr_->armor_.track_status != 1)
                        {
                            num_3 = 3;
                        }
                    }
                    break;
                }
                case 3:
                {
                    chassis_exe_ptr_->observe(0, 180);
                    attack_point_4();
                    break;
                }
                case 4:
                {
                    chassis_exe_ptr_->observe(0, 150);
                    if ((ros::Time::now().sec - last_time.sec) > 4) // 观察四秒
                    {
                        if (blackboard_ptr_->armor_.track_status != 1)
                        {
                            num_3 = 5;
                        }
                    }
                    break;
                }
                case 5:
                {
                    chassis_exe_ptr_->observe(-180, 180);
                    attack_point_5();
                    break;
                }
                case 6:
                {
                    if (blackboard_ptr_->enemy_stage_[1] != 2 && blackboard_ptr_->enemy_stage_[2] != 2 && blackboard_ptr_->enemy_stage_[3] != 2) // 对方全死后推塔
                    {
                        chassis_exe_ptr_->observe(30, 90);
                    }
                    else
                    {
                        chassis_exe_ptr_->observe(-180, 180);
                    }
                }
                }
            }
            else // 占中
            {
                ROS_INFO("attack_2");
                attack_point_2();
                log_exe_ptr_->info("behavior: attack");
                blackboard_ptr_->action_status_ = Blackboard::Action_Lock::ATTACK;
                ROS_INFO("status    %d", blackboard_ptr_->action_status_);
                return BehaviorState::SUCCESS;
            }
        }
        return BehaviorState::FAILURE;
    }

    void AttackBehavior::attack_point_1() // TODO：补充云台方向控制——————————守补给点
    {
        chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[0].x, blackboard_ptr_->attack_pos[0].y);
    }
    void AttackBehavior::attack_point_2() // TODO：补充云台方向控制———————————占中心
    {
        ROS_INFO("attack_center");
        if (chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[1].x, blackboard_ptr_->attack_pos[1].y) == 2)
        {
            chassis_exe_ptr_->Stop();
        }
        else if (chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[1].x, blackboard_ptr_->attack_pos[1].y) == 1)
        {
            chassis_exe_ptr_->Stop();
        }
    }
    void AttackBehavior::attack_point_3() // TODO：补充云台方向控制———————————冲对面家,点1
    {
        ROS_INFO("attack_3");
        if (chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[2].x, blackboard_ptr_->attack_pos[2].y) == 2)
        {
            chassis_exe_ptr_->Stop();
        }
        else if (chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[2].x, blackboard_ptr_->attack_pos[2].y) == 1)
        {
            chassis_exe_ptr_->Stop();
            num_3 = 2; // 到达1点
            last_time = ros::Time::now();
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
