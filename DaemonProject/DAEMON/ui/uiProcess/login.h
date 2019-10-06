#ifndef LOGIN_H
#define LOGIN_H
#include "communication.h"
#include <QMouseEvent>
#include <QPoint>
#include <QWidget>

namespace Ui {
class Login;
}


namespace nsLogin {


class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = NULL);
    ~Login();
private slots:

    void on_loginCancel_clicked();
    void on_loginConfirm_clicked();

private:
    Ui::Login *ui;
    QPoint relativePos; // store relative position
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *envet);

    void  init();
    void  sendByTcp() ;
    void   generateXml();


};

}



#endif // LOGIN_H
