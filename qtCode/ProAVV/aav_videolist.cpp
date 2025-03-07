#include "aav_videolist.h"
#include "ui_aav_videolist.h"
#include<QPushButton>
#include<QLineEdit>
#include<QStandardItemModel>
#include<QListView>
#include"aav_videocoverwidget.h"
#include<QWidget>
#include<QLabel>
#include<aav_networkmanager.h>
#include"aav_videodisplay.h"
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QJsonValue>
VideoList::VideoList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoList)
{
    ui->setupUi(this);
    /*
    视频列表展示类
    */

    //VideoDisplay* vdis_1=new VideoDisplay();
    m_widget_1=new VideoCoverWidget(this);

    //m_widget_1->setStyleSheet("QWidget{background-color: red;}");
    ui->gridLayout->addWidget(m_widget_1,0,0);

    //VideoDisplay* vdis_2=new VideoDisplay();
    m_widget_2=new VideoCoverWidget(this);

    //m_widget_2->setStyleSheet("QWidget{background-color: green;}");
    ui->gridLayout->addWidget(m_widget_2,0,1);



    //VideoDisplay* vdis_3=new VideoDisplay();
    m_widget_3=new VideoCoverWidget(this);

    //m_widget_3->setStyleSheet("QWidget{background-color: black;}");
    ui->gridLayout->addWidget(m_widget_3,1,0);


    //VideoDisplay* vdis_4=new VideoDisplay();
    m_widget_4=new VideoCoverWidget(this);

    //m_widget_4->setStyleSheet("QWidget{background-color: blue;}");
    ui->gridLayout->addWidget(m_widget_4,1,1);

    vec_wids.push_back(m_widget_1);
    vec_wids.push_back(m_widget_2);
    vec_wids.push_back(m_widget_3);
    vec_wids.push_back(m_widget_4);

}

VideoList::~VideoList()
{
    delete ui;
}

void VideoList::setUserVideoListsInfo()
{
    NetWorkManager net_manager{};
    QNetworkReply* reply=net_manager.http_get_user_video_lists_info(0);
    sloShowFilesInfo(reply);
}

void VideoList::setUserHistoryVideoListsInfo()
{
    NetWorkManager net_manager{};
    QNetworkReply* reply=net_manager.http_get_user_video_lists_info(1);
    sloShowFilesInfo(reply);
}

void VideoList::sloShowFilesInfo(QNetworkReply* reply)
{
    if(!reply){
        qDebug()<<"sloShowFilesInfo param is invalid";
        return;
    }
    QByteArray byte_array=reply->readAll();
    qDebug()<<"dsdsd:"<<byte_array;
    QJsonDocument json_docm=QJsonDocument::fromJson(byte_array);
    if(json_docm.isNull()){
        qDebug()<<"json document is null";
        return;
    }
    QJsonObject obj_root=json_docm.object();
    QString status=obj_root.value("status").toString();
    int file_info_cnt=obj_root.value("file_info_cnt").toInt();
    for(int i=0;i<file_info_cnt;i++){
        //给视频封面对象赋值
        qDebug()<<"i:::::"<<i;
        QJsonObject sobj=obj_root.value(QString::number(i)).toObject();
        QString file_img_path=sobj.value("file_image_path").toString();
        QString file_path=sobj.value("file_path").toString();
        QString file_title=sobj.value("file_title").toString();
        QString file_backplay_duration=sobj.value("file_backplay_duration").toString();
        QString file_md5=sobj.value("file_md5").toString();
        QString date_time=sobj.value("date_time").toString();
        QString file_playback_duration=sobj.value("file_playback_duration").toString();
        vec_wids[i]->m_file_path=file_path;
        vec_wids[i]->m_file_img_path=file_img_path;
        vec_wids[i]->m_intro=file_title;
        vec_wids[i]->m_file_md5=file_md5;
        qDebug()<<"send date_time:"<<date_time;
        vec_wids[i]->m_date_time=date_time;
        vec_wids[i]->m_duration=file_playback_duration;
        //赋值完毕后就触发信号去执行http请求
        //vec_wids[i]->sloRequestImg(file_img_path);
        connect(vec_wids[i],&VideoCoverWidget::sigRequestImg,vec_wids[i],&VideoCoverWidget::sloRequestImg);
        emit vec_wids[i]->sigRequestImg(file_img_path);
        //使用完立即解绑，不然会造成多次绑定，一次emit会触发多次slo处理
        disconnect(vec_wids[i], &VideoCoverWidget::sigRequestImg, vec_wids[i], &VideoCoverWidget::sloRequestImg);

    }
    qDebug()<<"delete reply";
    delete reply;
}
