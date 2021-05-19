//
// Created by desperado on 2021/4/26.
//
#include "help.h"
#include "iostream"

using namespace std;

void help() {
    cout << "usage: scanner [-h] [-v] [-s port] [-e port] [-t time]" << endl;
    printf("-h\tprint available commands\n");
    printf("-v\tshow version\n");
    printf("-s\tstart port, default 1\n");
    printf("-e\tend port, default 65535\n");
    printf("-t\tset timeout, default 1000ms\n");
}

void version() {
    cout << "scanner version \"1.5\"" <<endl;
    cout << "copyright@desperadoccy" <<endl;
}