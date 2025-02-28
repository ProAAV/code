#include "aav_videocoverwidget.h"
#include "ui_aav_videocoverwidget.h"
#include"aav_networkmanager.h"
#include<QByteArray>
#include<QPixmap>
#include"aav_videodisplay.h"
VideoCoverWidget::VideoCoverWidget(VideoDisplay* vdis,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoCoverWidget)
{
    ui->setupUi(this);
    //这是视频封面类，主要是展示视频封面，包含视频封面图，视频总时长，视频上传时间，视频简介，视频上传作者。
    m_vdis=vdis;


    m_lab_img=new QLabel(this);
    m_lab_duration=new QLabel(this);
    m_lab_upload_date=new QLabel(this);
    m_lab_intro=new QLabel(this);


    //将lab_img的时间过滤器装到VideoCoverWidget上
    m_lab_img->installEventFilter(this);

    m_hlayout=new QHBoxLayout();
    m_vlayout=new QVBoxLayout();


    m_hlayout->addWidget(m_lab_upload_date);
    m_hlayout->addWidget(m_lab_duration);

    m_vlayout->addWidget(m_lab_img);
    m_vlayout->addLayout(m_hlayout);
    m_vlayout->addWidget(m_lab_intro);

    m_vlayout->setStretchFactor(m_lab_img,5);
    m_vlayout->setStretchFactor(m_hlayout,2);
    m_vlayout->setStretchFactor(m_lab_intro,2);


    m_lab_duration->setText("17:23");
    m_lab_upload_date->setText("2025-2-1");
    m_lab_intro->setText("hhhhhhhhhhhhhh");
    m_lab_img->setFixedSize(200,200);

    this->setLayout(m_vlayout);
}

VideoCoverWidget::~VideoCoverWidget()
{
    delete ui;
}

bool VideoCoverWidget::eventFilter(QObject *obj, QEvent *event)
{
    //实现点击lab播放的效果
    if(obj==m_lab_img&&event->type()==QEvent::MouseButtonPress){
        qDebug()<<"clicked and show";
        qDebug()<<"file_path:"<<m_file_path;
        m_vdis->setVideoFilePath(m_file_path);

        m_vdis->show();
        m_vdis->play();
    }
}

void VideoCoverWidget::sloRequestImg(QString& file_img_path)
{
    NetWorkManager net_manager{};
    qDebug()<<"file_img_path:"<<file_img_path;
    if(file_img_path==""){
        qDebug()<<"file_img_path is null";
        return;
    }
    QNetworkReply* reply=net_manager.http_get_img_cover(file_img_path);
    QByteArray byte_arr=reply->readAll();
    QPixmap pix;
    if(!pix.loadFromData(byte_arr)){
        qDebug()<<"loadFromData is failed";
        return;
    }
    m_lab_img->setPixmap(pix.scaled(m_lab_img->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            m_lab_img->setAlignment(Qt::AlignCenter);
            m_lab_img->setScaledContents(true);
    reply->deleteLater();

}
