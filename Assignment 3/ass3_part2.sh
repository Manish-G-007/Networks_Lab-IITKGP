#!/bin/bash

# add the namespaces
sudo ip netns add H1
sudo ip netns add H2
sudo ip netns add H3
sudo ip netns add R
# create the bridge
BRIDGE=br-test
sudo ip netns exec R brctl addbr $BRIDGE
sudo ip netns exec R brctl stp br-test off
sudo ip netns exec R ip link set dev br-test up
#

#### PORT 1
# create a port pair
sudo ip link add veth1 type veth peer name veth2
# attach one side to linuxbridge
sudo ip link set veth2 netns R
sudo ip netns exec R brctl addif br-test veth2 
# attach the other side to namespace
sudo ip link set veth1 netns H1
#add ip
sudo ip netns exec H1 ip addr add 10.0.10.20/24 dev veth1
sudo ip addr add 10.0.10.1/24 dev veth2
# set the ports to up
sudo ip netns exec H1 ip link set dev veth1 up
sudo ip netns exec R ip link set veth2 up master br-test
# set default gateway
sudo ip netns exec H1 ip route add default via 10.0.10.20
#

#### PORT 2
# create a port pair
sudo ip link add veth6 type veth peer name veth3
# attach one side to linuxbridge
sudo ip link set veth3 netns R
sudo ip netns exec R brctl addif br-test veth3
# attach the other side to namespace
sudo ip link set veth6 netns H2
#add ip
sudo ip netns exec H2 ip addr add 10.0.20.20/24 dev veth6
sudo ip addr add 10.0.20.1/24 dev veth3
# set the ports to up
sudo ip netns exec H2 ip link set dev veth6 up
sudo ip netns exec R ip link set veth3 up master br-test
# set default gateway
sudo ip netns exec H2 ip route add default via 10.0.20.20
#

#### PORT 3
# create a port pair
sudo ip link add veth5 type veth peer name veth4
# attach one side to linuxbridge
sudo ip link set veth4 netns R
sudo ip netns exec R brctl addif br-test veth4
# attach the other side to namespace
sudo ip link set veth5 netns H3
#add ip
sudo ip netns exec H3 ip addr add 10.0.30.20/24 dev veth5
sudo ip addr add 10.0.30.1/24 dev veth4
# set the ports to up
sudo ip netns exec H3 ip link set dev veth5 up
sudo ip netns exec R ip link set veth4 up master br-test
# set default gateway
sudo ip netns exec H3 ip route add default via 10.0.30.20
#


sudo sysctl -w net.ipv4.ip_forward=1