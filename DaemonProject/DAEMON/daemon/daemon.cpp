//
// Created by root on 19-9-14.
//
#include <unistd.h>
#include <stdlib.h> 
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h> //线程头文件  
#include "daemon.h"
#include "socketpath.h"
#include "../common/communication.h"
#include "../common/logdebug.h"

using namespace std;
using namespace nsCommunication;
using namespace nsLogDebug;

namespace nsDaemon{

#define  LOG_PRINT_PATH  "/root/deamon.txt"
#define  LOG_DEBUG_PATH  "/root/logdebug.txt"

#define PROCESS_UNIQUE_IDENTIFIER "/var/lock/daemon_singleton"  
#define	RET_FAILURE	 -1	/* Failing return status.   */
#define	RET_SUCCESS	  0	/* Successful return status.*/


	void delete_file(void)
	{
		remove(PROCESS_UNIQUE_IDENTIFIER);
	}

	bool  processSingletonRun(){
		/*open() O_CREAT, O_EXEC（如果文件存在则会报错，errno会被置位，可以通过errno判断*/
		int fd = open(PROCESS_UNIQUE_IDENTIFIER, O_RDWR | O_TRUNC | O_CREAT | O_EXCL, 0664);	
		if (fd < 0)	
		{		
			if (errno == EEXIST)		
			{	//logdebug(LOG_INFO,LOG_DEBUG_PATH,"%s\t %d\t %s\t tips=%s\n",__FUNCTION__,__LINE__,__FILE__,"进程已经存在，并不要重复执行");		
				return false;		
			}	
		}

		atexit(delete_file);/*注册进程清理函数:需要在结束时清理掉创建的文件*/	

		//logdebug(LOG_INFO,LOG_DEBUG_PATH,"%s\t %d\t %s\t tips=I am running...\n",__FUNCTION__,__LINE__,__FILE__);			

		return true;

	}



	void  recvByTcp(){

		string returnMsg ="<Message>\n"
			"<Header>\n"
			"<ID>63C5E4-4D88-EF4F-A449-97C4B54B11FD52</ID>\n"
			"<From>tdasvc.daemon</From>\n"
			"<To>tdasvc.ui</To>\n"
			"<Type>OverProcess</Type>\n"
			"<GenTime>2019-08-05 20:26:09.277</GenTime>\n"
			"<SendTime>2019-08-05 20:26:09.277</SendTime>\n"
			"</Header>\n"
			"<Body>\n"
			"\t<OverProcess>success</OverProcess>\n"
			"</Body>\n"
			"</Message>";

		nsCommunication::replyMsg = returnMsg;
		while(1) {
			recvMsgSynSendMsgByTcp(LOCAL_SOCKET_PATH);
			std::cout << "\n=========================================\n";
			printf("[%s]\n",recvMsg);
			logdebug(LOG_INFO, LOG_PRINT_FILE,"recv msg [%s]\n", nsCommunication::recvMsg);

		}

	}

	void processOption(int argc, char* argv[])
	{

		/*-d 前台调试模式*/
		if ((2 == argc) && (0 == strcmp(argv[1], "-d"))) {
			return ;
		}

		int ret= daemon(0,0);
		if(RET_FAILURE == ret){

			/*********************************
			 *exit(0)表示正常退出，
			 *exit(x)（x不为0）都表示异常退出.
			 *********************************/
			exit(EXIT_FAILURE);
		}
		return ;

	}

	/**
	 * int daemon(int nochdir, int noclose);
	 * 主要功能:
	 * daemon()函数主要用于希望脱离控制台，以守护进程形式在后台运行的程序。
	 * 参数作用:
	 * 当nochdir为0时，daemon将更改进程的根目录为root(“/”)。
	 * 当noclose为0是，daemon将进城的STDIN, STDOUT, STDERR都重定向到/dev/null。
	 * 返回值：
	 * deamon()调用了fork()，如果fork成功，那么父进程就调用_exit(2)退出，所以看到的错误信息 全部是子进程产生的。
	 * 如果成功函数返回0，否则返回-1并设置errno。
	 * */
	void  workProcessRun(){

		/*bool isRun=processSingletonRun();
		  if(false==isRun){return false;}*/
		recvByTcp();
		return ;
	}

}
