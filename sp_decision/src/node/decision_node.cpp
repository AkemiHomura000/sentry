#include "node/decision_node.h"
#include <executor/blackboard.hpp>

namespace sp_decision
{
    DecisionNode::~DecisionNode()
    {
        if (decision_thread_.joinable())
        {
            decision_thread_running_ = false;
            decision_thread_.join();
        }
        delete root_node_;
    }
    DecisionNode::DecisionNode()
    {
        nh_.param("loop_rate", loop_rate_, 5);

        blackboard_ = std::make_shared<Blackboard>();
        log_exe_ = std::make_shared<LogExecutor>();
        chassis_exe_ = std::make_shared<ChassisExecutor>(blackboard_);
        root_node_ = new SequenceNode("robot_decision", 0, blackboard_, chassis_exe_,
                                      log_exe_);
        AddBloodBehavior *add_blood_node_ =
            new AddBloodBehavior("add blood", 1, blackboard_, chassis_exe_, log_exe_);
        RetreatBehavior *retreat_node_ =
            new RetreatBehavior("add retreat", 2, blackboard_, chassis_exe_, log_exe_);
        AttackBehavior *attack_node_ =
            new AttackBehavior("attack", 3, blackboard_, chassis_exe_, log_exe_);
        DenfenceBehavior *defence_node_ =
            new DenfenceBehavior("defence", 4, blackboard_, chassis_exe_, log_exe_);
        PursuitBehavior *pursuit_node_ =
            new PursuitBehavior("pursuit", 5, blackboard_, chassis_exe_, log_exe_);
        PatrolBehavior *patrol_node_ =
            new PatrolBehavior("patrol", 6, blackboard_, chassis_exe_, log_exe_);
        root_node_->addChild(add_blood_node_, BehaviorPriority::HIGH);
        // root_node_->addChild(retreat_node_, BehaviorPriority::MID);
        root_node_->addChild(pursuit_node_, BehaviorPriority::MID);
        root_node_->addChild(attack_node_, BehaviorPriority::MID);
        // root_node_->addChild(defence_node_, BehaviorPriority::HIGH);
        // root_node_->addChild(patrol_node_, BehaviorPriority::LOW);

        std::stringstream str;
        str << std::endl
            << "***********************" << std::endl;
        str << "*Super Power Decision*" << std::endl;
        str << "***********************";
        log_exe_->info(str);
        root_node_->print_tree();
        str.str("");
        str << "*********START*********";
        log_exe_->info(str);
        decision_thread_ = std::thread(&DecisionNode::ExecuteLoop, this);
        decision_thread_running_ = true;
    }
    void DecisionNode::ExecuteLoop()
    {
        ros::Rate loop_rate(loop_rate_);
        int cnt = 0;
        while (decision_thread_running_)
        {
            if (blackboard_->game_status_ == Blackboard::MatchSatuts::AT_MATCH)
            {
                root_node_->Run();
            }
        }
        cnt++;
        if (cnt >= loop_rate_)
        {
            cnt = 0;
            blackboard_->ResetFlag();
        }
        std::cout << "a";
        loop_rate.sleep();
    }

} // namespace sp_decision