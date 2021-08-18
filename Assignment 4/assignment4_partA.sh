#!/bin/bash

# Creating network namespaces
sudo ip netns add N1
sudo ip netns add N2
sudo ip netns add N3
sudo ip netns add N4


# Creating and adding veth pairs
sudo ip link add v1 type veth peer name v2
sudo ip link add v3 type veth peer name v4
sudo ip link add v5 type veth peer name v6

sudo ip link set v1 netns N1
sudo ip link set v2 netns N2
sudo ip link set v3 netns N2
sudo ip link set v4 netns N3
sudo ip link set v5 netns N3
sudo ip link set v6 netns N4

# Assigning ip Address to veth pairs and activating them
sudo ip netns exec N1 ip addr add 10.0.10.20/24 dev v1
sudo ip netns exec N2 ip addr add 10.0.10.21/24 dev v2
sudo ip netns exec N2 ip addr add 10.0.20.20/24 dev v3
sudo ip netns exec N3 ip addr add 10.0.20.21/24 dev v4
sudo ip netns exec N3 ip addr add 10.0.30.20/24 dev v5
sudo ip netns exec N4 ip addr add 10.0.30.21/24 dev v6

sudo ip netns exec N1 ip link set dev v1 up
sudo ip netns exec N2 ip link set dev v2 up
sudo ip netns exec N2 ip link set dev v3 up
sudo ip netns exec N3 ip link set dev v4 up
sudo ip netns exec N3 ip link set dev v5 up
sudo ip netns exec N4 ip link set dev v6 up

sudo ip netns exec N1 ip link set dev lo up
sudo ip netns exec N2 ip link set dev lo up
sudo ip netns exec N3 ip link set dev lo up
sudo ip netns exec N4 ip link set dev lo up

# Enabling network forwarding
sudo ip netns exec N1 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec N2 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec N3 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec N4 sysctl -w net.ipv4.ip_forward=1

# configuring ip route tables

sudo ip netns exec N1 ip route add 10.0.20.0/24 via 10.0.10.21 dev v1
sudo ip netns exec N1 ip route add 10.0.30.0/24 via 10.0.10.21 dev v1

sudo ip netns exec N2 ip route add 10.0.30.0/24 via 10.0.20.21 dev v3

sudo ip netns exec N3 ip route add 10.0.10.0/24 via 10.0.20.20 dev v4

sudo ip netns exec N4 ip route add 10.0.20.0/24 via 10.0.30.20 dev v6
sudo ip netns exec N4 ip route add 10.0.10.0/24 via 10.0.30.20 dev v6


# Pinging
# sudo ip netns exec N1 ping -c3 10.0.10.20
sudo ip netns exec N1 ping -c3 10.0.10.21
# sudo ip netns exec N1 ping -c3 10.0.20.20
sudo ip netns exec N1 ping -c3 10.0.20.21
# sudo ip netns exec N1 ping -c3 10.0.30.20
sudo ip netns exec N1 ping -c3 10.0.30.21

sudo ip netns exec N2 ping -c3 10.0.10.20
# sudo ip netns exec N2 ping -c3 10.0.10.21
# sudo ip netns exec N2 ping -c3 10.0.20.20
sudo ip netns exec N2 ping -c3 10.0.20.21
# sudo ip netns exec N2 ping -c3 10.0.30.20
sudo ip netns exec N2 ping -c3 10.0.30.21

sudo ip netns exec N3 ping -c3 10.0.10.20
# sudo ip netns exec N3 ping -c3 10.0.10.21
sudo ip netns exec N3 ping -c3 10.0.20.20
# sudo ip netns exec N3 ping -c3 10.0.20.21
# sudo ip netns exec N3 ping -c3 10.0.30.20
# sudo ip netns exec N3 ping -c3 10.0.30.21