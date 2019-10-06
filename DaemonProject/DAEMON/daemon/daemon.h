
#ifndef DAEMON_DAEMON_H
#define DAEMON_DAEMON_H


namespace nsDaemon{

extern void   workProcessRun();
extern void writeFile();
extern void processOption(int argc, char* argv[]);
}

#endif //DAEMON_DAEMON_H
