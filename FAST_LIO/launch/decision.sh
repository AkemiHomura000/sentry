source ~/Desktop/nav/catkin_ws/devel/setup.bash

gnome-terminal -x bash -c "roslaunch sim_referee_system sim_referee_system.launch ;exec bash"
sleep 1s
gnome-terminal -x bash -c "roslaunch sp_decision sp_decision.launch ;exec bash"
