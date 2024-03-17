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
            else
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

    void AttackBehavior::attack_point_1() // TODO：补充云台方向控制
    {
        if (num_1 == 0)
        {
            chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[1].x, blackboard_ptr_->attack_pos[1].y);
            num_1++;
        }
        if (chassis_exe_ptr_->GetMoveStatus())
        {
            chassis_exe_ptr_->VelIdle();
        }
    }
    void AttackBehavior::attack_point_2() // TODO：补充云台方向控制
    {
        // if (num_2 == 0)
        // {
        //     chassis_exe_ptr_->FastMove(blackboard_ptr_->attack_pos[2].x, blackboard_ptr_->attack_pos[2].y);
        //     num_2++;
        // }
        // if (chassis_exe_ptr_->GetMoveStatus())
        // {
        //     chassis_exe_ptr_->VelIdle();
        // }
         chassis_exe_ptr_->QueueMove(blackboard_ptr_->attack_queue_pos,blackboard_ptr_->action_status_, 3);
    }
}
