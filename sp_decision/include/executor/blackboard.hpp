/**
 * @file blackboard.hpp
 * @author liwei
 * @brief
 * @version 0.1
 * @date 2023-06-13
 * DANGER:
 */
#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <nav_msgs/Odometry.h>
#include <robot_msg/MatchMsg.h>
#include <robot_msg/RefereeInfoMsg.h>
#include <robot_msg/Armor.h>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <actionlib_msgs/GoalStatusArray.h>
#include "robot_msg/RobotHP.h"
namespace sp_decision
{

  class Blackboard
  {
  public:
    typedef std::shared_ptr<Blackboard> Ptr;
    Blackboard();
    ~Blackboard() {}
    std::mutex match_status_cbk_mutex;
    std::mutex robot_odom_cbk_mutex;
    std::mutex referee_data_cbk_mutex;
    std::mutex move_base_status_cbk_mutex;
    std::mutex nav_cmd_vel_cbk_mutex;
    std::mutex goal_status_mutex;
    std::mutex referee_info_mutex;
    std::mutex armor_mutex;
    void ResetFlag();

    /**
     * @brief
     */
    struct Point
    {
      double x;
      double y;
    };

    //远离门口一侧
    // std::vector<Point> buff_pos_ = {{-1.38, 1.77},{-1.38,1.57}};
    // std::vector<Point> buff_queue_pos_ = {{-1.48, 1.77},{-1.18, 1.77},{-1.38, 1.67},{-1.28, 1.87}};
    // std::vector<Point> backward_defence_pos_ = {{0.5, 0.5}, {1.6, 0.7}};   // base周围点位
    // std::vector<Point> backward_defence_queue_pos_ = {{1, -1}, {2, -1.5}}; // base周围点位
    // std::vector<Point> random_mode_pos = {{0.0, 0.0}, {0.5, 0.0}};
    // std::vector<Point> patrol_pos = {{0.0, 0.0}, {-0.8, 0.0}, {0.0, -0.8}};
    // std::vector<Point> attack_pos = {{1.7, 0}, {0.4, 1.6}, {1, -1.1}};
    // std::vector<Point> attack_queue_pos = {{1, -1.2}, {1.1, -1.2}};

    std::vector<Point> buff_pos_ = {{-1,1}, {0 ,0}};
    //std::vector<Point> buff_queue_pos_ = {{-1.48, 1.77}, {-1.18, 1.77}, {-1.38, 1.67}, {-1.28, 1.87}};
    std::vector<Point> backward_defence_pos_ = {{0.5, 0.5}, {1.6, 0.7}};   // base周围点位
    std::vector<Point> backward_defence_queue_pos_ = {{1, -1}, {2, -1.5}}; // base周围点位
    std::vector<Point> random_mode_pos = {{0.0, 0.0}, {1,-0.2}};
    std::vector<Point> patrol_pos = {{0.0, 0.0}, {-0.8, 0.0}, {0.0, -0.8}};
    std::vector<Point> attack_pos = {{1.7, 0}, {0.6, 1.1}, {1, -1.1}};
    std::vector<Point> attack_queue_pos = {{1.4,-1.8}, {1.3,-1.7}};
    int min_hp_;
    int min_bullet_;
    int min_outpost_;
    float distance_tolerance_;

    /**
     * @brief
     * test_id
     */
    uint8_t test_id;
    uint8_t game_progress;
    uint16_t stage_remain_time;
    uint16_t robot_hp_ = 600;
    uint16_t Sentry_HP_ = 600; // 敌方烧饼血量
    uint16_t robot_bullet_;
    uint16_t base_HP_ = 3000;
    ros::Time time_received_armor_;
    robot_msg::Armor armor_;
    nav_msgs::Odometry robot_pose_;

    /**
     * @brief
     */
    float posx_x_;
    float posy_y_;
    float key_z_;
    /**
     * @brief move_base
     */
    geometry_msgs::Twist vel_msg_sub_;

    // 比赛状态?
    enum class MatchSatuts
    {
      TO_BEGIN,
      AT_MATCH,
      AFTER_MATCH
    };
    MatchSatuts game_status_;

    // 动作状态?
    enum class Action_Lock
    {
      ADD_BLOOD,
      BACKWARD_DEFENCE,
      RETREAT,
      PURSUIT,
      ATTACK,
      PATROL,
      JUDGING,
    };
    Action_Lock action_status_ = Action_Lock::JUDGING;

    /*
     *@brife 决策所需变量
     */
    int available_hp_ = 600; // 剩余可加血量?
    bool plan_get_ = 0;      // 是否规划出路径规划出路径
    bool base_attacked_ = 0; // 基地受击状态?
    ros::Time current_time;  // 用于基地受击状态更新?
    int current_hp;
    bool status_init = 0; // 状态初始化
    ros::Time time_1;
    bool sentry_attacked_ = 0;    // 烧饼受击状态
    bool attacked_violently_ = 0; // 掉血速度过快
    bool armor_received_ = 0;     // 装甲板接收状态
    geometry_msgs::Point last_position;//记录位置
  private:
    ros::NodeHandle nh_;
    ros::Subscriber match_status_sub_;
    ros::Subscriber robot_odom_sub_;
    ros::Subscriber referee_data_sub_;
    ros::Subscriber move_base_status_sub_;
    ros::Subscriber cmd_vel_sub_;
    ros::Subscriber goal_status_sub_;
    ros::Subscriber referee_info_sub_;
    ros::Subscriber armor_sub_;
    ros::Subscriber enemy_hp_sub_;
    bool robot_odom_received_;
    bool match_state_received_;
    bool goal_status_received_;
    bool referee_info_received_;

    void GoalStatusCallback(const actionlib_msgs::GoalStatusArray::ConstPtr &msg);
    void MatchStatusCallback(const robot_msg::MatchMsg::ConstPtr msg);
    void RobotPoseCallback(const nav_msgs::Odometry::ConstPtr msg);
    void RefereeDataCallback(const geometry_msgs::Point::ConstPtr &msg);
    void CmdVelDataCallback(const geometry_msgs::Twist &msg);
    void RefereeInfoCallback(const robot_msg::RefereeInfoMsg::ConstPtr &msg);
    void ArmorCallback(const robot_msg::Armor::ConstPtr &msg);
    void EnemyCallback(const robot_msg::RobotHP::ConstPtr &msg);
  };
} // namespace sp_decision
#endif