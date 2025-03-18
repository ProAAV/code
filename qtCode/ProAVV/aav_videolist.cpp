#include "aav_videolist.h"
#include "ui_aav_videolist.h"
#include<QPushButton>
#include<QLineEdit>
#include<QStandardItemModel>
#include<QListView>
#include"aav_videocoverwidget.h"
#include"aav_networkthread.h"
#include<QWidget>
#include<QLabel>
#include<aav_networkmanager.h>
#include"aav_videodisplay.h"
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QJsonValue>
#include<QScrollBar>
VideoList::VideoList(int flag,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoList),m_x{0},m_y{0},m_idx{0},m_offset{0}
{
    ui->setupUi(this);
    m_flag=flag;
    m_thread=new NetWorkThread(this);
    /*
    视频列表展示类
    */

    /*//VideoDisplay* vdis_1=new VideoDisplay();
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

    m_vec_video_cover_wids.push_back(m_widget_1);
    m_vec_video_cover_wids.push_back(m_widget_2);
    m_vec_video_cover_wids.push_back(m_widget_3);
    m_vec_video_cover_wids.push_back(m_widget_4);*/

    /*addVideoCoverWidget(0,0);
    addVideoCoverWidget(0,1);
    addVideoCoverWidget(1,0);
    addVideoCoverWidget(1,1);*/

    /*addVideoCoverWidget(m_x,m_y);
    addVideoCoverWidget(m_x,m_y);
    addVideoCoverWidget(m_x,m_y);
    addVideoCoverWidget(m_x,m_y);*/
    ui->gridLayout->setAlignment(Qt::AlignTop);
    //连接scroll滚动条的事件与槽
    connect(ui->scrollArea->verticalScrollBar(),&QScrollBar::valueChanged,this,[=](int value){
        this->sloHandleScrollBarValueChanged(value);
    });

}

VideoList::~VideoList()
{
    delete ui;
}

void VideoList::setUserVideoListsInfo()
{
    /*NetWorkManager net_manager{};
    QNetworkReply* reply=net_manager.http_get_user_video_lists_info(0);*/
    if(!m_thread){
        m_thread=new NetWorkThread(this);
    }
    //NetWorkThread* thread=new NetWorkThread(this);
    if(!m_thread->isRunning()){
        m_thread->m_net_manager->http_get_user_video_lists_info(0,this);
        connect(m_thread,&NetWorkThread::finished,this,[=](){
            m_thread->deleteLater();
        });
    }

    //sloShowFilesInfo(reply);
}

void VideoList::setUserHistoryVideoListsInfo()
{
    if(!m_thread){
        m_thread=new NetWorkThread(this);
    }
    //NetWorkThread* thread=new NetWorkThread(this);
    if(!m_thread->isRunning()){
        m_thread->m_net_manager->http_get_user_video_lists_info(1,this);
        connect(m_thread,&NetWorkThread::finished,this,[=](){
            m_thread->deleteLater();
        });
    }
    //sloShowFilesInfo(reply);
}

void VideoList::getNewVideoListsInfo(VideoList* list)
{
    if(!m_thread){
        m_thread=new NetWorkThread(this);
    }
    //NetWorkThread* thread=new NetWorkThread(this);
    if(!m_thread->isRunning()){
        m_thread->m_net_manager->http_get_new_video_lists_file_info(m_flag,list,m_offset);
        connect(m_thread,&NetWorkThread::finished,this,[=](){
            m_thread->deleteLater();
        });
    }
}

VideoCoverWidget* VideoList::addVideoCoverWidget(int x,int y)
{
    qDebug()<<"x:"<<x<<" "<<"y:"<<y;
    VideoCoverWidget* video_cover_wid=new VideoCoverWidget();
    if(!video_cover_wid){
        qDebug("erorr to create video_cover_wid");
        return nullptr;
    }

    ui->gridLayout->addWidget(video_cover_wid,x,y);
    m_vec_video_cover_wids.push_back(video_cover_wid);
    if(y==1){
        m_x+=1;
    }
    m_y=(m_y+1)%2;
    return video_cover_wid;
}

void VideoList::reload()
{

    m_x=0;
    m_y=0;
    m_idx=0;
    m_offset=0;
    //qDeleteAll(m_vec_video_cover_wids);
    int size=m_vec_video_cover_wids.size();
    qDebug()<<"reload size:"<<size;
    for(int i=0;i<size;i++){
        auto wid=m_vec_video_cover_wids.last();
        m_vec_video_cover_wids.pop_back();
        delete wid;
    }
    //m_vec_video_cover_wids.clear();

    qDebug()<<"m_vec_video_cover_wids size:"<<m_vec_video_cover_wids.size();
    /*addVideoCoverWidget(m_x,m_y);
    addVideoCoverWidget(m_x,m_y);
    addVideoCoverWidget(m_x,m_y);
    addVideoCoverWidget(m_x,m_y);*/

}



void VideoList::sloShowFilesInfo(QNetworkReply* reply)
{
    qDebug()<<"offset:"<<m_offset;
    if(!reply){
        qDebug()<<"sloShowFilesInfo param is invalid";
        return;
    }
    QByteArray byte_array=reply->readAll();
    qDebug()<<"recv file lists info:"<<byte_array;
    QJsonDocument json_docm=QJsonDocument::fromJson(byte_array);
    if(json_docm.isNull()){
        qDebug()<<"json document is null";
        return;
    }
    QJsonObject obj_root=json_docm.object();
    QString status=obj_root.value("status").toString();
    int file_info_cnt=obj_root.value("file_info_cnt").toInt();
    //直接根据数据库返回记录数动态创建对应的VideoCover
    for(int i=0;i<file_info_cnt;i++){
        addVideoCoverWidget(m_x,m_y);
    }
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
        QString progress_data=sobj.value("progress_data").toString();
        QString file_username=sobj.value("username").toString();
        QString file_type=sobj.value("file_type").toString();
        m_vec_video_cover_wids[m_idx]->m_file_path=file_path;
        m_vec_video_cover_wids[m_idx]->m_file_img_path=file_img_path;
        m_vec_video_cover_wids[m_idx]->m_intro=file_title;
        m_vec_video_cover_wids[m_idx]->m_file_md5=file_md5;
        m_vec_video_cover_wids[m_idx]->m_username=file_username;
        m_vec_video_cover_wids[m_idx]->m_file_type=file_type;
        qDebug()<<"file_path"<<file_path;
        qDebug()<<"file_img_path"<<file_img_path;
        qDebug()<<"file_title"<<file_title;
        qDebug()<<"file_md5"<<file_md5;
        qDebug()<<"file_username"<<file_username;
        qDebug()<<"file_type"<<file_type;
        qDebug()<<"m_progress_data"<<progress_data;
        m_vec_video_cover_wids[m_idx]->m_progress_data=progress_data;
        qDebug()<<"send date_time:"<<date_time;
        m_vec_video_cover_wids[m_idx]->m_date_time=date_time;
        m_vec_video_cover_wids[m_idx]->m_duration=file_playback_duration;
        //赋值完毕后就触发信号去执行http请求
        //m_vec_video_cover_wids[i]->sloRequestImg(file_img_path);
        m_vec_video_cover_wids[m_idx]->sloRequestImg(file_img_path);
        m_idx++;
        /*connect(m_vec_video_cover_wids[m_idx],&VideoCoverWidget::sigRequestImg,m_vec_video_cover_wids[m_idx],&VideoCoverWidget::sloRequestImg);
        emit m_vec_video_cover_wids[m_idx]->sigRequestImg(file_img_path);
        //使用完立即解绑，不然会造成多次绑定，一次emit会触发多次slo处理
        disconnect(m_vec_video_cover_wids[m_idx], &VideoCoverWidget::sigRequestImg, m_vec_video_cover_wids[m_idx], &VideoCoverWidget::sloRequestImg);
        m_idx++;*/
    }
    m_offset+=file_info_cnt;
    qDebug()<<"delete reply";
    delete reply;
}

void VideoList::sloHandleScrollBarValueChanged(int value)
{
    qDebug("enter sloHandleScrollBarValueChanged");
    int scrollBarMaximum = ui->scrollArea->verticalScrollBar()->maximum();
    if (value >= scrollBarMaximum - 100){
        //快到底部时动态加载4个视频

        /*for(int i=0;i<4;i++){
            addVideoCoverWidget(m_x,m_y);
        }*/
        qDebug("enter sloHandleScrollBarValueChanged create new finished");
        //请求数据
        getNewVideoListsInfo(this);
    }
}
