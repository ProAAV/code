#include "aav_main_view.h"
#include "ui_aav_main_view.h"
#include<QWidget>
#include<QDebug>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include"aav_videolist.h"
#include"aav_videodisplay.h"
#include"aav_networkmanager.h"
#include"aav_userlogin.h"
#include"aav_usermanager.h"
MainView::MainView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainView)
{
    ui->setupUi(this);
    /*
     暂时是提供两个窗口，一个是usrpage,一个是videolist,通过左边导航栏不同按钮可以
     切换不同的窗口，usrpage窗口就是用户窗口，提供用户各种信息，videolist窗口是视
     频列表窗口，程序启动就默认显示这个窗口。
    */
    //先向服务器请求files_info数据,将要显示这些数据到videocoverwidget中
    NetWorkManager net_manager{};
    QNetworkReply* reply=net_manager.http_get_files_info();


    m_userpage=new UsrPage(this);
    VideoList* video_list_wid=new VideoList(this);
    QWidget* video_page_wid=new QWidget(this);

    QLineEdit* ledit_search=new QLineEdit(video_page_wid);
    QPushButton* btn_search=new QPushButton(video_page_wid);
    btn_search->setText("搜索");

    QHBoxLayout* hlayout=new QHBoxLayout();
    hlayout->addWidget(ledit_search);
    hlayout->addWidget(btn_search);
    QVBoxLayout* vlayout=new QVBoxLayout(video_page_wid);
    vlayout->addLayout(hlayout);
    vlayout->addWidget(video_list_wid);
    //将files_info返回的信息传递出去
    connect(this,&MainView::sigFilesInfo,video_list_wid,&VideoList::sloShowFilesInfo);
    emit sigFilesInfo(reply);

    ui->stackw->addWidget(video_page_wid);
    ui->stackw->addWidget(m_userpage);




    connect(ui->btn_video_list,&QPushButton::clicked,ui->stackw,[=](){
        ui->stackw->setCurrentWidget(video_page_wid);
    });

    connect(ui->btn_usr_page,&QPushButton::clicked,this,&MainView::sloBtnUserPageHandle);

    connect(ui->btn_upload,&QPushButton::clicked,this,[=](){
        if(UserManager::instance()->getUserName()==""){
            qDebug()<<"user is not login,can not upload files";
            m_login_and_regis_wid=new UserLoginAndRegis();
            m_login_and_regis_wid->show();
            connect(m_login_and_regis_wid->m_login_wid,&UserLogin::sigLoginSuccess,this,&MainView::sloUserLoginSuccess2);
            return;
        }
        m_wid_upload_select=new UploadSelect();
        m_wid_upload_select->show();
    });


}

MainView::~MainView()
{
    delete ui;
}

void MainView::sloBtnUserPageHandle()
{
    //用户点击usr_page页面时会先进行用户登录验证，如果用户验证信息过期或者没有验证则打开一个userlogin页面
    //要是用户验证信息未过期，直接显示userpage
    //暂时没有实现token功能
    if(UserManager::instance()->getUserName()!=""){
        qDebug()<<"enter sloBtnUserPageHandle";
        //m_userpage->setUserInfo();
        ui->stackw->setCurrentWidget(m_userpage);
        return;
    }

    m_login_and_regis_wid=new UserLoginAndRegis();
    m_login_and_regis_wid->show();
    connect(m_login_and_regis_wid->m_login_wid,&UserLogin::sigLoginSuccess,this,&MainView::sloUserLoginSuccess);

}

void MainView::sloUserLoginSuccess()
{
    m_login_and_regis_wid->close();
    m_userpage->setUserInfo();
    ui->stackw->setCurrentWidget(m_userpage);
}

void MainView::sloUserLoginSuccess2()
{
    m_login_and_regis_wid->close();
}
