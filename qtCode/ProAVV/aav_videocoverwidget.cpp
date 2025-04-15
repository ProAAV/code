#include "aav_videocoverwidget.h"
#include "ui_aav_videocoverwidget.h"
#include"aav_networkmanager.h"
#include"aav_networkthread.h"
#include<QByteArray>
#include<QPixmap>
#include"aav_videodisplay.h"
VideoCoverWidget::VideoCoverWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoCoverWidget)
{
    ui->setupUi(this);
    //这是视频封面类，主要是展示视频封面，包含视频封面图，视频总时长，视频上传时间，视频简介，视频上传作者。


    m_lab_img=new QLabel(this);
    //m_lab_img->setMinimumSize(400,400);
    //m_lab_img->setMaximumSize(1000,1000);
    //m_lab_img->setMaximumHeight(1000);
    //m_lab_img->setMinimumHeight(400);
    m_lab_duration=new QLabel(this);
    m_lab_duration->setAlignment(Qt::AlignRight);
    m_lab_upload_date=new QLabel(this);
    m_lab_intro=new QLabel(this);
    m_lab_username=new QLabel(this);

    //将lab_img的时间过滤器装到VideoCoverWidget上
    m_lab_img->installEventFilter(this);

    m_hlayout=new QHBoxLayout();
    m_vlayout=new QVBoxLayout();


    m_hlayout->addWidget(m_lab_username);
    m_hlayout->addWidget(m_lab_upload_date);
    m_hlayout->addWidget(m_lab_duration);

    m_vlayout->addWidget(m_lab_img);
    m_vlayout->addLayout(m_hlayout);
    m_vlayout->addWidget(m_lab_intro);

    m_vlayout->setStretchFactor(m_lab_img,15);
    m_vlayout->setStretchFactor(m_hlayout,1);
    m_vlayout->setStretchFactor(m_lab_intro,2);





    this->setLayout(m_vlayout);
}

VideoCoverWidget::~VideoCoverWidget()
{
    //delete m_vdis;
    delete ui;
}

bool VideoCoverWidget::eventFilter(QObject *obj, QEvent *event)
{
    //实现点击lab播放的效果
    if(obj==m_lab_img&&event->type()==QEvent::MouseButtonPress){
        qDebug()<<"clicked and show";
        qDebug()<<"file_path:"<<m_file_path;
        m_vdis=new VideoDisplay(m_file_md5,m_file_type,nullptr);
        m_vdis->resize(1200,800);
        connect(m_vdis,&VideoDisplay::sigClose,this,&VideoCoverWidget::sloCloseDisplayer);

        m_vdis->setVideoFilePath(m_file_path);
        qint64 pro=m_progress_data.toInt();
        qDebug()<<"pro :::::::"<<pro;

        m_vdis->show();
        m_vdis->play(m_file_path,m_file_md5);
        m_vdis->setVideoFileProgressData(pro);
    }


}



void VideoCoverWidget::sloRequestImg(QString& file_img_path)
{
    NetWorkManager net_manager{};
    qDebug()<<this<<"file_img_path:"<<file_img_path;
    //不仅仅时请求图片数据，顺便把文件数据一起设置了
    //先转换duration秒为HH:MM:SS形式
    bool ok;
    qDebug()<<"m_duration:"<<m_duration;
    QString integerPart = m_duration.split('.')[0];
    qint64 duration=integerPart.toInt(&ok);
    if(!ok){
        qDebug()<<"error toInt";
    }
    qDebug()<<"duration:"<<duration;
    QString dd=VideoDisplay::integraTime(duration,1);
    qDebug()<<"dd:"<<dd;
    m_lab_duration->setText(dd);
    qDebug()<<"m_date_time:"<<m_date_time;

    m_lab_upload_date->setText(m_date_time);
    m_lab_intro->setText(m_intro);
    m_lab_username->setText(m_username);


    if(file_img_path=="null"){
        qDebug()<<"file_img_path is null";
        //如果图片是空，那么直接使用qt自定义的一张图片展示封面
        QPixmap pix_(":/new.png");
        m_lab_img->resize(500,500);
        qDebug()<<"lab_size:"<<m_lab_img->size();
        m_lab_img->setPixmap(pix_.scaled(m_lab_img->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        m_lab_img->setAlignment(Qt::AlignCenter);
        m_lab_img->setScaledContents(true);
        return;

    }
    NetWorkThread* thread=new NetWorkThread{};
    thread->m_net_manager->http_get_img_cover(file_img_path,m_lab_img);
    /*QNetworkReply* reply=net_manager.http_get_img_cover(file_img_path);
    QByteArray byte_arr=reply->readAll();
    QPixmap pix;
    if(!pix.loadFromData(byte_arr)){
        qDebug()<<"loadFromData is failed";
        return;
    }
    m_lab_img->setPixmap(pix.scaled(m_lab_img->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_lab_img->setAlignment(Qt::AlignCenter);
    m_lab_img->setScaledContents(true);
    reply->deleteLater();*/

}

void VideoCoverWidget::sloCloseDisplayer()
{
    m_vdis->deleteLater();
    qDebug()<<"delete vdis";
}
