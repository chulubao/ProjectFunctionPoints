//
// Created by root on 19-8-30.
//

#ifndef COMMUNICATION_COMMUNICATION_H
#define COMMUNICATION_COMMUNICATION_H
#include "socketpath.h"
#include <iostream>
using  namespace std;


/*对外socket路径接口*/
#define  LOCAL_SOCKET_PATH    LOCAL_ACCOUNT_SOCKET
namespace nsCommunication {



/*对外socket接口:XML<From>*/
#define   MSG_SOURCE    "tdasvc.ui"
/*对外socket接口:XML<To>*/
#define   MSG_DESTINATION    "tdasvc.modifywhitelist"

/*---------------------------------------*/

#define   RECV_MSG_LEN    1024
#define   REPLY_MSG_LEN  1024
#define   LEN128   128

#define   IN
#define   OUT

    typedef enum {
        XML_PARSE_TYPE = 0,                /* 解析类型:XML格式 */
        CHAR_PARSE_TYPE,                   /* 解析类型:CHAR格式 */
        INT_PARSE_TYPE,                    /* 解析类型:INT格式 */
        STRUCT_PARSE_TYPE_,                /* 解析类型:结构体格式 */
        INVALID_PARSE_TYPE
    } MSG_PARSE_TYPE;

    typedef struct {
        char msgFrom[LEN128];             /* 消息来源 */
        char msgTo[LEN128];               /* 消息目的 */
        MSG_PARSE_TYPE parseType;        /* 消息解析类型 */
        unsigned int msgDataLen;          /* 消息体长度 */
        unsigned int msgTypeEX;           /* 拓展消息类型 */
        unsigned int sessionID;           /* 会话ID (异步上下文ID) */
    } MSG_HEAD;

    typedef struct{
        int iEpoll;
        int iSocket;
        pthread_rwlock_t stRdWrLock;
    } SOCKET_MAINREQ_INFO;

    extern  string replyMsg;
    extern  char  recvMsg[RECV_MSG_LEN];
    extern  void sendMsgByTcp(IN const char* localSocketPath,IN string sendMsg,IN int timeOut,OUT char **recvMsg);
    extern  void recvMsgSynSendMsgByTcp(IN const char *pcLocalSocketPath);
}

#endif //COMMUNICATION_COMMUNICATION_H
