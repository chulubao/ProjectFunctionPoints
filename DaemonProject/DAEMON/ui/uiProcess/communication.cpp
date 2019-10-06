//
// Created by root on 19-8-30.
//

#include "communication.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

using namespace nsLogDebug;

namespace nsCommunication {
/***************************************************************************
 * socket  send  function   by  tcp
 * *************************************************************************/
#define     UINT       unsigned int
#define     ULONG      unsigned long
#define     ERROR_SUCCESS           0UL
#define     ERROR_FAILED            0xFFFFFFFF
#define     SOCKEt_MAX_LISTEN_COUNT 5
#define     free_mm(x) if (x) { free(x); x = NULL; }

char  recvMsg[RECV_MSG_LEN]={0};
string replyMsg;

static SOCKET_MAINREQ_INFO g_mainRcvLocalTcpMsg;


    static char *msgCreatePacket(MSG_HEAD *msgHead, const void *msgData) {

        if (NULL == msgHead || NULL == msgData) { return NULL; }

        unsigned int uiDataMsgLen = msgHead->msgDataLen;
        /*****************************************************************************
         * 动态内存分配并清零.
         *分配n个长度为size的连续空间，函数返回一个指向分配起始地址的指针；如果分配不成功，返回NULL.
         *****************************************************************************/
        char *packet = (char *) calloc(1, sizeof(MSG_HEAD) + uiDataMsgLen);
        if (NULL == packet) { return NULL; }

        memcpy(packet, (char *) msgHead, sizeof(MSG_HEAD));
        memcpy(packet + sizeof(MSG_HEAD), msgData, uiDataMsgLen);

        return packet;
    }


    static char *recvMsgHeader(int socket_fd)
    {
        char *msg_buf = NULL;
        int recv_len = 0;
        int data_len = 0;

        MSG_HEAD msg_head;
        memset(&msg_head, 0, sizeof(MSG_HEAD));
        recv_len = recv(socket_fd, &msg_head, sizeof(MSG_HEAD), MSG_PEEK);
        if(0 >= recv_len)
        {
            logdebug(LOG_ERR, LOG_PRINT_FILE,"peek msg faild %s\n", strerror(errno));
            goto end;
        }
        /* printf( "peek msg len %d\n", msg_head.uiMsgDataLen);*/

        data_len = sizeof(MSG_HEAD) + msg_head.msgDataLen;
        msg_buf = (char *)calloc(data_len + 1, sizeof(char));
        if (NULL == msg_buf)
        {
            logdebug(LOG_ERR, LOG_PRINT_FILE,"calloc msg faild %s\n", strerror(errno));
            goto end;
        }

        recv_len = recv(socket_fd, msg_buf, data_len, 0);

        if (0 >= recv_len)
        {
            free_mm(msg_buf);
            logdebug(LOG_ERR, LOG_PRINT_FILE,"real recv msg faild %s\n", strerror(errno));
        }

        end:
        return msg_buf;
    }

    ssize_t sendLocalTcpMsg(IN const char *pcSocketPath,
                                        IN const char *pcSendMsg,
                                        IN int uiMsgLen,
                                        IN int recv_timeout,
                                        OUT char **RecvMsg)
    {
        struct sockaddr_un stUnixAddr;
        int iSocket = -1;
        ssize_t iSendLen = -1;
        int ret = 0;

        if (NULL == pcSendMsg || NULL == pcSocketPath)
        {
            logdebug(LOG_ERR, LOG_PRINT_FILE,"参数错误:发送消息为空\n");
            return -1;
        }

        iSocket = socket(AF_UNIX, SOCK_STREAM, 0);

        if (0 > iSocket)
        {
            logdebug(LOG_ERR, LOG_PRINT_FILE,"创建本地SOCKET出错 error:%s", strerror(errno));
            return -1;
        }

        if (recv_timeout > 0)
        {
            struct timeval RecvTimeOut;
            RecvTimeOut.tv_sec = recv_timeout;
            RecvTimeOut.tv_usec = 0;
            /*接收时限recv_timeout*/
            setsockopt(iSocket, SOL_SOCKET, SO_RCVTIMEO, &RecvTimeOut, sizeof(struct timeval));
        }

        memset(&stUnixAddr, 0, sizeof(stUnixAddr));
        stUnixAddr.sun_family = AF_UNIX;
        memcpy(stUnixAddr.sun_path, pcSocketPath, sizeof(stUnixAddr.sun_path));

        ret = connect(iSocket, (struct sockaddr*)&stUnixAddr, sizeof(stUnixAddr));
        if(ret < 0)
        {
            close(iSocket);
            logdebug(LOG_ERR, LOG_PRINT_FILE, "connect %s faild %s\n", pcSocketPath, strerror(errno));
            return -1;
        }

        iSendLen = send(iSocket, pcSendMsg, uiMsgLen, 0);

        if (iSendLen < 0) {
            logdebug(LOG_ERR, "发送消息到 %s 出错 错误码 %d error:%s\n消息内容:%s\n", \
                 pcSocketPath, errno, strerror(errno), pcSendMsg);
            close(iSocket);
            return iSendLen;
        }
        if (recv_timeout > 0)
        {
            // 同步接收消息，接收成功，返回发送的字节数，RecvMsg带出接收结果
            char *recv = recvMsgHeader(iSocket);/*接收消息头*/
            if (recv)
            {
                recv=recv+ sizeof(MSG_HEAD);/*接收消息体*/

                memcpy(RecvMsg, &recv, sizeof(&recv));
            }
        }
        close(iSocket);

        return iSendLen;
    }

    void sendMsgByTcp(IN const char* localSocketPath,IN string sendMsg,IN int timeOut,OUT char **recvMsg)
    {
        char *recvMsgTmp=NULL;
        MSG_HEAD msgHead;
        memset(&msgHead, 0, sizeof(MSG_HEAD));
        strncpy(msgHead.msgFrom, MSG_SOURCE, sizeof(msgHead.msgFrom));
        strncpy(msgHead.msgTo, MSG_DESTINATION, sizeof(msgHead.msgTo));
        msgHead.parseType = XML_PARSE_TYPE;
        msgHead.msgDataLen = strlen(sendMsg.c_str())+1;
        int  packetLen =msgHead.msgDataLen+ sizeof(MSG_HEAD);


        char *msgPacket = msgCreatePacket(&msgHead, (void *) (sendMsg.c_str()));
        if (msgPacket == NULL) {
            logdebug(LOG_ERR, LOG_PRINT_FILE,"msg_packet is NULL");
            return ;
        }

        sendLocalTcpMsg(localSocketPath,msgPacket,packetLen,timeOut,&recvMsgTmp);
        memcpy(recvMsg, &recvMsgTmp, sizeof(&recvMsgTmp));
    }

/********************************************send-Msg-ByTcp-end******************************************************/

    static int socketChmodSocket(int iSocket) {
        if (fchmod(iSocket, 0666) < 0) {
            printf("Fchmod socket error!\n");
            return -1;
        }
        return 0;
    }


    static int socketBindLocalAddr(int iUnixLocalSocket, const char *pcLocalSocketPath) {
        struct sockaddr_un stUnixAddr;
        int iRet = -1;

        memset(&stUnixAddr, 0, sizeof(stUnixAddr));
        stUnixAddr.sun_family = AF_UNIX;
        memcpy(stUnixAddr.sun_path, pcLocalSocketPath, sizeof(stUnixAddr.sun_path));

        unlink(pcLocalSocketPath);

        iRet = bind(iUnixLocalSocket, (struct sockaddr *) &stUnixAddr, sizeof(stUnixAddr));

        return iRet;
    }


    static int socketCreateLocalTcpSocket(IN const char *pcLocalSocketPath) {
        int iUnixLocalSocket = -1;
        int iRet = -1;

        if (NULL == pcLocalSocketPath) {
            printf("Invalid local socket path!");

            return -1;
        }

        iUnixLocalSocket = socket(AF_UNIX, SOCK_STREAM, 0);
        if (0 > iUnixLocalSocket) {
            printf("Socket Create Error:%s!", strerror(errno));

            return -1;
        }

        if (remove(pcLocalSocketPath) == -1 && errno != ENOENT) {
            printf("remove %s error", pcLocalSocketPath);

            close(iUnixLocalSocket);
            return -1;
        }

        iRet = socketChmodSocket(iUnixLocalSocket);
        if (0 != iRet) {
            printf("Chmod Socket Error!");
            close(iUnixLocalSocket);

            return -1;
        }

        iRet = socketBindLocalAddr(iUnixLocalSocket, pcLocalSocketPath);
        if (0 != iRet) {
            printf("Socket bind address Error!");
            close(iUnixLocalSocket);

            return -1;
        }

        iRet = listen(iUnixLocalSocket, SOCKEt_MAX_LISTEN_COUNT);
        if (-1 == iRet) {
            printf("Listen Error:%s!", strerror(errno));
            (void) close(iUnixLocalSocket);

            return -1;
        }

        return iUnixLocalSocket;

    }

    static ULONG mainRequestPeekMsgHead(IN int iReadySocket, OUT MSG_HEAD *pstMsgHead)
    {
        int iRecvLen = 0;
        iRecvLen = recv(iReadySocket, pstMsgHead, sizeof(MSG_HEAD), MSG_PEEK);
        if (0 >= iRecvLen)
        {
            logdebug(LOG_ERR,LOG_PRINT_FILE,"recv from %d error:%s  line=%d  function=%s",\
            iReadySocket, strerror(errno), __LINE__,__FUNCTION__);

            return ERROR_FAILED;
        }

        return ERROR_SUCCESS;
    }

    static char *mainRequestRecvMsg(IN int iReadySocket, IN UINT uiMsgDataLen) {
        char *pcMsgBuf = NULL;
        int iRecvLen = 0;

        pcMsgBuf = (char *) malloc(uiMsgDataLen + sizeof(MSG_HEAD));
        if (NULL == pcMsgBuf) {
            return NULL;
        }

        memset(pcMsgBuf, 0, uiMsgDataLen + sizeof(MSG_HEAD));

        iRecvLen = recv(iReadySocket, pcMsgBuf, uiMsgDataLen + sizeof(MSG_HEAD), 0);

        if (0 >= iRecvLen) {
            logdebug(LOG_ERR,LOG_PRINT_FILE,"recv from %d error:%s", iReadySocket, strerror(errno));
            free(pcMsgBuf);

            return NULL;
        }

        return pcMsgBuf;
    }



    static ULONG recvMsgProcRequests(IN int iReadySocket) {
        /* 接收消息、
               解析msgType、解析maintype、判断是否是需要回复的消息、
               执行回调、进行回复
               查找session还原上下文、
            */
        ULONG ulErrCode = ERROR_SUCCESS;
        MSG_HEAD  stMsgHead;
        char *pcMsgBuf = NULL;
        char *msgbody = NULL;

        memset(&stMsgHead, 0, sizeof(stMsgHead));

        ulErrCode = mainRequestPeekMsgHead(iReadySocket, &stMsgHead);
        if (ERROR_SUCCESS != ulErrCode) {
            return ERROR_FAILED;
        }

        pcMsgBuf = mainRequestRecvMsg(iReadySocket, stMsgHead.msgDataLen);
        if (NULL == pcMsgBuf) {
            /* 对端挂断、连接出错等情况recv会返回0 此时应结束本次服务 进入下一次accept */
            return ERROR_FAILED;
        }


        msgbody = pcMsgBuf + sizeof(stMsgHead);

        /////////////////////////////////////////////////////////
        //std::cout << "msgbody:\n" << msgbody;
        /////////////////////////////////////////////////////////


        if (NULL != msgbody) {
            memset(recvMsg,0,sizeof(recvMsg));
            memcpy(&recvMsg,msgbody,strlen(msgbody));

            MSG_HEAD msgHead;
            memset(&msgHead, 0, sizeof(MSG_HEAD));
            strncpy(msgHead.msgFrom, MSG_SOURCE, sizeof(msgHead.msgFrom));
            strncpy(msgHead.msgTo, MSG_DESTINATION, sizeof(msgHead.msgTo));
            msgHead.parseType = XML_PARSE_TYPE;
            msgHead.msgDataLen = strlen(replyMsg.c_str())+1;
            int  packetLen =msgHead.msgDataLen+ sizeof(MSG_HEAD);


            char *msgPacket = msgCreatePacket(&msgHead, (void *) (replyMsg.c_str()));
            if (msgPacket == NULL) {
                logdebug(LOG_ERR, LOG_PRINT_FILE,"msg_packet is NULL");
                return ERROR_FAILED;
            }

            send(iReadySocket, msgPacket, packetLen, 0);

        }



        free(pcMsgBuf);

        return ulErrCode;
    }

    static ULONG mainRecvMsgProcRequests(IN int iSocket) {

        memset(&g_mainRcvLocalTcpMsg, 0, sizeof(g_mainRcvLocalTcpMsg));
        g_mainRcvLocalTcpMsg.iEpoll = -1;
        g_mainRcvLocalTcpMsg.iSocket = -1;
        pthread_rwlock_init(&g_mainRcvLocalTcpMsg.stRdWrLock, NULL);

        pthread_rwlock_wrlock(&g_mainRcvLocalTcpMsg.stRdWrLock);
        g_mainRcvLocalTcpMsg.iSocket = iSocket;
        pthread_rwlock_unlock(&g_mainRcvLocalTcpMsg.stRdWrLock);
        pthread_rwlock_destroy(&g_mainRcvLocalTcpMsg.stRdWrLock);

        /*send other message  interface*/
        ULONG iRet = ERROR_SUCCESS;
        while (ERROR_SUCCESS == iRet) {
            iRet = recvMsgProcRequests(iSocket);
        }

        return iRet;
    }

    void recvMsgSynSendMsgByTcp(IN const char *pcLocalSocketPath) {
        int iServerSocket = -1;
        struct sockaddr_un clt_addr;
        socklen_t len = sizeof(clt_addr);
        int iClientSocket = -1;
        unsigned long ulRet = ERROR_FAILED;

        iServerSocket = socketCreateLocalTcpSocket(pcLocalSocketPath);
        if (-1 == iServerSocket) {
            printf("Create Listen Socket Error!");
            return;
        }

        while (1) {
            iClientSocket = accept(iServerSocket, (struct sockaddr *) &clt_addr, &len);
            if (-1 != iClientSocket) {
                ulRet = mainRecvMsgProcRequests(iClientSocket);
                (void) close(iClientSocket);
                if (ERROR_FAILED == ulRet) { break; }
            }
        }

    }


}
