#include "swapinfo.h"

//linux head
#include <getopt.h>
#include <sched.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <sys/stat.h>

//c head
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//cpp head
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <vector>
#include <functional>
#include <algorithm>
#include <cassert>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

// #define unitTest
// #define debug

int getSwapInfo(SwapInfo &swapInfo)
{
    FILE *stream;
    char buf[100];
    bool flag;
    if((stream = fopen("/proc/meminfo", "r")) == NULL)
    {
        printf("open file failed!\n");
        return -1;
    }
    flag = true;
    while(fgets(buf, 1000, stream) != NULL)
    {
        sscanf(buf, "SwapTotal:%lu", &swapInfo.SwapTotal);
        sscanf(buf, "SwapFree:%lu", &swapInfo.SwapFree);
    }

    return 0;
}
