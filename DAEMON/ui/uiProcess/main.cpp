#include "login.h"
#include <QApplication>
#include <QTextCodec>
#include <QDebug>
using namespace nsLogin;
#define  DEBUG  qDebug()<<__FILE__<<__FUNCTION__<<__LINE__<<":"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //设置编码格式为utf-8显示中文不乱码
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
    Login w;
    w.show();
    
    return a.exec();
}
