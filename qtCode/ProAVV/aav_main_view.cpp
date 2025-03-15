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
#include"aav_networkthread.h"
MainView::MainView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainView)
{
    ui->setupUi(this);
    m_wid_upload_select=nullptr;
    /*
     暂时是提供两个窗口，一个是usrpage,一个是videolist,通过左边导航栏不同按钮可以
     切换不同的窗口，usrpage窗口就是用户窗口，提供用户各种信息，videolist窗口是视
     频列表窗口，程序启动就默认显示这个窗口。
    */
    //先向服务器请求files_info数据,将要显示这些数据到videocoverwidget中
    /*NetWorkManager net_manager{};
    QNetworkReply* reply=net_manager.http_get_files_info();*/


    m_userpage=new UsrPage(this);
    QWidget* video_page_wid=new QWidget(this);

    VideoList* video_list_wid=new VideoList(0,video_page_wid);
    m_search_video_lists=new VideoList(3,video_page_wid);


    m_ledit_search=new LineEditSearch(video_page_wid);
    QPushButton* btn_search=new QPushButton(video_page_wid);
    btn_search->setText("搜索");
    connect(btn_search,&QPushButton::clicked,this,&MainView::sloBtnSearch);

    QHBoxLayout* hlayout=new QHBoxLayout();
    hlayout->addWidget(m_ledit_search);
    hlayout->addWidget(btn_search);

    QPushButton* btn_audios=new QPushButton(this);
    QPushButton* btn_videos=new QPushButton(this);
    btn_audios->setText("音频");
    btn_videos->setText("视频");
    QHBoxLayout* hlayout_2=new QHBoxLayout();
    hlayout_2->addWidget(btn_audios);
    hlayout_2->addWidget(btn_videos);


    //这个stack_wid作为video_page_wid这里面的一个控件
    stack_video_page_wid=new QStackedWidget(video_page_wid);
    QVBoxLayout* vlayout=new QVBoxLayout(video_page_wid);
    vlayout->addLayout(hlayout);
    vlayout->addLayout(hlayout_2);
    vlayout->addWidget(stack_video_page_wid);
    video_page_wid->setLayout(vlayout);

    VideoList* video_list_home_audios=new VideoList(4,this);
    VideoList* video_list_home_videos=new VideoList(5,this);
    stack_video_page_wid->addWidget(video_list_wid);
    stack_video_page_wid->addWidget(m_search_video_lists);
    stack_video_page_wid->addWidget(video_list_home_audios);
    stack_video_page_wid->addWidget(video_list_home_videos);

    //默认情况下的按钮回应，之后还要考虑搜索情况下的回应

    connect(btn_audios,&QPushButton::clicked,this,[=](){
        qDebug()<<"clicked btn_audios----------------";
        NetWorkThread* thread=new NetWorkThread{this};
        stack_video_page_wid->setCurrentWidget(video_list_home_audios);
        video_list_home_audios->reload();
        thread->m_net_manager->http_get_random_audios(video_list_home_audios);
        connect(thread,&NetWorkThread::finished,this,[thread](){
            thread->deleteLater();
        });
        qDebug()<<"out clicked btn_audios----------------";
    });
    connect(btn_videos,&QPushButton::clicked,this,[=](){
        qDebug()<<"clicked btn_videos----------------";
        NetWorkThread* thread=new NetWorkThread{this};
        stack_video_page_wid->setCurrentWidget(video_list_home_videos);
        video_list_home_videos->reload();
        thread->m_net_manager->http_get_random_videos(video_list_home_videos);
        connect(thread,&NetWorkThread::finished,this,[thread](){
            thread->deleteLater();
        });
        qDebug()<<"out clicked btn_audios----------------";
    });

    //先向服务器请求files_info数据,将要显示这些数据到videocoverwidget中
    NetWorkThread* thread=new NetWorkThread(this);
    thread->m_net_manager->http_get_files_info(video_list_wid);
    //将files_info返回的信息传递出去
    /*connect(this,&MainView::sigFilesInfo,video_list_wid,&VideoList::sloShowFilesInfo);
    emit sigFilesInfo(reply);*/

    ui->stackw->addWidget(video_page_wid);
    ui->stackw->addWidget(m_userpage);
    //ui->stackw->addWidget(m_search_video_lists);



    connect(ui->btn_video_list,&QPushButton::clicked,ui->stackw,[=](){
        NetWorkThread* thread=new NetWorkThread(this);
        video_list_wid->reload();
        thread->m_net_manager->http_get_files_info(video_list_wid);
        //emit sigFilesInfo(reply);
        stack_video_page_wid->setCurrentWidget(video_list_wid);
        ui->stackw->setCurrentWidget(video_page_wid);
        connect(thread,&NetWorkThread::finished,this,[=](){
            thread->deleteLater();
        });
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
        if(!m_wid_upload_select)m_wid_upload_select=new UploadSelect();
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
    if(!m_wid_upload_select)m_wid_upload_select=new UploadSelect();
    m_wid_upload_select->show();
}

void MainView::sloBtnSearch()
{
    //先向服务器发送用户搜索日志插入请求
    QString search_key=m_ledit_search->text();
    if(search_key==""){
        return;
    }
    NetWorkThread* thread1=new NetWorkThread(this);
    thread1->m_net_manager->http_insert_search_log(search_key);
    connect(thread1,&NetWorkThread::finished,this,[thread1](){
        thread1->deleteLater();
    });
    //再返回根据关键字查找到的所有的视频信息

    //ui->stackw->setCurrentWidget(m_search_video_lists);
    NetWorkThread* thread2=new NetWorkThread(this);
    stack_video_page_wid->setCurrentWidget(m_search_video_lists);
    m_search_video_lists->reload();
    thread2->m_net_manager->http_get_search_video_lists_info(m_search_video_lists,search_key);
    connect(thread2,&NetWorkThread::finished,this,[thread2](){
        thread2->deleteLater();
    });
}
