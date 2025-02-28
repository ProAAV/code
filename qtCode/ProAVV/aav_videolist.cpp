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

    VideoDisplay* vdis=new VideoDisplay();
    m_widget_1=new VideoCoverWidget(vdis,this);

    m_widget_1->setStyleSheet("QWidget{background-color: red;}");
    ui->gridLayout->addWidget(m_widget_1,0,0);


    m_widget_2=new VideoCoverWidget(vdis,this);

    m_widget_2->setStyleSheet("QWidget{background-color: green;}");
    ui->gridLayout->addWidget(m_widget_2,0,1);




    m_widget_3=new VideoCoverWidget(vdis,this);

    m_widget_3->setStyleSheet("QWidget{background-color: black;}");
    ui->gridLayout->addWidget(m_widget_3,1,0);

    m_widget_4=new VideoCoverWidget(vdis,this);

    m_widget_4->setStyleSheet("QWidget{background-color: blue;}");
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
            QJsonObject sobj=obj_root.value(QString::number(i)).toObject();
            QString file_img_path=sobj.value("file_image_path").toString();
            QString file_path=sobj.value("file_path").toString();
            QString file_title=sobj.value("file_title").toString();
            QString file_backplay_duration=sobj.value("file_backplay_duration").toString();
            vec_wids[i]->m_file_path=file_path;
            vec_wids[i]->m_file_img_path=file_img_path;
            vec_wids[i]->m_lab_intro->setText(file_title);

            //赋值完毕后就触发信号去执行http请求
            connect(vec_wids[i],&VideoCoverWidget::sigRequestImg,vec_wids[i],&VideoCoverWidget::sloRequestImg);
            vec_wids[i]->sigRequestImg(file_img_path);
        }
}
