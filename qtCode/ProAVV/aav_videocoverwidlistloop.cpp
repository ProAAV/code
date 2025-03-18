#include "aav_videocoverwidlistloop.h"
#include "ui_aav_videocoverwidlistloop.h"
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QJsonValue>
#include"aav_networkthread.h"
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QMouseEvent>
VideoCoverWidListLoop::VideoCoverWidListLoop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoCoverWidListLoop)
{
    ui->setupUi(this);
    lab_cover_img=new QLabel(this);

    lab_intro=new QLabel(this);
    lab_intro->setText("dsfsdfd");
    lab_auth=new QLabel(this);
    lab_auth->setText("dsdfsdfsdfsdfsd");

    QHBoxLayout* hlayout=new QHBoxLayout(this);
    QVBoxLayout* vlayout=new QVBoxLayout();

    vlayout->addWidget(lab_intro);
    vlayout->addWidget(lab_auth);

    hlayout->addWidget(lab_cover_img);
    hlayout->addLayout(vlayout);

    this->setLayout(hlayout);

}

VideoCoverWidListLoop::~VideoCoverWidListLoop()
{
    delete ui;
}

void VideoCoverWidListLoop::showCover()
{
    lab_intro->setText(m_intro);
    lab_auth->setText(m_username);
    if(m_file_img_path=="null"){
        qDebug()<<"file_img_path is null";
        //如果图片是空，那么直接使用qt自定义的一张图片展示封面
        QPixmap pix_(":/aa7243c933094e26b927243ee7e2856f.png");
        lab_cover_img->setPixmap(pix_.scaled(lab_cover_img->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        lab_cover_img->setAlignment(Qt::AlignCenter);
        lab_cover_img->setScaledContents(true);
        return;
    }
    NetWorkThread* thread=new NetWorkThread{};
    thread->m_net_manager->http_get_img_cover(m_file_img_path,lab_cover_img);
    connect(thread,&QThread::finished,this,[thread](){
        thread->deleteLater();
    });
}

/**bool VideoCoverWidListLoop::eventFilter(QObject *obj, QEvent *event)
{
    //实现点击lab播放的效果
    qDebug()<<"enter list loop eventFilter";
    if(obj==this&&event->type()==QEvent::MouseButtonPress){
        qDebug()<<"click list loop";
        qDebug()<<"file_path:"<<m_file_path;
        emit sigPlay();
    }
}*/

void VideoCoverWidListLoop::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 在这里处理点击事件
        qDebug()<<"mousePressEvent clicked";
        emit sigPlay(); // 发射点击信号
    }
    //QWidget::mousePressEvent(event); // 调用父类的实现
}


