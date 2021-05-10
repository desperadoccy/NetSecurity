//
// Created by desperado on 2021/5/2.
//

#include "scanner.h"

using namespace std;

struct timeval tout;
char buffer[800];
extern int IP_sum;
extern int sec, usec;
vector<string> ipPool;
pthread_t ntid;

typedef struct {
    in_addr_t addr;
    int tgt_port;
    int result;
} scan_args;

void split(char *strs)
{
    int cnt = 0;
    string ip, mac, type, Iface;
    if ("" == strs)
        return;

    char *p = strtok(strs, " ");
    while (p)
    {
        cnt++;
        string s = p; //分割得到的字符串转换为string类型
        switch (cnt)
        {
        case 1:
            ip = s;
            break;
        case 2:
            type = s;
            break;
        case 3:
            mac = s;
            break;
        case 5:
            Iface = s;
            break;
        }
        p = strtok(NULL, " ");
    }
    if (cnt > 4)
    {
        cout << ip << endl;
        ipPool.push_back(ip);
    }
}

void getArp()
{
    int cnt = 0;
    FILE *fp;
    fp = popen("arp -e", "r");
    fgets(buffer, sizeof(buffer), fp);
    while (fgets(buffer, sizeof(buffer), fp))
    {
        cnt++;
        if (cnt >= IP_sum)
        {
            printf("超出ip缓存上限，可能未扫描完全");
            break;
        }
        split(buffer);
    }
    pclose(fp);
}

void scan(const int start,const int end)
{
    cout << "start scanning ip address" << endl;
    getArp();
    if (start < 0 || end > 65536 || end < start)
    {
        cout << "port error" << endl;
        exit(0);
    }
    cout << "\n==================================" << endl;
    for (int i = 0; i < ipPool.size(); i++)
    {
        printf("\nstart scanning ip: %s\n", ipPool[i].c_str());
        int j, k;
        scan_args args_list[1000];
        pthread_t pth_list[1000];
        struct in_addr i_addr;
        i_addr.s_addr = inet_addr(ipPool[i].c_str());
        for (j = start/1000; j < end/1000; j++)
        {
            for (k = 0; k < 1000; k++)
            {
                args_list[k].addr = inet_addr(ipPool[i].c_str());
                args_list[k].tgt_port = j * 1000 + (k + 1);
                pthread_create(pth_list + k, NULL, connect_scan, args_list + k);
            }
            for (k = 0; k < 1000; k++)
            {
                pthread_join(pth_list[k], NULL);
                if (args_list[k].result)
                    printf("\rport %d is open\n", args_list[k].tgt_port);
            }
        }
        for (k = start/1000 == end /1000 ? start%1000 : 0; k <= end%1000; k++)
        {
            args_list[k].addr = inet_addr(ipPool[i].c_str());
            args_list[k].tgt_port = end/1000 * 1000 + k ;
            pthread_create(pth_list + k, NULL, connect_scan, args_list + k);
        }
        
        for (k = start/1000 == end /1000 ? start%1000 : 0; k <= end%1000; k++)
        {
            pthread_join(pth_list[k], NULL);
            if (args_list[k].result)
                printf("\rport %d is open\n", args_list[k].tgt_port);
        }
    }
}

void *connect_scan(void *args)
{
    scan_args *p = (scan_args *) args;
    p->result = scan_port(p->addr, p->tgt_port);
    return NULL;
}


int scan_port(in_addr_t addr, int port)
{
    tout.tv_sec = sec;
    tout.tv_usec = usec;
    int fd, res, valopt, retflag = 1;
    long arg;
    unsigned int temp;
    fd_set set;
    struct sockaddr_in tgt_addr;
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    tgt_addr.sin_addr.s_addr = addr;
    tgt_addr.sin_family = AF_INET;
    tgt_addr.sin_port = htons(port);

    arg = fcntl(fd, F_GETFL, NULL);
    arg |= O_NONBLOCK;
    fcntl(fd, F_SETFL, arg);

    // Trying to connect with timeout
    res = connect(fd, (struct sockaddr *)&tgt_addr, sizeof(tgt_addr));
    if (res < 0)
    {
        if (errno == EINPROGRESS)
        {
            //fprintf(stderr, "EINPROGRESS in connect() - selecting\n");
            while (1)
            {
                FD_ZERO(&set);
                FD_SET(fd, &set);
                res = select(fd + 1, NULL, &set, NULL, &tout);
                if (res < 0 && errno != EINTR)
                {
                    //fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
                    retflag = 0;
                    goto ret;
                }
                else if (res > 0)
                {
                    // Socket selected for write
                    temp = sizeof(int);
                    if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (void *)(&valopt), &temp) < 0)
                    {
                        //fprintf(stderr, "Error in getsockopt() %d - %s\n", errno, strerror(errno));
                        retflag = 0;
                        goto ret;
                    }
                    // Check the value returned...
                    if (valopt)
                    {
                        //fprintf(stderr, "Error in delayed connection() %d - %s\n", valopt, strerror(valopt));
                        retflag = 0;
                        goto ret;
                    }
                    break;
                }
                else
                {
                    //fprintf(stderr, "Timeout in select() - Cancelling!\n");
                    retflag = 0;
                    goto ret;
                }
            }
        }
        else
        {
            //fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
            retflag = 0;
            goto ret;
        }
    }
ret:
    close(fd);
    return retflag;
}
