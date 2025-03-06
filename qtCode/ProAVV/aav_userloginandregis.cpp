#include "aav_userloginandregis.h"
#include "ui_aav_userloginandregis.h"
#include<QPushButton>
#include<QHBoxLayout>

#include<QVBoxLayout>
UserLoginAndRegis::UserLoginAndRegis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserLoginAndRegis)
{
    ui->setupUi(this);
    //作为主要的垂直布局
    QVBoxLayout* vlayout=new QVBoxLayout(this);
    m_stack_widget=new QStackedWidget(this);
    //最下排做两个按钮，登录，注册
    QHBoxLayout* hlayout=new QHBoxLayout();
    QPushButton* btn_login=new QPushButton(this);
    QPushButton* btn_register=new QPushButton(this);
    btn_login->setText("登录");
    btn_register->setText("注册");
    hlayout->addWidget(btn_login);
    hlayout->addWidget(btn_register);

    vlayout->addWidget(m_stack_widget);
    vlayout->addLayout(hlayout);

    m_login_wid=new UserLogin(this);
    m_register_wid=new UserRegister(this);



    m_stack_widget->addWidget(m_login_wid);
    m_stack_widget->addWidget(m_register_wid);
    connect(btn_login,&QPushButton::clicked,this,&UserLoginAndRegis::sloHandleLoginBtn);

    connect(btn_register,&QPushButton::clicked,this,&UserLoginAndRegis::sloHandleRegisterBtn);


}
void UserLoginAndRegis::sloHandleLoginBtn(){
    if(m_stack_widget->currentWidget()==m_login_wid){
        //如果是在login页面，那么发送http请求
        m_login_wid->handleLogin();

    }else{
        m_stack_widget->setCurrentWidget(m_login_wid);
    }
}
void UserLoginAndRegis::sloHandleRegisterBtn(){
    if(m_stack_widget->currentWidget()==m_register_wid){
        m_register_wid->handleRegister();

    }else{
        m_stack_widget->setCurrentWidget(m_register_wid);
    }
}
UserLoginAndRegis::~UserLoginAndRegis()
{
    delete ui;
}
