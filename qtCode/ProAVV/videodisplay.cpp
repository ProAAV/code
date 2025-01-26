#include "videodisplay.h"
#include "ui_videodisplay.h"
#include<QMediaPlayer>
#include<QVideoWidget>
VideoDisplay::VideoDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoDisplay)
{
    ui->setupUi(this);
    /*
     视频播放类
    */
    ui->horizontalLayout->setStretch(0,5);
    ui->horizontalLayout->setStretch(1,2);
}

VideoDisplay::~VideoDisplay()
{
    delete ui;
}
