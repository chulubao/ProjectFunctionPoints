#include "login.h"
#include "ui_login.h"
#include "common.h"
#include "xmltool.h"
#include <QDesktopWidget>
#include <QFileInfo>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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

    connect(this,SIGNAL(isSuccess(bool)),this,SLOT(on_isSuccess_recved(bool)));

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

void   Login::on_isSuccess_recved(bool flag){
    DEBUG<<flag;
    logdebug(LOG_ERR,LOG_PRINT_UI_FILE,"file:%s,line:%d: flag:\n %d",__FILE__,__LINE__,flag);
}

void  Login::sendByTcp() {
    QString userRoleText=ui->roleComboBox->currentText();
    QString  userNameText=ui->userNameComboBox->currentText();
    QString passwordText=ui->pwdLineEdit->text();
    QString sendMsg;
    createXml(QString::fromUtf8("uiAccount"),QString::fromUtf8("daemonAccount"),QString::fromUtf8("Account"),\
           QString::fromUtf8("Account") ,QString::fromUtf8("1"),userRoleText,userNameText,passwordText,sendMsg );
    char *recvmsg = NULL;
    int timeOut = 10;
    sendMsgByTcp(LOCAL_SOCKET_PATH, sendMsg.toStdString(), timeOut, &recvmsg);
    DEBUG<<"sendMsg:["<<sendMsg<<"]";
    DEBUG<< "recvmsg:"<<QString(recvmsg);
    logdebug(LOG_ERR,LOG_PRINT_UI_FILE,"file:%s,line:%d: sendMsg:\n %s",__FILE__,__LINE__,sendMsg.toStdString().data());
    logdebug(LOG_ERR,LOG_PRINT_UI_FILE,"file:%s,line:%d: recvmsg:\n %s",__FILE__,__LINE__,recvmsg);
    if( parseXmlMsg(QString(recvmsg)))
    {
        emit isSuccess(true);
    }

}
void Login::on_loginConfirm_clicked()
{
    sendByTcp();
}

}


