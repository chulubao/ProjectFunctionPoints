#include "../common/commoninfo.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <sys/types.h>
#include <linux/hdreg.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <openssl/md5.h>
#include <unistd.h>
#include <linux/if.h>
#include<time.h>

/**********************
 *依赖动态库
*LIBS += -lcrypto
**********************/

namespace  nsCommonInfo{
#define LENGTH_32 32
#define LENGTH_128 128
#define LENGTH_512 512
#define LENGTH_1024 1024

char net_adapter[LENGTH_32] = {0};


static int __get_mac(char *interface, char *buf, ssize_t len)
{

    int sockfd;
    struct ifreq if_req;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return -1;
    }

    memset(&if_req, 0, sizeof(if_req));
    strncpy(if_req.ifr_name, interface, sizeof(if_req.ifr_name));
    if (ioctl(sockfd, SIOCGIFHWADDR, &if_req) < 0) {
        close(sockfd);
        return -1;
    }

    close(sockfd);

    memcpy(buf, if_req.ifr_hwaddr.sa_data, len);

    return 0;
}

int get_mac_addr(char *interface, char *mac_str, ssize_t len)
{

    char data[7] = {0};

    if (!interface || ! mac_str)
        return -1;

    if (__get_mac(interface, data, sizeof(data)) != 0)
        return -1;

    snprintf(mac_str, len, "%02X:%02X:%02X:%02X:%02X:%02X",
            (unsigned char)data[0],
            (unsigned char)data[1],
            (unsigned char)data[2],
            (unsigned char)data[3],
            (unsigned char)data[4],
            (unsigned char)data[5]);

    return 0;
}


int get_hardid(const char *hdisk, char *hardid_str, ssize_t len)
{

    if (!hdisk || !hardid_str)
        return -1;

    int fd = 0;
    int ret = -1;
    static struct hd_driveid id;

    memset(hardid_str, 0, len);
    fd = open(hdisk, O_RDONLY);
    if (fd < 0) {


        return ret;
    }

    if (ioctl(fd, HDIO_GET_IDENTITY, &id) == 0) {
        strncpy(hardid_str, (const char*)id.serial_no, len);
        ret = 0;
    } else {
    }

    close(fd);
    return ret;
}



void get_md5_str(char *str, char *md5sum)
{

    unsigned char md[17] = {0};
    unsigned char md5_key[32] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcde"};

    MD5_CTX ctx;

    MD5_Init(&ctx);
    MD5_Update(&ctx, str, 512);
    MD5_Update(&ctx, (unsigned char *)md5_key, 32);
    MD5_Final(md, &ctx);

    memcpy(md5sum, (const char *)md, 16);
}


void get_uu_str(char *md5sum, char *hardid)
{
    unsigned char md[17] = {0};
    char target_s[64] = {0};
    char tmp[3] = {0};

    get_md5_str(hardid, (char *)md);

    int i = 0;
    for (i = 0; i < 16; i++) {
        snprintf(tmp, 3, "%02X", md[i]);
        switch (i) {
            case 3:
                strcat(target_s, "-");
                break;
            case 5:
                strcat(target_s, "-");
                break;
            case 7:
                strcat(target_s, "-");
                break;
            case 9:
                strcat(target_s, "-");
                break;
            default:
                break;
        }

        strcat(target_s, tmp);
    }

    strncpy(md5sum, target_s, 36);
}

/**
获取代理商ID
*/
void  getAgentID(char *pcAgentID)
{
    char hardid[LENGTH_512 + 1] = {0};
    char mac[LENGTH_32] = {0};

    if (NULL == pcAgentID)
    {
        return;
    }

    get_hardid("/dev/sda", hardid, sizeof(hardid));
    get_mac_addr(net_adapter, mac, sizeof(mac));

    strncat(hardid, mac, strlen(mac));

    get_uu_str(pcAgentID, hardid);

    return;
}


/**
获取当前时间
*/
char *get_current_date(char *str)
{
    if (!str)
        return NULL;

    time_t timer = time(NULL);
    struct tm *tblock;
    tblock = localtime(&timer);

    snprintf(str, 64, "%04d-%02d-%02d %02d:%02d:%02d", tblock->tm_year + 1900, tblock->tm_mon + 1,
                tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec);

    return str;
}

}
