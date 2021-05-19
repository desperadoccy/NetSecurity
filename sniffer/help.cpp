//
// Created by desperado on 2021/4/26.
//
#include "help.h"
#include "iostream"

using namespace std;

void help() {
    cout << "usage: sniffer [-h] [-v] [-p protocol] [-s source_ip_address] [-d destination_ip_address]" << endl;
    printf("-h\tprint available commands\n");
    printf("-v\tshow version\n");
    printf("-p\tset protocol, default all\n");
    printf("-s\tsource_ip_address, default none\n");
    printf("-e\tdestination_ip_address, default none\n");
}

void version() {
    cout << "scanner version \"1.2\"" <<endl;
    cout << "copyright@desperadoccy" <<endl;
}