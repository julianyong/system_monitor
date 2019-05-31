#ifndef SWAPINFO_H
#define SWAPINFO_H


#include <unistd.h>
#include <string>
#include <vector>
using namespace std;

struct SwapInfo
{
    unsigned long SwapTotal,
        SwapFree;//in kB
};

int getSwapInfo(SwapInfo&swapInfo);

#endif // SWAPINFO_H
