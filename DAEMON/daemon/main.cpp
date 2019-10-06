#include <iostream>
#include <string.h>
#include <stdio.h>
#include "daemon.h"
using namespace nsDaemon;
int main(int argc, char* argv[]) {
    processOption(argc, argv); 
    workProcessRun();    
    return 0;
}
