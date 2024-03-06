#include "behaviors/add_blood.hpp"

namespace sp_decision
{
    BehaviorState AddBloodBehavior::Update()
    {
        if (blackboard_ptr_->test_id == 1)
        {
           // ros::Duration(0.1).sleep();
            //ROS_INFO("add_blood");
            Go2Buff();
            log_exe_ptr_->info("behavior: add blood");
            return BehaviorState::SUCCESS;
        }
        return BehaviorState::FAILURE;
    }

    void AddBloodBehavior::Go2Buff()
    {
        // double distance=sqrt(pow(blackboard_ptr_->robot_pose_.pose.pose.position.x - blackboard_ptr_->prepare_pos_[0].x, 2) + pow(blackboard_ptr_->robot_pose_.pose.pose.position.x - blackboard_ptr_->prepare_pos_[0].y, 2));
        // if(distance>0.35)
        // {
        //     chassis_exe_ptr_->FastMove(blackboard_ptr_->prepare_pos_[0].x,
        //                                blackboard_ptr_->prepare_pos_[0].y);
        //                                ROS_INFO("distance %f",distance);
        // }
        // else
        // { 
            chassis_exe_ptr_->Cruisr(blackboard_ptr_->buff_pos_[0].x,
                                   blackboard_ptr_->buff_pos_[0].y);
        // }
    }
}