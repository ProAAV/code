#include "aav_videocoverwidlistloop.h"
#include "ui_aav_videocoverwidlistloop.h"

VideoCoverWidListLoop::VideoCoverWidListLoop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoCoverWidListLoop)
{
    ui->setupUi(this);
    lab_cover_img=new QLabel(this);
    lab_cover_img->setText("pppppppppppppp");
}

VideoCoverWidListLoop::~VideoCoverWidListLoop()
{
    delete ui;
}
