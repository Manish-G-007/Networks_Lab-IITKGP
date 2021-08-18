#!/bin/bash

sudo ip netns add ns1
sudo ip netns add ns2
sudo ip link add veth0 type veth peer name veth1
sudo ip link set veth0 netns ns1
sudo ip link set veth1 netns ns2
sudo ip netns exec ns1 ip addr add 10.1.1.0/24 dev veth0
sudo ip netns exec ns2 ip addr add 10.1.2.0/24 dev veth1
sudo ip netns exec ns1 ip link set dev veth0 up
sudo ip netns exec ns2 ip link set dev veth1 up
sudo ip netns exec ns1 ip route add default via 10.1.1.0
sudo ip netns exec ns2 ip route add default via 10.1.2.0
sudo ip -n ns1 route add 10.1.2.0/24 dev veth0
sudo ip -n ns2 route add 10.1.1.0/24 dev veth1