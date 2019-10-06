#include "login.h"
#include "ui_login.h"
#include "communication.h"
#include "../../common/commoninfo.h"
#include <QDesktopWidget>
#include <QFileInfo>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <QDebug>

#define  DEBUG  qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<":"
using namespace std;
using namespace nsCommunication;
using namespace nsCommonInfo;
using namespace nsLogDebug;

namespace nsLogin {
Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    init();
}

Login::~Login()
{

    delete ui;
}


void Login::init(){
    ui->pwdLineEdit->setToolTip(QString::fromUtf8("8~16位,至少一个大写字母与小写字母!"));
    this->setFixedSize(400,300);
    this->setWindowTitle("login");
    this->setWindowIcon(QIcon(":/logo/image/logo_white.ico"));
    /*The login screen is displayed in the center.*/
    move(( QApplication::desktop()->width() - this->width())/2, ( QApplication::desktop()->height() - this->height())/2);
    /*Remove the maximum display function.*/
    this->setWindowFlags(Qt::CustomizeWindowHint);
    /*Qt设置密码密文*/
    ui->pwdLineEdit->setEchoMode(QLineEdit::Password);



}

void Login::mousePressEvent(QMouseEvent *event)
{
    relativePos=this->pos()-event->globalPos();
}


void Login::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()+ relativePos);
}

void Login::on_loginCancel_clicked()
{
    this->hide();
    this->close();
}

void   Login::generateXml(){

}

void  Login::sendByTcp() {

    string sendMsg = "<Message>\n"
                     "<Header>\n"
                     "<ID>63C5E4-4D88-EF4F-A449-97C4B54B11FD52</ID>\n"
                     "<From>tdasvc.ui</From>\n"
                     "<To>tdasvc.modifywhitelist</To>\n"
                     "<Type>Whitelist</Type>\n"
                     "<GenTime>2019-08-05 20:26:09.277</GenTime>\n"
                     "<SendTime>2019-08-05 20:26:09.277</SendTime>\n"
                     "</Header>\n"
                     "<Body>\n"
                     "\t<DeleteSingleItem>success</DeleteSingleItem>\n"
                     "\t<UserName>Admin</UserName>\n"
                     "\t<UserRole>0</UserRole>\n"
                     "</Body>\n"
                     "</Message>";

    char *recvmsg = NULL;
    int timeOut = 10;
    sendMsgByTcp(LOCAL_SOCKET_PATH, sendMsg, timeOut, &recvmsg);
    DEBUG<<QString(recvmsg);

}
void Login::on_loginConfirm_clicked()
{
    sendByTcp();
}

}


