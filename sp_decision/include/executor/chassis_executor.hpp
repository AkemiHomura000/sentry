/**
 * @file chassis_executor.hpp
 * @author liwei
 * @brief  发送给sp_planning的目标点以及机器人状态，用于代替aerial_navigation
 * @version 0.1
 * @date 2023-06-13
 * DANGER: 该段代码还没有在虚拟/实际的裁判系统上测试
 */
#ifndef CHASSIS_EXECUTOR_H
#define CHASSIS_EXECUTOR_H

#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <robot_msg/RobotStateMsg.h>
#include <ros/ros.h>
#include <executor/blackboard.hpp>
#include <cmath>
#include "executor/blackboard.hpp"

// 给MoveBaseAction定义一个别名，方便创建对象
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> Client;

class ChassisExecutor
{
public:
  enum RobotState
  {
    MOVE,
    CRUISR,
    IDLE,
    FAST,
    STOP,
    ROTATE,
    PURSUIT,
    SlOW,
  };
  ChassisExecutor(const sp_decision::Blackboard::Ptr &blackboard_ptr);
  typedef std::shared_ptr<ChassisExecutor> Ptr;
  void robotStatePub(RobotState robot_state);
  bool Move(double pos_x, double pos_y);
  void QueueMove(std::vector<sp_decision::Blackboard::Point> points, sp_decision::Blackboard::Action_Lock action, int stay_time = 0); // 需保证不同动作调用该函数时不会混淆
  void QueueMoveSlow(std::vector<sp_decision::Blackboard::Point> points, sp_decision::Blackboard::Action_Lock action);
  bool FastMove(double pos_x, double pos_y);
  void Cruisr(double pos_x, double pos_y);
  void VelIdle();
  void VelStop();
  void Stop();
  void Idle();
  void Pursuit(double pos_x, double pos_y);
  void SendDataToPlan(double pos_x, double pos_y);
  bool GetMoveStatus();
  bool move_status = 0;                                                                               // 移动完成
  int num = -1;                                                                                       // 目标点序号
  sp_decision::Blackboard::Action_Lock action_status = sp_decision::Blackboard::Action_Lock::JUDGING; // 记录调用QueueMove()的动作来源
  geometry_msgs::Twist sentry_cmdvel_;

private:
  sp_decision::Blackboard::Ptr blackboard_;
  ros::NodeHandle nh_;
  ros::Publisher set_goal_pub_;
  ros::Publisher robot_state_pub_;
  ros::Publisher sentry_cmdvel_pub_;
  geometry_msgs::PoseStamped target_pose_;
  move_base_msgs::MoveBaseGoal goal_;
  double max_vel_theta_;
  ros::Time last_time = ros::Time(0, 0); // 用于发布目标点的计时器
};

#endif