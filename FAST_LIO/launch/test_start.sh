#!/bin/bash
sleep 5
source /opt/ros/noetic/setup.bash
source /home/sentry/ws_livox/devel/setup.bash
source /home/sentry/sp_nav_ws/devel/setup.bash
export ROS_PACKAGE_PATH=/home/sentry/ws_livox/src:$ROS_PACKAGE_PATH
export ROS_PACKAGE_PATH=/home/sentry/sp_nav_ws/src:$ROS_PACKAGE_PATH
gnome-terminal -x bash -c "/home/sentry/sp_nav_ws/src/sp_nav/FAST_LIO/launch/start.sh ;exec bash"
sleep 3s
gnome-terminal -x bash -c "/home/sentry/sp_nav_ws/src/sp_nav/FAST_LIO/launch/relocalization_nav.sh ;exec bash"
sleep 1s
gnome-terminal -x bash -c "/home/sentry/sp_nav_ws/src/sp_nav/FAST_LIO/launch/usb2can.sh;exec bash"
sleep 1s
gnome-terminal -x bash -c "/home/sentry/sp_nav_ws/src/sp_nav/FAST_LIO/launch/decision.sh ;exec bash"
gnome-terminal -x bash -c "rosbag record /localization /referee_info /Enemy_robot_HP /enemy_stage /sentry/log /armor /move_base/status /cmd_vel /sentry/cmd_vel;exec bash"

