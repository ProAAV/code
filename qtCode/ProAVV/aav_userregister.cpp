#include "aav_userregister.h"
#include "ui_aav_userregister.h"
#include<QLabel>
#include"aav_networkmanager.h"
#include<QTimer>
UserRegister::UserRegister(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserRegister)
{
    ui->setupUi(this);
    //记得free
    success_tip=new ToTips();
    success_tip->resize(200,50);
    success_tip->hide();

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
    m_ledit_password->setEchoMode(QLineEdit::Password);

    m_ledit_nickname=new QLineEdit(this);
    m_ledit_repassword=new QLineEdit(this);
    m_ledit_repassword->setEchoMode(QLineEdit::Password);

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

        success_tip->getLab()->setStyleSheet("QLabel{background-color:red;}");
        success_tip->getLab()->setText("account or password or nickname or repassword is null");
        success_tip->show();
        QTimer* time=new QTimer(this);
        connect(time,&QTimer::timeout,this,[=](){
            success_tip->hide();
            time->deleteLater();
        });
        time->start(1000);
        qDebug()<<"account or password or nickname or repassword is null";
        return;
    }
    if(password!=repassword){
        success_tip->getLab()->setStyleSheet("QLabel{background-color:red;}");
        success_tip->getLab()->setAlignment(Qt::AlignCenter);
        success_tip->getLab()->setText("密码与重复输出密码不对");
        success_tip->show();
        QTimer* time=new QTimer(this);
        connect(time,&QTimer::timeout,this,[=](){
            success_tip->hide();
            time->deleteLater();
        });
        time->start(1000);
        qDebug()<<"密码与重复输出密码不对";
        return;
    }
    if(net_manager.http_register(account,password,nickname,repassword)){
        success_tip->getLab()->setStyleSheet("QLabel{background-color:green;color:white;font-size:20px;}");
        success_tip->getLab()->setText("注册成功");
        success_tip->getLab()->setAlignment(Qt::AlignCenter);
        success_tip->show();
        QTimer* time=new QTimer(this);
        connect(time,&QTimer::timeout,this,[=](){
            success_tip->hide();
            time->deleteLater();
        });
        time->start(1000);
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
