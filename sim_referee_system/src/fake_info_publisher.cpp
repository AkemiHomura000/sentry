#include <fake_info_publisher.h>
#include <sstream>
#include <chrono>
void FakeInfoPublisher::initialization(int id = 4)
{
  switch (id)
  {
  case 1:
  {
    referee_msg_.game_progress = 4;
    break;
  }
  case 4:
  {
    referee_msg_.game_progress = 4;
    break;
  }
  }
}
void FakeInfoPublisher::Remainder(int time)
{
  if (time > 0)
  {
    referee_msg_.stage_remain_time = time;
  }
}
void FakeInfoPublisher::Test(int id, FakeInfoPublisher *fakeinfo)
{
  switch (id)
  {
  case 1:
     referee_msg_.stage_remain_time=300;
    referee_msg_.base_HP = 3000;
    referee_msg_.game_progress = 3;
    break;
  case 2:
   referee_msg_.stage_remain_time=300;
    referee_msg_.base_HP = 3000;
    referee_msg_.game_progress = 4;
    break;
  case 3:
  {
    std::string userinput;
    int userorder[2];
    std::cout << "依次键入数据 :  剩余时间 血�?" << std::endl;
    std::getline(std::cin, userinput);
    std::istringstream userstr(userinput);
    // �? std::istringstream 中提取整数，并存储在 vector �?
    for (int j = 0; j < 2; j++)
    {
      userstr >> userorder[j];
    }
    referee_msg_.base_HP = 3000;
    referee_msg_.stage_remain_time = userorder[0];
    ROS_INFO("time%d",referee_msg_.stage_remain_time);
    match_msg_.robot_hp = userorder[1];
      ROS_INFO("hp%d",match_msg_.robot_hp);
    break;
  }
  case 4:
  {
    std::string userinput;
    int userorder[1];
    std::cout << "依次键入数据 :基地血�?" << std::endl;
    std::getline(std::cin, userinput);
    std::istringstream userstr(userinput);
    // �? std::istringstream 中提取整数，并存储在 vector �?
    for (int j = 0; j < 1; j++)
    {
      userstr >> userorder[j];
    }
    referee_msg_.base_HP = userorder[0];
    break;
  }
  case 5:
  {
    std::string userinput;
    int userorder[2];
    std::cout << "依次键入数据 :烧饼掉血速度(/s),剩余时间" << std::endl;
    std::getline(std::cin, userinput);
    std::istringstream userstr(userinput);
    for (int j = 0; j < 2; j++)
    {
      userstr >> userorder[j];
    }
    for (int n = 0; n < 100; n++)
    {
      match_msg_.robot_hp = 600 - 0.1 * n * userorder[0];
      referee_msg_.stage_remain_time = userorder[1];
      fakeinfo->FakeInfoPub();
      ros::Duration(0.1).sleep();
    }
  }
    // match_msg_.test_id = id;
  }
}
void FakeInfoPublisher::GameStart() { match_msg_.match_state = 1; }
void FakeInfoPublisher::Attacked()
{
  match_msg_.match_state = 1;
  match_msg_.robot_hp = rng_.generateRandomInt(0, 200);
  match_msg_.robot_bullet = rng_.generateRandomInt(0, 750);
  referee_msg_.base_HP = rng_.generateRandomInt(0, 600);
}
void FakeInfoPublisher::LackBullet()
{
  match_msg_.match_state = 1;
  match_msg_.robot_hp = rng_.generateRandomInt(0, 400);
  match_msg_.robot_bullet = rng_.generateRandomInt(0, 100);
  referee_msg_.base_HP = rng_.generateRandomInt(0, 600);
}
void FakeInfoPublisher::UserSetStatus(int match_state, int robot_hp, int robot_bullet, int base_hp)
{
  match_msg_.match_state = match_state;
  match_msg_.robot_hp = robot_hp;
  match_msg_.robot_bullet = robot_bullet;
  referee_msg_.base_HP = base_hp;
}
std::string userinput;
int userorder[4];
std::string playerorder[3];
void FakeInfoPublisher::SwitchScenarios(int id)
{
  switch (id)
  {
  case 1:
    GameStart();
    break;
  case 2:
    Attacked();
    break;
  case 3:
    LackBullet();
    break;
  case 4:
  {
    std::cout << "依次键入数据 : 比赛状�? 血�? 弹丸 前哨�?" << std::endl;
    std::getline(std::cin, userinput);
    std::istringstream userstr(userinput);
    // �? std::istringstream 中提取整数，并存储在 vector �?
    for (int j = 0; j < 4; j++)
    {
      userstr >> userorder[j];
    }
    UserSetStatus(userorder[0], userorder[1], userorder[2], userorder[3]);
    break;
  }
  case 5:
  {
    std::cout << "依次键入数据:云台手标点x,云台手标点y,云台手按键；" << std::endl;
    std::getline(std::cin, userinput);
    std::istringstream userstr(userinput);
    for (int k = 0; k < 3; k++)
    {
      userstr >> playerorder[k];
      std::cout << playerorder[k] << " ";
    }
    lower_referee_data_.x = std::stof(playerorder[0]);
    lower_referee_data_.y = std::stof(playerorder[1]);
    for (char ch : playerorder[2])
    {
      lower_referee_data_.z = static_cast<float>(ch);
    }
    FakeRefereeDataPub();
    break;
  }
  default:
    ROS_WARN_STREAM("Scenario error:" << id);
    break;
  }
  // 场景选择错误，信息保留上一�?
  FakeInfoPub();
}

void FakeInfoPublisher::FakeInfoPub()
{
  fake_info_publihser_.publish(match_msg_);
  fake_referee_info_publisher_.publish(referee_msg_);
  fake_referee_data_publisher_.publish(lower_referee_data_);
}

void FakeInfoPublisher::FakeRefereeDataPub()
{
  fake_referee_data_publisher_.publish(lower_referee_data_);
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "sp_decision_node");
  FakeInfoPublisher fakeinfo;
  std::string input;
  fakeinfo.initialization();
  ros::Rate r(1);
  while (ros::ok())
  {
    std::cout << "Enter the test_id: ";
    std::getline(std::cin, input);
    if (input == "q")
      break;
    int id = std::stoi(input);
    fakeinfo.Test(id, &fakeinfo);
    // std::cout << "Enter the remainder(/s): ";
    // std::getline(std::cin, input);
    // int time = std::stoi(input);
    // fakeinfo.Remainder(time);
    // std::cout << "Enter a scenario id (or 'quit' to exit): ";
    // std::getline(std::cin, input);
    // if (input == "quit")
    // {
    //   break;
    // }
    // int id = std::stoi(input);
    // fakeinfo.SwitchScenarios(id);
    fakeinfo.FakeInfoPub();
    r.sleep();
  }
  return 0;
}