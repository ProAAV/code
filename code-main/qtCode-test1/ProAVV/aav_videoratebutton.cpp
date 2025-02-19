#include "aav_videoratebutton.h"
#include<QDebug>
#include<QLabel>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QSpacerItem>
Videoratebutton::Videoratebutton(QWidget *parent):QPushButton (parent)
{
    //作为widgetVolume的父控件，VolumeButton可以通过信号与槽机制来控制通信

    m_tm=new QTimer(this);
    m_tm->setSingleShot(true);
    connect(m_tm,&QTimer::timeout,this,&Videoratebutton::sloTimeOut);
    //用wid_vlm作为sli_vlm的背景板
    m_wid_rate=new WidgetVlume(parent);
    m_sli_rate=new QSlider(m_wid_rate);
    connect(m_wid_rate,&WidgetVlume::sigEnter,this,&Videoratebutton::sloTimeOutStop);
    //将lab显示音量大小和sli_vlm放入wid_vlm中

    /* m_slider_video_rate=new QSlider(Qt::Vertical);
     m_slider_video_rate->setRange(50,200);
     m_slider_video_rate->setValue(100);
      QLabel *speedLabel=new QLabel("1.0x",this);
     m_btn_rate->installEventFilter(this);*/

    m_lab_rate=new QLabel(m_wid_rate);
    m_lab_rate->setAlignment(Qt::AlignCenter);
    QVBoxLayout* vlayout=new QVBoxLayout();
    QHBoxLayout* hlayout_1=new QHBoxLayout();
    QHBoxLayout* hlayout_2=new QHBoxLayout();
    hlayout_1->addSpacing(50);
    hlayout_1->addWidget(m_lab_rate);
    hlayout_1->addSpacing(50);

    hlayout_2->addSpacing(20);
    hlayout_2->addWidget(m_sli_rate);
    hlayout_2->addSpacing(20);

    vlayout->addLayout(hlayout_1);
    vlayout->addLayout(hlayout_2);
    m_wid_rate->setLayout(vlayout);
    vlayout->setStretchFactor(hlayout_1,1);
    vlayout->setStretchFactor(hlayout_2,5);
    //先隐藏
    m_wid_rate->hide();
    m_wid_rate->setStyleSheet("QWidget{background-color:red}");



    m_sli_rate->setRange(50,200);
    m_sli_rate->setValue(100);

}

Videoratebutton::~Videoratebutton()
{
    delete m_sli_rate;
}

QSlider *Videoratebutton::getSlider()
{
    return m_sli_rate;
}

QLabel *Videoratebutton::getLabVlm()
{
    return m_lab_rate;
}

void Videoratebutton::enterEvent(QEvent *event)
{
    //实时计算wid_vlm的位置和大小
    int wid_width=this->width();
    //int max_height=parent->height();
    //int wid_height=200>max_height?max_height:200;
    int wid_height=200;
    m_wid_rate->setFixedSize(wid_width,wid_height);


    //移动wid_vlm
    QPoint p=this->pos();
    int wid_x=p.x()+15;
    int wid_y=p.y()-m_wid_rate->height();
    m_wid_rate->move(wid_x,wid_y);

    m_wid_rate->show();

    QPushButton::enterEvent(event);

}

void Videoratebutton::leaveEvent(QEvent *event)
{
    m_tm->start(500);

    QPushButton::leaveEvent(event);
}

void Videoratebutton::sloTimeOut()
{
    m_wid_rate->hide();
}

void Videoratebutton::sloTimeOutStop()
{
    m_tm->stop();
}

