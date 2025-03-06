#include "aav_userlogin.h"
#include "ui_aav_userlogin.h"
#include<QLabel>
#include<QLineEdit>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include"aav_networkmanager.h"
#include<QPushButton>
#include"aav_usermanager.h"
UserLogin::UserLogin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserLogin)
{
    ui->setupUi(this);

    //设计用户登录页面
    QLabel* lab_account=new QLabel(this);
    QLabel* lab_password=new QLabel(this);
    lab_account->setText("账号:");
    lab_password->setText("密码:");
    m_ledit_account=new QLineEdit(this);
    m_ledit_password=new QLineEdit(this);

    QHBoxLayout* hlayout_1=new QHBoxLayout();
    QHBoxLayout* hlayout_2=new QHBoxLayout();

    QVBoxLayout* vlayout=new QVBoxLayout();
    hlayout_1->addWidget(lab_account);
    hlayout_1->addWidget(m_ledit_account);
    hlayout_2->addWidget(lab_password);
    hlayout_2->addWidget(m_ledit_password);


    vlayout->addLayout(hlayout_1);
    vlayout->addLayout(hlayout_2);


    this->setLayout(vlayout);


}

UserLogin::~UserLogin()
{
    delete ui;
}

void UserLogin::handleLogin()
{
    NetWorkManager net_manager{};
    QString account=m_ledit_account->text();
    QString password=m_ledit_password->text();
    if(account==""||password==""){
        qDebug()<<"account or password is null";
        return;
    }
    if(net_manager.http_login(account,password)){
        qDebug()<<"http_login success";
        //登录成功即可设置用户名
        UserManager::instance()->setUserName(account);
        emit sigLoginSuccess();
    }
    else{
        qDebug()<<"login failed";
        emit sigLoginFailed();
    }
}
