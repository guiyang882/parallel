#pragma once

#include <iostream>
#include <vector>

using namespace std;

enum TASKSTATUS {
    TASKINIT,
    TASKRUNNING,
    TASKCOMPELETE,
    TASKERROR,
    ARGERROR
};

typedef struct _TaskPackStruct {
    void* input;
    void* output;
}TaskPackStruct;