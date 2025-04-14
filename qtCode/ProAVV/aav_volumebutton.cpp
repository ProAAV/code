#include "aav_volumebutton.h"
#include<QDebug>
#include<QLabel>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QSpacerItem>

VolumeButton::VolumeButton(QWidget *parent):QPushButton (parent)
{
    //作为widgetVolume的父控件，VolumeButton可以通过信号与槽机制来控制通信

    m_tm=new QTimer(this);
    m_tm->setSingleShot(true);
    connect(m_tm,&QTimer::timeout,this,&VolumeButton::sloTimeOut);
    //用wid_vlm作为sli_vlm的背景板
    m_wid_vlm=new WidgetVlume(parent);
    m_sli_vlm=new QSlider(m_wid_vlm);
    connect(m_wid_vlm,&WidgetVlume::sigEnter,this,&VolumeButton::sloTimeOutStop);
    //将lab显示音量大小和sli_vlm放入wid_vlm中
    m_lab_vlm=new QLabel(m_wid_vlm);
    m_lab_vlm->setAlignment(Qt::AlignCenter);
    QVBoxLayout* vlayout=new QVBoxLayout();
    QHBoxLayout* hlayout_1=new QHBoxLayout();
    QHBoxLayout* hlayout_2=new QHBoxLayout();
    hlayout_1->addSpacing(50);
    hlayout_1->addWidget(m_lab_vlm);
    hlayout_1->addSpacing(50);

    hlayout_2->addSpacing(20);
    hlayout_2->addWidget(m_sli_vlm);
    hlayout_2->addSpacing(20);

    vlayout->addLayout(hlayout_1);
    vlayout->addLayout(hlayout_2);
    m_wid_vlm->setLayout(vlayout);
    vlayout->setStretchFactor(hlayout_1,1);
    vlayout->setStretchFactor(hlayout_2,5);

    //先隐藏
    m_wid_vlm->hide();
    //m_wid_vlm->setStyleSheet("QWidget{background-color:black}");



    m_sli_vlm->setRange(0,100);
    m_sli_vlm->setValue(50);

}

VolumeButton::~VolumeButton()
{
    delete m_sli_vlm;
}

QSlider *VolumeButton::getSlider()
{
    return m_sli_vlm;
}

QLabel *VolumeButton::getLabVlm()
{
    return m_lab_vlm;
}

void VolumeButton::enterEvent(QEvent *event)
{
    //实时计算wid_vlm的位置和大小
    int wid_width=this->width();
    //int max_height=parent->height();
    //int wid_height=200>max_height?max_height:200;
    int wid_height=200;
    m_wid_vlm->setFixedSize(wid_width,wid_height);


    //移动wid_vlm
    QPoint p=this->pos();
    int wid_x=p.x()+15;
    int wid_y=p.y()-m_wid_vlm->height();
    m_wid_vlm->move(wid_x,wid_y);

    m_wid_vlm->show();

    QPushButton::enterEvent(event);

}

void VolumeButton::leaveEvent(QEvent *event)
{
    m_tm->start(500);

    QPushButton::leaveEvent(event);
}

void VolumeButton::sloTimeOut()
{
    m_wid_vlm->hide();
}

void VolumeButton::sloTimeOutStop()
{
    m_tm->stop();
}
