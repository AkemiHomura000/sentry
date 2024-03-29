#include "node/decision_node.h"
#include <executor/blackboard.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

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
        ros::Time t = ros::Time::now();
        boost::posix_time::ptime time = t.toBoost();
        int year = time.date().year();
        int month = time.date().month();
        int day = time.date().day();
        int hour = time.time_of_day().hours()+8;
        int minute = time.time_of_day().minutes();
        int second = time.time_of_day().seconds();

        std::stringstream ss;
        ss << "/home/sentry/bag/" << year << "-" << month << "-" << day << "-" << hour << "-" << minute << "-" << second << ".bag";
        std::string path = ss.str();
        std::string topics = " /localization /referee_info /Enemy_robot_HP /enemy_stage /sentry/log /armor /move_base/status /cmd_vel /sentry/cmd_vel";
        std::string node_name = " __name:=my_record_node";
        std::string cmd_str = "gnome-terminal -x bash -c 'rosbag record -O " + path + topics + node_name + "'";
        int ret = system(cmd_str.c_str()); // #include <stdlib.h>

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
        // root_node_->addChild(pursuit_node_, BehaviorPriority::MID);
        root_node_->addChild(attack_node_, BehaviorPriority::MID);
        // root_node_->addChild(defence_node_, BehaviorPriority::HIGH);
        // root_node_->addChild(patrol_node_, BehaviorPriority::LOW);

        std::stringstream str;
        str << "*Super Power Decision*" << std::endl;
        str << "***********************";
        str << "*********START*********";
        blackboard_->LogPub(str.str());
        decision_thread_ = std::thread(&DecisionNode::ExecuteLoop, this);
        decision_thread_running_ = true;
    }
    void DecisionNode::ExecuteLoop()
    {
        ros::Rate loop_rate(loop_rate_);
        int cnt = 0;
        while (decision_thread_running_)
        {
            if (blackboard_->game_status_ == Blackboard::MatchSatuts::AFTER_MATCH)
            {
                ros::V_string v_nodes;
                ros::master::getNodes(v_nodes);

                std::string node_name = std::string("/my_record_node");
                auto it = std::find(v_nodes.begin(), v_nodes.end(), node_name.c_str());
                if (it != v_nodes.end())
                {
                    std::string cmd_str = "rosnode kill " + node_name;
                    int ret = system(cmd_str.c_str());
                    std::cout << "## stop rosbag record cmd: " << cmd_str << std::endl;
                }
            }
            if (blackboard_->game_status_ != Blackboard::MatchSatuts::AT_MATCH)
            {
                std::stringstream str;
                str << "progress!=4,idle";
                switch (blackboard_->game_status_)
                {
                case Blackboard::MatchSatuts::TO_BEGIN:
                    str << "TO_BEGIN";
                    break;
                case Blackboard::MatchSatuts::AT_MATCH:
                    str << "AT_MATCH";
                    break;
                case Blackboard::MatchSatuts::AFTER_MATCH:
                    str << "AFTER_MATCH";
                    break;
                default:
                    str << "Unknown";
                    break;
                }
                chassis_exe_->Idle();
                chassis_exe_->observe(-180, 180);
                blackboard_->LogPub(str.str());
            }
            else
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