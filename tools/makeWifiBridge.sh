sudo brctl addbr br0
#sudo ip addr flush dev eth0
#sudo brctl addif br0 eth0
sudo tunctl -t tap0 -u $1
sudo brctl addif br0 tap0
#sudo ip link set eth0 up
sudo ip link set tap0 up
sudo ip link set br0 up
#sudo dhclient -v br0
