//
// Created by desperado on 2021/5/2.
//
#include "sniffer.h"
#define BUFFSIZE 1024

extern char proto[],saddr[],daddr[];
char address[20] = {};
char *real_proto;
void sniffer(){
    printf("start sniffing\n");
    printf("use ctrl+c to stop, log file will be saved in path ./log.txt\n");
    int rawsock;
    unsigned char buff[BUFFSIZE];
    int n;
    int count = 0, TCP_cnt = 0, UDP_cnt = 0, ICMP_cnt = 0, other_cnt = 0;

    rawsock = socket(PF_PACKET,SOCK_DGRAM, htons(ETH_P_IP));
    if(rawsock < 0){
        printf("raw socket error!\n");
        exit(1);
    }
    
    FILE* file = fopen("log.txt", "w");
    while(1){    
        n = recvfrom(rawsock,buff,BUFFSIZE,0,NULL,NULL);
        if(n<0){
            printf("receive error!\n");
            exit(1);
        }

        count++;
        struct ip *ip = (struct ip*)(buff);
        if(strlen(proto)){
            if(!strcmp(proto, "TCP")){
                if(ip->ip_p != IPPROTO_TCP)
                    continue;
                else
                    goto addr;
            }else if(!strcmp(proto, "UDP")){
                if(ip->ip_p != IPPROTO_UDP)
                    continue;
                else
                    goto addr;
            }else if(!strcmp(proto, "ICMP")){
                if(ip->ip_p != IPPROTO_ICMP)
                    continue;
                else
                    goto addr;
            }
        }

addr:
        if(strlen(saddr)){
            strcpy(address, inet_ntoa(ip->ip_src));
            if(strcmp(address, saddr) != 0)
                continue;
        }
        if(strlen(daddr)){
            strcpy(address, inet_ntoa(ip->ip_dst));
            if(strcmp(address, daddr) != 0)
                continue;
        }
        switch (ip->ip_p)
        {
            case IPPROTO_TCP:
                TCP_cnt++;
                real_proto = "TCP";
                break;
            case IPPROTO_ICMP:
                ICMP_cnt++;
                real_proto = "ICMP";
                break;
            case IPPROTO_UDP:
                UDP_cnt++;
                real_proto = "UDP";
                break;
            default:
                other_cnt++;
                real_proto = "other";
                break;
        }
        
        fprintf(file, "%5s    %15s","index", "ip_src");
        fprintf(file, "%15s    %5s    %5s\n", "ip_dest", "protocol", "len");
        fprintf(file, "%5d    %15s",count,inet_ntoa(ip->ip_src));
        fprintf(file, "%15s    %8s    %5d\n\n",inet_ntoa(ip->ip_dst),real_proto,ntohs(ip->ip_len));

        int i=0,j=0;
        for(i=0;i<n;i++){
            if(i!=0 && i%16==0){
                fprintf(file, "    ");
                for(j=i-16;j<i;j++){
                    if(buff[j]>=32&&buff[j]<=128)
                        fprintf(file, "%c", buff[j]);
                    else fprintf(file, ".");
                }
                fprintf(file,"\n");
            }
            if(i%16 == 0) fprintf(file, "%04x    ",i);            
            fprintf(file, "%02x",buff[i]);

            if(i == n-1){
                for(j = 0; j < 15-i%16; j++) fprintf(file, "  ");
                fprintf(file, "    ");
                for(j=i-i%16;j<=i;j++){
                    if(buff[j]>=32&&buff[j]<127)
                        fprintf(file, "%c", buff[j]);
                    else fprintf(file, ".");

                }

            }

        }

        fprintf(file, "\n\n==============================\n\n");
        printf("TCP:%4d,UDP:%4d,ICMP:%4d,OTHER:%4d\r",TCP_cnt, UDP_cnt, ICMP_cnt, other_cnt);
        fflush(file);
        fflush(stdout);
    }
    fclose(file);
}