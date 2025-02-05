#include "aav_videodisplay.h"
#include "ui_aav_videodisplay.h"
#include<QMediaPlayer>
#include<QVideoWidget>
#include<QLabel>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QSlider>
VideoDisplay::VideoDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoDisplay)
{
    ui->setupUi(this);
    /*
     视频播放类
    */
    QLabel* lab_video_backgrd=new QLabel(this);
    lab_video_backgrd->setText("ppppppp");
    QSlider* slider_video_process=new QSlider(Qt::Horizontal,this);

    QLabel* lab_video_pre=new QLabel(this);
    lab_video_pre->setText("ooooo");
    QLabel* lab_video_pause=new QLabel(this);
    lab_video_pause->setText("kkkkk");
    QLabel* lab_video_next=new QLabel(this);
    lab_video_next->setText("jjjjj");

    m_vlayout=new QVBoxLayout;
    m_hlayout=new QHBoxLayout;
    m_hlayout->addWidget(lab_video_pre);
    m_hlayout->addWidget(lab_video_pause);
    m_hlayout->addWidget(lab_video_next);

    m_vlayout->addWidget(lab_video_backgrd);
    m_vlayout->addWidget(slider_video_process);
    m_vlayout->addLayout(m_hlayout);

    ui->frame->setLayout(m_vlayout);



}

VideoDisplay::~VideoDisplay()
{
    delete m_hlayout;
    delete m_vlayout;
    delete ui;
}
