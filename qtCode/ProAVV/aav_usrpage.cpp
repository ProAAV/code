#include "aav_usrpage.h"
#include "ui_aav_usrpage.h"
#include"aav_userlogin.h"
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include"aav_networkmanager.h"
#include"aav_networkthread.h"
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QJsonValue>
UsrPage::UsrPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsrPage)
{
    ui->setupUi(this);
    /*
    用户界面展示类
    */
    //做一个stack控件，当用户点击不同的按钮时展示不同的音视频封面
    m_stack_wid=new QStackedWidget(this);

    m_user_info_wid=new QWidget(this);

    //暂时先这样设置m_user_info_wid的布局
    QHBoxLayout* hlayout_user_info_wid=new QHBoxLayout(m_user_info_wid);
    lab_username=new QLabel(m_user_info_wid);
    lab_usernickname=new QLabel(m_user_info_wid);
    m_btn_logout=new QPushButton(m_user_info_wid);
    m_btn_logout->setText("退出登录");
    QLabel* lab_tips_username=new QLabel(m_user_info_wid);
    QLabel* lab_tips_usernickname=new QLabel(m_user_info_wid);
    lab_tips_username->setText("账号:");
    lab_tips_usernickname->setText("昵称:");

    hlayout_user_info_wid->addWidget(lab_tips_username);
    hlayout_user_info_wid->addWidget(lab_username);
    hlayout_user_info_wid->addWidget(lab_tips_usernickname);
    hlayout_user_info_wid->addWidget(lab_usernickname);
    hlayout_user_info_wid->addWidget(m_btn_logout);



    m_btn_videos=new QPushButton(this);
    m_btn_history=new QPushButton(this);

    m_video_lists_wid=new VideoList(1,m_stack_wid);
    m_video_lists_history_wid=new VideoList(2,m_stack_wid);
    m_stack_wid->addWidget(m_video_lists_wid);
    m_stack_wid->addWidget(m_video_lists_history_wid);

    connect(m_btn_videos,&QPushButton::clicked,this,[=](){
        m_video_lists_wid->reload();

        m_video_lists_wid->setUserVideoListsInfo();
        qDebug()<<"111111111111111111111111111111111111:";
        m_stack_wid->setCurrentWidget(m_video_lists_wid);
    });
    connect(m_btn_history,&QPushButton::clicked,this,[=](){
        qDebug("clicked m_btn_history");
        m_video_lists_history_wid->reload();
        m_video_lists_history_wid->setUserHistoryVideoListsInfo();
        m_stack_wid->setCurrentWidget(m_video_lists_history_wid);
    });

    m_btn_videos->setText("我的上传");
    m_btn_history->setText("历史浏览");

    QHBoxLayout* hlayout=new QHBoxLayout();
    hlayout->addWidget(m_btn_videos);
    hlayout->addWidget(m_btn_history);

    QVBoxLayout* vlayout=new QVBoxLayout();
    vlayout->addWidget(m_user_info_wid);
    vlayout->addLayout(hlayout);
    vlayout->addWidget(m_stack_wid);

    this->setLayout(vlayout);



}

UsrPage::~UsrPage()
{
    delete ui;
}

void UsrPage::setUserInfo()
{
    /*NetWorkManager net_manager{};
    QNetworkReply* reply=net_manager.http_get_user_info();*/
    NetWorkThread* thread=new NetWorkThread();
    thread->start();
    thread->m_net_manager->http_get_user_info(lab_username,lab_usernickname);
    /*QByteArray byte_array=reply->readAll();
    qDebug()<<"hhhhhhhhhhhhh:"<<byte_array;
    QJsonDocument json_docm=QJsonDocument::fromJson(byte_array);
    if(json_docm.isNull()){
        qDebug()<<"json document is null";
        return;
    }
    QJsonObject obj_root=json_docm.object();
    QString status=obj_root.value("status").toString();
    QString username=obj_root.value("username").toString();
    QString nickname=obj_root.value("nickname").toString();
    if(status=="0"){
        qDebug()<<"user info get failed";
        return;
    }

    lab_username->setText(username);
    lab_username->setText(nickname);*/
}

void UsrPage::showEvent(QShowEvent *event)
{
    //显示出来后直接发送一次http请求得到user_video_lists_info的信息显示出来
    qDebug()<<"enter showEvent";
    QWidget::showEvent(event);
    m_video_lists_wid->reload();
    m_video_lists_wid->setUserVideoListsInfo();

}
