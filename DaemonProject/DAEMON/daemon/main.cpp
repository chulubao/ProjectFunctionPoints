#include <iostream>
#include <string.h>
#include <stdio.h>
#include "daemon.h"
#include "../common/commoninfo.h"
using namespace nsDaemon;
using namespace nsCommonInfo;
int main(int argc, char* argv[]) {
    processOption(argc, argv); 
    workProcessRun();    
    return 0;
}
