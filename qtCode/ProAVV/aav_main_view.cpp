#include "aav_main_view.h"
#include "ui_aav_main_view.h"
#include<QWidget>
#include<QDebug>
#include"aav_usrpage.h"
#include"aav_videolist.h"
#include"aav_videodisplay.h"
#include"aav_networkmanager.h"
#include"aav_userlogin.h"
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


    UsrPage* w1=new UsrPage(this);
    VideoList* w2=new VideoList(this);
    //将files_info返回的信息传递出去
    connect(this,&MainView::sigFilesInfo,w2,&VideoList::sloShowFilesInfo);
    emit sigFilesInfo(reply);

    ui->stackw->addWidget(w2);
    ui->stackw->addWidget(w1);



    /*connect(ui->btn_usr_page,&QPushButton::clicked,ui->stackw,[=](){

        ui->stackw->setCurrentWidget(w1);
    });*/
    connect(ui->btn_video_list,&QPushButton::clicked,ui->stackw,[=](){
        ui->stackw->setCurrentWidget(w2);
    });

    connect(ui->btn_usr_page,&QPushButton::clicked,this,&MainView::sloBtnUserPageHandle);
    /*VideoDisplay* vdis=new VideoDisplay();

    connect(ui->btn_video_disp,&QPushButton::clicked,ui->stackw,[=](){
        vdis->show();


    });*/

    connect(ui->btn_upload,&QPushButton::clicked,this,[=](){
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
    UserLogin* ulogin=new UserLogin();
    connect(ulogin,&UserLogin::sigLoginSuccess,this,[=](){
        ulogin->show();
    });
    ulogin->show();
}
