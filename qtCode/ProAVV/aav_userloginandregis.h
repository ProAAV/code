#ifndef AAV_USERLOGINANDREGIS_H
#define AAV_USERLOGINANDREGIS_H

#include <QWidget>
#include<QStackedWidget>
#include"aav_userlogin.h"
#include"aav_userregister.h"
#include"aav_networkmanager.h"

namespace Ui {
class UserLoginAndRegis;
}

class UserLoginAndRegis : public QWidget
{
    Q_OBJECT

public:
    explicit UserLoginAndRegis(QWidget *parent = nullptr);
    ~UserLoginAndRegis();
    UserLogin* m_login_wid;
    UserRegister* m_register_wid;
private:
    Ui::UserLoginAndRegis *ui;
    QStackedWidget* m_stack_widget;

    //NetWorkManager m_net_manager;
private slots:
    void sloHandleLoginBtn();
    void sloHandleRegisterBtn();
};

#endif // AAV_USERLOGINANDREGIS_H
