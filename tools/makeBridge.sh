sudo brctl addbr br0
sudo ip addr flush dev $1
sudo brctl addif br0 $1
sudo tunctl -t tap0 -u $USER
sudo brctl addif br0 tap0
sudo ip link set $1 up
sudo ip link set tap0 up
sudo ip link set br0 up
sudo dhclient -v br0