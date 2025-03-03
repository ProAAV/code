#include "aav_userlogin.h"
#include "ui_aav_userlogin.h"
#include<QLabel>
#include<QLineEdit>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include"aav_networkmanager.h"
#include<QPushButton>
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
    QPushButton* btn_login=new QPushButton(this);
    btn_login->setText("登录");
    QPushButton* btn_register=new QPushButton(this);
    btn_register->setText("注册");
    QHBoxLayout* hlayout_1=new QHBoxLayout();
    QHBoxLayout* hlayout_2=new QHBoxLayout();
    QHBoxLayout* hlayout_3=new QHBoxLayout();
    QVBoxLayout* vlayout=new QVBoxLayout();
    hlayout_1->addWidget(lab_account);
    hlayout_1->addWidget(m_ledit_account);
    hlayout_2->addWidget(lab_password);
    hlayout_2->addWidget(m_ledit_password);
    hlayout_3->addWidget(btn_login);
    hlayout_3->addWidget(btn_register);

    vlayout->addLayout(hlayout_1);
    vlayout->addLayout(hlayout_2);
    vlayout->addLayout(hlayout_3);

    this->setLayout(vlayout);
    //将登录与注册按钮绑定处理函数
    connect(btn_login,&QPushButton::clicked,this,&UserLogin::sloHandleLogin);

}

UserLogin::~UserLogin()
{
    delete ui;
}

void UserLogin::sloHandleLogin()
{
    NetWorkManager net_manager{};
    QString account=m_ledit_account->text();
    QString password=m_ledit_password->text();
    if(account==""||password==""){
        qDebug()<<"account or password is null";
        return;
    }
    if(net_manager.http_login(account,password)){
        emit sigLoginSuccess();
    }
    else{
        emit sigLoginFailed();
    }
}
