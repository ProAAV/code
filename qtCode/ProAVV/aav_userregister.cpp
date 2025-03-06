#include "aav_userregister.h"
#include "ui_aav_userregister.h"
#include<QLabel>
#include"aav_networkmanager.h"
UserRegister::UserRegister(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserRegister)
{
    ui->setupUi(this);

    //设计用户注册页面
    QLabel* lab_account=new QLabel(this);
    QLabel* lab_password=new QLabel(this);
    QLabel* lab_nikename=new QLabel(this);
    QLabel* lab_repassword=new QLabel(this);

    lab_account->setText("账号:");
    lab_password->setText("密码:");
    lab_nikename->setText("昵称:");
    lab_repassword->setText("确认密码:");

    m_ledit_account=new QLineEdit(this);
    m_ledit_password=new QLineEdit(this);
    m_ledit_nickname=new QLineEdit(this);
    m_ledit_repassword=new QLineEdit(this);

    QHBoxLayout* hlayout_1=new QHBoxLayout();
    QHBoxLayout* hlayout_2=new QHBoxLayout();
    QHBoxLayout* hlayout_3=new QHBoxLayout();
    QHBoxLayout* hlayout_4=new QHBoxLayout();

    QVBoxLayout* vlayout=new QVBoxLayout();
    hlayout_1->addWidget(lab_account);
    hlayout_1->addWidget(m_ledit_account);
    hlayout_2->addWidget(lab_password);
    hlayout_2->addWidget(m_ledit_password);
    hlayout_3->addWidget(lab_nikename);
    hlayout_3->addWidget(m_ledit_nickname);
    hlayout_4->addWidget(lab_repassword);
    hlayout_4->addWidget(m_ledit_repassword);

    vlayout->addLayout(hlayout_3);
    vlayout->addLayout(hlayout_1);
    vlayout->addLayout(hlayout_2);
    vlayout->addLayout(hlayout_4);


    this->setLayout(vlayout);
}
void UserRegister::handleRegister()
{
    NetWorkManager net_manager{};
    QString account=m_ledit_account->text();
    QString password=m_ledit_password->text();
    QString nickname=m_ledit_nickname->text();
    QString repassword=m_ledit_repassword->text();
    if(account==""||password==""||nickname==""||repassword==""){
        qDebug()<<"account or password or nickname or repassword is null";
        return;
    }
    if(password!=repassword){
        qDebug()<<"密码与重复输出密码不对";
        return;
    }
    if(net_manager.http_register(account,password,nickname,repassword)){
        emit sigRegisterSuccess();
    }
    else{
        emit sigRegisterFailed();
    }
}
UserRegister::~UserRegister()
{
    delete ui;
}
