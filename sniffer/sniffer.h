//
// Created by desperado on 2021/5/2.
//
#ifndef SNIFFER_SNIFFER_H
#define SNIFFER_SNIFFER_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/ip_icmp.h>
#include<netinet/tcp.h>
#include<netinet/udp.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<linux/if_ether.h>
#include<arpa/inet.h>
#include <unistd.h>
void sniffer();
#endif //SNIFFER_SNIFFER_H