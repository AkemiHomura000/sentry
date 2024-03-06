gnome-terminal -x bash -c "./start.sh ;exec bash"
sleep 3s
gnome-terminal -x bash -c "./relocalization_nav.sh ;exec bash"
sleep 1s
gnome-terminal -x bash -c "./usb2can.sh;exec bash"
gnome-terminal -x bash -c "./relay.sh;exec bash"
sleep 1s
gnome-terminal -x bash -c "./decision.sh ;exec bash"


