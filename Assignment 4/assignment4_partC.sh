#!/bin/bash

# Colours for traceroute
GREEN=`tput setaf 2`
YELLOW=`tput setaf 3`
BLUE=`tput setaf 4`
RESET=`tput sgr0`

# Creating network namespaces
sudo ip netns add N1
sudo ip netns add N2
sudo ip netns add N3
sudo ip netns add N4
sudo ip netns add N5
sudo ip netns add N6

# Creating and adding veth pairs
sudo ip link add v1 type veth peer name v2
sudo ip link add v3 type veth peer name v4
sudo ip link add v5 type veth peer name v6
sudo ip link add v7 type veth peer name v8
sudo ip link add v9 type veth peer name v10
sudo ip link add v11 type veth peer name v12

sudo ip link set v1 netns N1
sudo ip link set v2 netns N2
sudo ip link set v3 netns N2
sudo ip link set v4 netns N3
sudo ip link set v5 netns N3
sudo ip link set v6 netns N4
sudo ip link set v7 netns N4
sudo ip link set v8 netns N5
sudo ip link set v9 netns N5
sudo ip link set v10 netns N6
sudo ip link set v11 netns N6
sudo ip link set v12 netns N1

# Assigning ip Address to veth pairs and activating them
sudo ip netns exec N1 ip addr add 10.0.60.21/24 dev v12
sudo ip netns exec N1 ip addr add 10.0.10.20/24 dev v1

sudo ip netns exec N2 ip addr add 10.0.10.21/24 dev v2
sudo ip netns exec N2 ip addr add 10.0.20.20/24 dev v3

sudo ip netns exec N3 ip addr add 10.0.20.21/24 dev v4
sudo ip netns exec N3 ip addr add 10.0.30.20/24 dev v5

sudo ip netns exec N4 ip addr add 10.0.30.21/24 dev v6
sudo ip netns exec N4 ip addr add 10.0.40.20/24 dev v7

sudo ip netns exec N5 ip addr add 10.0.40.21/24 dev v8
sudo ip netns exec N5 ip addr add 10.0.50.20/24 dev v9

sudo ip netns exec N6 ip addr add 10.0.50.21/24 dev v10
sudo ip netns exec N6 ip addr add 10.0.60.20/24 dev v11


sudo ip netns exec N1 ip link set dev v12 up
sudo ip netns exec N1 ip link set dev v1 up

sudo ip netns exec N2 ip link set dev v2 up
sudo ip netns exec N2 ip link set dev v3 up

sudo ip netns exec N3 ip link set dev v4 up
sudo ip netns exec N3 ip link set dev v5 up

sudo ip netns exec N4 ip link set dev v6 up
sudo ip netns exec N4 ip link set dev v7 up

sudo ip netns exec N5 ip link set dev v8 up
sudo ip netns exec N5 ip link set dev v9 up

sudo ip netns exec N6 ip link set dev v10 up
sudo ip netns exec N6 ip link set dev v11 up


sudo ip netns exec N1 ip link set dev lo up
sudo ip netns exec N2 ip link set dev lo up
sudo ip netns exec N3 ip link set dev lo up
sudo ip netns exec N4 ip link set dev lo up
sudo ip netns exec N5 ip link set dev lo up
sudo ip netns exec N6 ip link set dev lo up


# Enabling network forwarding
sudo ip netns exec N1 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec N2 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec N3 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec N4 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec N5 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec N6 sysctl -w net.ipv4.ip_forward=1

# Configuring ip route tables
sudo ip netns exec N1 ip route add 10.0.20.0/24 via 10.0.10.21 dev v1
sudo ip netns exec N1 ip route add 10.0.30.0/24 via 10.0.10.21 dev v1
sudo ip netns exec N1 ip route add 10.0.40.0/24 via 10.0.10.21 dev v1
sudo ip netns exec N1 ip route add 10.0.50.0/24 via 10.0.10.21 dev v1


sudo ip netns exec N2 ip route add 10.0.30.0/24 via 10.0.20.21 dev v3
sudo ip netns exec N2 ip route add 10.0.40.0/24 via 10.0.20.21 dev v3
sudo ip netns exec N2 ip route add 10.0.50.0/24 via 10.0.20.21 dev v3
sudo ip netns exec N2 ip route add 10.0.60.0/24 via 10.0.20.21 dev v3


sudo ip netns exec N3 ip route add 10.0.10.0/24 via 10.0.30.21 dev v5
sudo ip netns exec N3 ip route add 10.0.40.0/24 via 10.0.30.21 dev v5
sudo ip netns exec N3 ip route add 10.0.50.0/24 via 10.0.30.21 dev v5
sudo ip netns exec N3 ip route add 10.0.60.0/24 via 10.0.30.21 dev v5

sudo ip netns exec N4 ip route add 10.0.10.0/24 via 10.0.40.21 dev v7
sudo ip netns exec N4 ip route add 10.0.20.0/24 via 10.0.40.21 dev v7
sudo ip netns exec N4 ip route add 10.0.50.0/24 via 10.0.40.21 dev v7
sudo ip netns exec N4 ip route add 10.0.60.0/24 via 10.0.40.21 dev v7

sudo ip netns exec N5 ip route add 10.0.10.0/24 via 10.0.50.21 dev v9
sudo ip netns exec N5 ip route add 10.0.20.0/24 via 10.0.50.21 dev v9
sudo ip netns exec N5 ip route add 10.0.30.0/24 via 10.0.50.21 dev v9
sudo ip netns exec N5 ip route add 10.0.60.0/24 via 10.0.50.21 dev v9

sudo ip netns exec N6 ip route add 10.0.10.0/24 via 10.0.60.21 dev v11
sudo ip netns exec N6 ip route add 10.0.20.0/24 via 10.0.60.21 dev v11
sudo ip netns exec N6 ip route add 10.0.30.0/24 via 10.0.60.21 dev v11
sudo ip netns exec N6 ip route add 10.0.40.0/24 via 10.0.60.21 dev v11


# Pinging
# sudo ip netns exec N1 ping -c3 10.0.10.20
# sudo ip netns exec N1 ping -c3 10.0.10.21
# sudo ip netns exec N1 ping -c3 10.0.20.20
# sudo ip netns exec N1 ping -c3 10.0.20.21
# sudo ip netns exec N1 ping -c3 10.0.30.20
# sudo ip netns exec N1 ping -c3 10.0.30.21
# sudo ip netns exec N1 ping -c3 10.0.40.20
sudo ip netns exec N1 ping -c3 10.0.40.21
# sudo ip netns exec N1 ping -c3 10.0.50.20
# sudo ip netns exec N1 ping -c3 10.0.50.21
sudo ip netns exec N1 ping -c3 10.0.60.20
# sudo ip netns exec N1 ping -c3 10.0.60.21

# sudo ip netns exec N2 ping -c3 10.0.10.20
# sudo ip netns exec N2 ping -c3 10.0.10.21
# sudo ip netns exec N2 ping -c3 10.0.20.20
# sudo ip netns exec N2 ping -c3 10.0.20.21
# sudo ip netns exec N2 ping -c3 10.0.30.20
# sudo ip netns exec N2 ping -c3 10.0.30.21
# sudo ip netns exec N2 ping -c3 10.0.40.20
# sudo ip netns exec N2 ping -c3 10.0.40.21
# sudo ip netns exec N2 ping -c3 10.0.50.20
# sudo ip netns exec N2 ping -c3 10.0.50.21
# sudo ip netns exec N2 ping -c3 10.0.60.20
sudo ip netns exec N2 ping -c3 10.0.60.21

# sudo ip netns exec N3 ping -c3 10.0.10.20
# sudo ip netns exec N3 ping -c3 10.0.10.21
# sudo ip netns exec N3 ping -c3 10.0.20.20
# sudo ip netns exec N3 ping -c3 10.0.20.21
# sudo ip netns exec N3 ping -c3 10.0.30.20
# sudo ip netns exec N3 ping -c3 10.0.30.21
# sudo ip netns exec N3 ping -c3 10.0.40.20
# sudo ip netns exec N3 ping -c3 10.0.40.21
sudo ip netns exec N3 ping -c3 10.0.50.20
# sudo ip netns exec N3 ping -c3 10.0.50.21
# sudo ip netns exec N3 ping -c3 10.0.60.20
# sudo ip netns exec N3 ping -c3 10.0.60.21

# sudo ip netns exec N4 ping -c3 10.0.10.20
# sudo ip netns exec N4 ping -c3 10.0.10.21
sudo ip netns exec N4 ping -c3 10.0.20.20
# sudo ip netns exec N4 ping -c3 10.0.20.21
# sudo ip netns exec N4 ping -c3 10.0.30.20
# sudo ip netns exec N4 ping -c3 10.0.30.21
# sudo ip netns exec N4 ping -c3 10.0.40.20
# sudo ip netns exec N4 ping -c3 10.0.40.21
# sudo ip netns exec N4 ping -c3 10.0.50.20
# sudo ip netns exec N4 ping -c3 10.0.50.21
# sudo ip netns exec N4 ping -c3 10.0.60.20
# sudo ip netns exec N4 ping -c3 10.0.60.21

# sudo ip netns exec N5 ping -c3 10.0.10.20
# sudo ip netns exec N5 ping -c3 10.0.10.21
# sudo ip netns exec N5 ping -c3 10.0.20.20
# sudo ip netns exec N5 ping -c3 10.0.20.21
sudo ip netns exec N5 ping -c3 10.0.30.20
# sudo ip netns exec N5 ping -c3 10.0.30.21
# sudo ip netns exec N5 ping -c3 10.0.40.20
# sudo ip netns exec N5 ping -c3 10.0.40.21
# sudo ip netns exec N5 ping -c3 10.0.50.20
# sudo ip netns exec N5 ping -c3 10.0.50.21
# sudo ip netns exec N5 ping -c3 10.0.60.20
# sudo ip netns exec N5 ping -c3 10.0.60.21

# sudo ip netns exec N6 ping -c3 10.0.10.20
# sudo ip netns exec N6 ping -c3 10.0.10.21
# sudo ip netns exec N6 ping -c3 10.0.20.20
# sudo ip netns exec N6 ping -c3 10.0.20.21
# sudo ip netns exec N6 ping -c3 10.0.30.20
sudo ip netns exec N6 ping -c3 10.0.30.21
# sudo ip netns exec N6 ping -c3 10.0.40.20
# sudo ip netns exec N6 ping -c3 10.0.40.21
# sudo ip netns exec N6 ping -c3 10.0.50.20
# sudo ip netns exec N6 ping -c3 10.0.50.21
# sudo ip netns exec N6 ping -c3 10.0.60.20
# sudo ip netns exec N6 ping -c3 10.0.60.21

# Traceroute for N1 to N5,N3 to N5 and N3 to N1
echo -e "${BLUE}			HOPS FROM N1 to N5${RESET}"
sudo ip netns exec N1 traceroute -n 10.0.40.21
sleep 1.5
echo -e "${YELLOW}			HOPS FROM N3 to N5${RESET}"
sudo ip netns exec N3 traceroute -n 10.0.40.21
sleep 3
echo -e "${GREEN}			HOPS FROM N3 to N1${RESET}"
sudo ip netns exec N3 traceroute -n 10.0.60.21