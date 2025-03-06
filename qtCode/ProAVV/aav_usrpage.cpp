#include "aav_usrpage.h"
#include "ui_aav_usrpage.h"
#include"aav_userlogin.h"
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>

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
    QLabel* lab_username=new QLabel(m_user_info_wid);
    hlayout_user_info_wid->addWidget(lab_username);
    lab_username->setText("hcc");

    m_btn_videos=new QPushButton(this);
    m_btn_history=new QPushButton(this);

    m_video_lists_wid=new VideoList(m_stack_wid);
    m_video_lists_history_wid=new VideoList(m_stack_wid);
    m_stack_wid->addWidget(m_video_lists_wid);
    m_stack_wid->addWidget(m_video_lists_history_wid);

    connect(m_btn_videos,&QPushButton::clicked,this,[=](){
        //m_video_lists_wid->setUserVideoListsInfo();
        m_stack_wid->setCurrentWidget(m_video_lists_wid);
    });
    connect(m_btn_history,&QPushButton::clicked,this,[=](){
        //m_video_lists_history_wid->setUserHistoryVideoListsInfo();
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

void UsrPage::showEvent(QShowEvent *event)
{
    //显示出来后直接发送一次http请求得到user_video_lists_info的信息显示出来
    m_video_lists_wid->setUserVideoListsInfo();
    QWidget::showEvent(event);
}
