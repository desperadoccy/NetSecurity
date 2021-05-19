//
// Created by desperado on 2021/5/2.
//

#ifndef SCANNER_SCANNER_H
#define SCANNER_SCANNER_H
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <vector>
#include "scanner.h"

void getArp();
void scan(int start, int end);
void *scanPort(int index, int port);
void *connect_scan(void *args);
int scan_port(in_addr_t addr, int port);
bool checkIPandPort(const in_addr_t ip, const int port);
#endif //SCANNER_SCANNER_H
