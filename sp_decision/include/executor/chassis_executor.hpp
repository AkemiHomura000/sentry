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
#include "robot_msg/CmdGimbal.h" // 导入自定义消息类型

// 给MoveBaseAction定义一个别名，方便创建对象
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> Client;
enum Implementation
{
  IN_PROGRESS,
  FINISHED,
  FAILED,
};
class ChassisExecutor
{
public:
    std::mutex gimbal_mutex;
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
  Implementation Move(double pos_x, double pos_y);
  Implementation QueueMove(std::vector<sp_decision::Blackboard::Point> points, sp_decision::Blackboard::Action_Lock action, int stay_time = 0); // 需保证不同动作调用该函数时不会混淆
  Implementation QueueMoveSlow(std::vector<sp_decision::Blackboard::Point> points, sp_decision::Blackboard::Action_Lock action);
  Implementation FastMove(double pos_x, double pos_y);
  Implementation Cruisr(double pos_x, double pos_y);
  Implementation VelIdle();
  Implementation VelStop();
  Implementation Stop();
  Implementation Idle();
  Implementation Pursuit(double pos_x, double pos_y);
  Implementation SendDataToPlan(double pos_x, double pos_y);
  void observe(double min_angle, double max_angle);
  bool GetMoveStatus();
  bool move_status = 0;                                                                               // 移动完成
  int num = -1;                                                                                       // 目标点序号
  sp_decision::Blackboard::Action_Lock action_status = sp_decision::Blackboard::Action_Lock::JUDGING; // 记录调用QueueMove()的动作来源
  geometry_msgs::Twist sentry_cmdvel_;
  Implementation exec_stauts;
  bool control_gimbal=0;//是否需要控制云台
  int number_1=0;
private:
  sp_decision::Blackboard::Ptr blackboard_;
  ros::NodeHandle nh_;
  ros::Publisher set_goal_pub_;
  ros::Publisher robot_state_pub_;
  ros::Publisher sentry_cmdvel_pub_;
  ros::Publisher gimbal_pub_;
  geometry_msgs::PoseStamped target_pose_;
  move_base_msgs::MoveBaseGoal goal_;
  double max_vel_theta_;
  ros::Time last_time = ros::Time(0, 0); // 用于发布目标点的计时器
};

#endif