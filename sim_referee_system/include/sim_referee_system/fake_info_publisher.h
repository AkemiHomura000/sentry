#ifndef FAKE_INFO_H
#define FAKE_INFO_H
#include <robot_msg/MatchMsg.h>
#include <ros/ros.h>
#include <robot_msg/RefereeInfoMsg.h>
#include <geometry_msgs/Point.h>
#include <robot_msg/RobotHP.h>
#include <iostream>
#include <random>
#include <string>
#include <thread>
class FakeInfoPublisher
{
public:
  FakeInfoPublisher()
  {
    fake_info_publihser_ =
        nh_.advertise<robot_msg::MatchMsg>("match_status", 1);
    fake_referee_data_publisher_ =
        nh_.advertise<geometry_msgs::Point>("referee_data", 1);
    fake_referee_info_publisher_ =
        nh_.advertise<robot_msg::RefereeInfoMsg>("referee_info", 1);
    enemy_hp_publisher_ =
        nh_.advertise<robot_msg::RobotHP>("Enemy_robot_HP", 1);
    match_msg_.match_state = 0;
    referee_msg_.game_progress = 1;
    referee_msg_.stage_remain_time = 300;
    referee_msg_.base_HP = 500;
    match_msg_.robot_bullet = 750;
    referee_msg_.robot_HP = 600;
    match_msg_.test_id = 0;

    lower_referee_data_.x = 0.0;
    lower_referee_data_.y = 0.0;
    lower_referee_data_.z = 0.0;
  }
  void initialization(int id);
  void Remainder(int time);
  void Test(int id, FakeInfoPublisher *fakeinfo);
  void GameStart();
  void Attacked();
  void LackBullet();
  void UserSetStatus(int match_state, int robot_HP, int robot_bullet, int base_hp);
  void SwitchScenarios(int id);
  void FakeInfoPub();
  void FakeRefereeDataPub();
  std::thread countdown_thread;

private:
  ros::NodeHandle nh_;
  ros::Publisher fake_info_publihser_;
  ros::Publisher fake_referee_data_publisher_;
  ros::Publisher fake_referee_info_publisher_;
  ros::Publisher enemy_hp_publisher_;
  robot_msg::MatchMsg match_msg_;
  robot_msg::RefereeInfoMsg referee_msg_;
  robot_msg::RobotHP enemy_hp_;
  geometry_msgs::Point lower_referee_data_;
  struct RandomNumberGenerator
  {
  public:
    RandomNumberGenerator() : generator_(std::random_device()()) {}

    int generateRandomInt(int min, int max)
    {
      std::uniform_int_distribution<int> distribution(min, max);
      return distribution(generator_);
    }

  private:
    std::mt19937 generator_;
  };
  RandomNumberGenerator rng_;
};

#endif