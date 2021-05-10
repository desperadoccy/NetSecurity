#include <iostream>
#include <getopt.h>
#include <cstdio>
#include <vector>
#include "help.h"
#include "scanner.h"

using namespace std;

extern vector<string> ipPool, macPool;
int IP_sum = 800;
int IPstart = 0, IPend = 65535;
int sec = 1, usec = 0;

int main(int argc, char *argv[]) {
    int oc;/*选项字符 */
    char ec;/*无效的选项字符*/
    while ((oc = getopt(argc, argv, "hvs:e:t:")) != -1) {
        switch (oc) {
            case 'h':
                help();
                return 0;
            case 'v':
                version();
                return 0;
            case 's':
                IPstart = atoi(optarg);
                break;
            case 'e':
                IPend = atoi(optarg);
                break;
            case 't':
                int time = atoi(optarg);
                sec = time / 1000;
                usec = time % 1000;
                break;
        }
    }
    scan(IPstart, IPend);
    return 0;
}
