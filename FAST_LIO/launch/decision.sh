source ~/sp_nav_ws/devel/setup.bash

gnome-terminal -x bash -c "roslaunch sim_referee_system sim_referee_system.launch ;exec bash"
gnome-terminal -x bash -c "roslaunch sp_decision sp_decision.launch ;exec bash"
