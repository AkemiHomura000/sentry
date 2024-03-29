cmake_minimum_required(VERSION 3.0.2)
project(sp_decision)
# 设置编译选项
# option(DEBUG_MODE "Enable debug mode" OFF)

# # 添加可选的编译宏定义
# if(DEBUG_MODE)
#     add_compile_definitions(DEBUG_MODE)
# endif()


find_package(catkin REQUIRED COMPONENTS
  robot_msg
  roscpp
  rospy
  geometry_msgs
  nav_msgs
  actionlib
  actionlib_msgs
  tf
  tf2_ros
  tf2_geometry_msgs
)
find_package(Eigen3 REQUIRED)


include_directories(${EIGEN3_INCLUDE_DIR})
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES sp_decision
#  CATKIN_DEPENDS robot_msg roscpp rospy
#  DEPENDS system_lib
)

include_directories(
# include
  include
  ${CMAKE_CURRENT_SOURCE_DIR} 
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/node/behavior_tree.cpp
  src/node/decision_node.cpp
  src/executor/blackboard.cpp
  src/executor/chassis_executor.cpp
  src/executor/log_executor.cpp
  src/behaviors/add_blood.cpp
  src/behaviors/attack.cpp
  src/behaviors/retreat.cpp
  src/behaviors/backward_defence.cpp
  src/behaviors/pursuit.cpp
  src/behaviors/patrol.cpp
)
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)

add_executable(armor_relay src/executor/armor_relay.cpp)
target_link_libraries(armor_relay 
  ${catkin_LIBRARIES}
) 

add_executable(vel_relay src/vel_relay.cpp)
target_link_libraries(vel_relay 
  ${catkin_LIBRARIES}
) 

add_executable(get_plan src/executor/move_base_test.cpp)
target_link_libraries(get_plan 
  ${catkin_LIBRARIES}
) 

add_executable(enemy src/test.cpp)
target_link_libraries(enemy 
  ${catkin_LIBRARIES}
) 