//
// Created by desperado on 2021/5/2.
//
#include <iostream>
#include <getopt.h>
#include <cstdio>
#include <vector>
#include "help.h"
#include "sniffer.h"

char proto[6],saddr[20] = {},daddr[20] ={};
int main(int argc, char *argv[]){
    char ch;
    int slen = 0;

    while((ch = getopt(argc, argv, "p:s:d:hv")) != -1){
        switch (ch) {
            case 'h':
                help();
                exit(0);
            case 'v':
                version();
                exit(0);
            case 'p':
                slen = strlen(optarg);
                if(slen > 4){
                    fprintf(stdout, "The protocol is error!\n");
                    return -1;
                }
                memcpy(proto, optarg, slen);
                proto[slen] = '\0';
                break;
            case 's':
                slen = strlen(optarg);
                if(slen > 15 || slen < 7){
                    fprintf(stdout, "The IP address is error!\n");
                    return -1;
                }
                memcpy(saddr, optarg, slen);
                saddr[slen] = '\0';
                break;
            case 'd':
                slen = strlen(optarg);
                if(slen > 15 || slen < 7){
                    fprintf(stdout, "The IP address is error!\n");
                    return -1;
                }
                memcpy(daddr, optarg, slen);
                daddr[slen] = '\0';
                break;
            case '?':
                fprintf(stdout, "unrecongized option: %c\n", ch);
                exit(-1);
        }
    }
    sniffer();
}