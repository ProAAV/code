#include "aav_videodisplay.h"
#include "ui_aav_videodisplay.h"
#include"aav_usermanager.h"
#include"aav_networkmanager.h"
#include"aav_networkthread.h"
#include<QMediaPlayer>
#include<QVideoWidget>
#include<QLabel>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QSlider>
#include<QPushButton>
#include<QMenu>
#include<QVariantList>
#include<QCloseEvent>
#include<unistd.h>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QJsonValue>
#include"aav_videocoverwidlistloop.h"
#include <QPropertyAnimation>
#include <QRandomGenerator>
#include<QSizePolicy>
VideoDisplay::VideoDisplay(QString& file_md5,QString& file_type,QWidget *parent) :
    QWidget(parent),ui(new Ui::VideoDisplay),m_file_path(""),m_file_type(file_type),m_ptr(0),animation{nullptr},danmuLabel{nullptr}
{
    ui->setupUi(this);
    qDebug()<<"VideoDisplay create and vec:"<<vec_list_loop_wids.size();
    player_mode=0;

    list_loop_wid=new QWidget(this);
    vlayout_list_loop_wid=new QVBoxLayout(list_loop_wid);
    vlayout_list_loop_wid->setAlignment(Qt::AlignTop);
    ui->scrollArea->setWidget(list_loop_wid);
    m_cnt_pause_player=0;
    m_file_md5=file_md5;


    qDebug()<<"videodisplay init";
    /*
     视频播放类
    */
    //视频播放类mediaplayer和视频显示类videowidget的创建和交互，作为该页面的第一板块
    m_player=new QMediaPlayer(this);
    m_video_widget=new QVideoWidget(this);
    m_player->setVideoOutput(m_video_widget);

    m_media_list=new QMediaPlaylist(this);
    //必须在player和video_widget初始化完成后再进行
    //createDanmu("hello world!",5000);
    //当前进度，分隔符，总时长再加一个进度条的创建，作为当前页面的第二个板块

    m_lab_present_time=new QLabel(this);
    m_lab_present_time->setText("00:00:00");
    QLabel* lab_sepra=new QLabel(this);
    lab_sepra->setText("/");
    m_lab_dur_time=new QLabel(this);
    m_lab_dur_time->setText("00:00:00");
    m_slider_video_process=new QSlider(Qt::Horizontal,this);
    m_slider_video_process->setRange(0,0);
    QHBoxLayout* hlayout_slider=new QHBoxLayout();
    hlayout_slider->addWidget(m_lab_present_time);
    hlayout_slider->addWidget(lab_sepra);
    hlayout_slider->addWidget(m_lab_dur_time);
    hlayout_slider->addWidget(m_slider_video_process);
    //暂停，视频下一集,倍速，音量的按钮，播放模式按钮

    QPushButton* btn_video_pause=new QPushButton(this);
    btn_video_pause->setText("暂停");
    QPushButton* btn_video_next=new QPushButton(this);
    btn_video_next->setText("next");
    QPushButton* btn_play_mode=new QPushButton(this);
    btn_play_mode->setText("模式");
    m_btn_rate=new QPushButton(this);
    m_menu_mode=new QMenu(this);
    m_menu_rate=new QMenu(this);
    QAction* act_rate_1=new QAction(this);
    act_rate_1->setText("0.5X");
    QAction* act_rate_2=new QAction(this);
    act_rate_2->setText("0.75X");
    QAction* act_rate_3=new QAction(this);
    act_rate_3->setText("1.0X");
    QAction* act_rate_4=new QAction(this);
    act_rate_4->setText("1.5X");
    QAction* act_rate_5=new QAction(this);
    act_rate_5->setText("2.0X");
    m_menu_rate->addAction(act_rate_1);
    m_menu_rate->addAction(act_rate_2);
    m_menu_rate->addAction(act_rate_3);
    m_menu_rate->addAction(act_rate_4);
    m_menu_rate->addAction(act_rate_5);
    m_btn_rate->setMenu(m_menu_rate);

    QAction* act_mode_1=new QAction(this);
    act_mode_1->setText(" 顺序播放");
    QAction* act_mode_2=new QAction(this);
    act_mode_2->setText("随机播放");
    QAction* act_mode_3=new QAction(this);
    act_mode_3->setText("单个播放");
    QAction* act_mode_4=new QAction(this);
    act_mode_4->setText("单个循环");
    QAction* act_mode_5=new QAction(this);
    act_mode_5->setText("列表循环");
    m_menu_mode->addAction(act_mode_1);
    m_menu_mode->addAction(act_mode_2);
    m_menu_mode->addAction(act_mode_3);
    m_menu_mode->addAction(act_mode_4);
    m_menu_mode->addAction(act_mode_5);
    btn_play_mode->setMenu(m_menu_mode);
    connect(act_mode_1,&QAction::triggered,this,[=](){
        qDebug()<<"enter SEQUENCE";
        /*m_media_list->addMedia(QMediaContent(m_file_path));

        for(const auto&wid:vec_list_loop_wids){
            qDebug()<<"addaddadd";
            m_media_list->addMedia(QMediaContent(wid->m_file_path));
        }*/
        /*int count = m_media_list->mediaCount();
                    for (int i = 0; i < count; ++i) {
                        QMediaContent content = m_media_list->media(i);
                        QUrl url = content.canonicalUrl();

                        qDebug() << "Media content at index" << i << ":" << url.toString();
        }
        m_player->setPlaylist(m_media_list);

        m_media_list->setPlaybackMode(QMediaPlaylist::Sequential);*/
        player_mode=Mode::SEQUENCE;

    });

    connect(act_mode_2,&QAction::triggered,this,[=](){
        player_mode=Mode::RANDOM;

    });
    connect(act_mode_3,&QAction::triggered,this,[=](){
        player_mode=Mode::SINGLE;
    });
    connect(act_mode_4,&QAction::triggered,this,[=](){
        player_mode=Mode::SINGLELOOP;

    });
    connect(act_mode_5,&QAction::triggered,this,[=](){
        player_mode=Mode::LOOP;
    });



    m_btn_vlm=new VolumeButton(this);
    m_btn_vlm->setText("音量");

    m_hlayout=new QHBoxLayout;

    m_hlayout->addWidget(btn_video_pause);
    m_hlayout->addWidget(btn_video_next);
    m_hlayout->addWidget(m_btn_rate);
    m_hlayout->addWidget(m_btn_vlm);
    m_hlayout->addWidget(btn_play_mode);
    //下一集按钮
    connect(btn_video_next,&QPushButton::clicked,this,&VideoDisplay::sloHandleBtnNext);

    //实现暂停，倍速播放
    connect(btn_video_pause,&QPushButton::clicked,this,&VideoDisplay::sloPlayerPause);
    connect(m_btn_rate,&QPushButton::clicked,this,&VideoDisplay::sloMenuUnfold);
    connect(act_rate_1,&QAction::triggered,this,[=](){
        this->sloAdjRate(1);
    });
    connect(act_rate_2,&QAction::triggered,this,[=](){
        this->sloAdjRate(2);
    });
    connect(act_rate_3,&QAction::triggered,this,[=](){
        this->sloAdjRate(3);
    });
    connect(act_rate_4,&QAction::triggered,this,[=](){
        this->sloAdjRate(4);
    });
    connect(act_rate_5,&QAction::triggered,this,[=](){
        this->sloAdjRate(5);
    });
    //最后用一个垂直布局将三个板块合并
    m_vlayout=new QVBoxLayout;
    m_vlayout->addWidget(m_video_widget);

    m_vlayout->addLayout(hlayout_slider);
    m_vlayout->addLayout(m_hlayout);
    ui->frame->setLayout(m_vlayout);



    //实现进度条与视频播放的同步
    connect(m_player,&QMediaPlayer::durationChanged,this,&VideoDisplay::sloSetSliderDura);
    connect(m_player,&QMediaPlayer::positionChanged,this,&VideoDisplay::sloSetSliderPos);
    connect(m_slider_video_process,&QSlider::sliderMoved,this,&VideoDisplay::sloPlayerMove);
    //实现音频同步
    connect(m_btn_vlm->getSlider(),&QSlider::sliderMoved,this,[this](int position){
        this->sloVolumeChanged(position);
    });

    //实现推荐列表的显示
    NetWorkThread* thread=new NetWorkThread(this);
    thread->m_net_manager->http_get_recommend_audio_lists(file_md5,m_file_type,this);
    connect(thread,&QThread::finished,this,[thread](){
        thread->deleteLater();
    });
    qDebug()<<"m_file_path:"<<m_file_path;


}


void VideoDisplay::sloSetSliderDura(qint64 dur){
    int val=static_cast<int>(dur);
    m_slider_video_process->setRange(0,val);
    updateDurationTimeLab(dur);
}
void VideoDisplay::sloSetSliderPos(qint64 dur){
    int val=static_cast<int>(dur);
    //qDebug()<<"val:"<<val<<" "<<"dur:"<<dur;
    m_slider_video_process->setValue(val);
    updatePresentTimeLab(dur);


    int second=val/1000;
    checkDanmuPresentTime(second);


}
void VideoDisplay::sloPlayerMove(int pos){
    m_player->setPosition(pos);
}

QString VideoDisplay::integraTime(qint64 tim,int opt){
    //opt作为一个判断参数，0表示传入的不是秒数，1表示传入秒数
    qint64 time_as=0;
    if(opt==1){
        time_as=tim;
    }
    //tim是毫秒，先得到秒
    else if(opt==0){
        time_as=tim/1000;
    }
    //再计算小时数
    qint64 time_h=time_as/3600;
    //再计算分钟数
    qint64 time_=time_as%3600;
    qint64 time_m=time_/60;
    //最后计算秒数
    qint64 time_s=time_%60;
    //转为字符串
    QString str_h=QString::number(time_h);
    QString str_m=QString::number(time_m);
    QString str_s=QString::number(time_s);
    if(time_h<10){
        str_h="0"+QString::number(time_h);
    }
    if(time_m<10){
        str_m="0"+QString::number(time_m);
    }
    if(time_s<10){
        str_s="0"+QString::number(time_s);
    }
    QString str_duration=str_h+":"+str_m+":"+str_s;
    return str_duration;
}

void VideoDisplay::setVideoFilePath(QString &file_path)
{
    m_file_path=file_path;
}

void VideoDisplay::setVideoFileProgressData(qint64 progress)
{
    m_player->setPosition(progress);
}

void VideoDisplay::play(const QString& file_path,const QString& file_md5)
{
    //播放视频
    //通过请求到ts视频文件的m3u8文件列表来实现QMediaPlayer配合m3u8文件来播放连续的ts视频文件
    //在这里videodisplay先被初始化也就是说先使用m_file_path开始了，只不过还没有show出来，但是这时候m_file_path还没有被设置
    qDebug()<<"play begin";
    if(file_path==""){
        qDebug()<<"file_path is invalid null";
        return;
    }

    QUrl url(file_path);
    m_media_list->addMedia(QMediaContent(m_file_path));
    m_player->stop();

    m_player->setMedia(QMediaContent(url));
    //这是后面改的，可能会出问题
    m_file_path=file_path;

    connect(m_player,&QMediaPlayer::mediaStatusChanged,this,&VideoDisplay::sloPreload);

    //每一次的视频播放都要解析弹幕需要的信息
    /*NetWorkThread* thread=new NetWorkThread();
    qDebug()<<"enter play 111";
    thread->m_net_manager->http_get_video_analysis_messages(m_file_md5,this);
    connect(thread,&QThread::finished,this,[thread](){
        thread->deleteLater();
    });*/
    //异步获取数据并且解析
    vec_end_time.clear();
    vec_messages.clear();
    vec_start_time.clear();
    vec_descriptions.clear();
    m_ptr=0;
    vis.reset();

    if(danmuLabel){
        if(animation){
            qDebug()<<"animation is ok";
            animation->stop();
        }
        if(danmuLabel){
            delete danmuLabel;
            danmuLabel=nullptr;
        }


    }
    if(m_file_type=="1"){
        //视频才会去请求视频解析
        NetWorkThread* thread=new NetWorkThread();
        thread->m_net_manager->http_get_video_analysis_messages(file_md5,this,9);
        connect(thread,&QThread::finished,this,[thread](){
            thread->deleteLater();
        });
    }


    /*for(int i=1;i<=10;i++){
        QTimer::singleShot(30000, this, [=](){
            NetWorkThread* thread=new NetWorkThread();
            qDebug()<<"enter play 111";
            thread->m_net_manager->http_get_video_analysis_messages(m_file_md5,this,i);
            connect(thread,&QThread::finished,this,[thread](){
                thread->deleteLater();
            });
        });
    }*/


    //createDanmu("hhhhhhhhhh",3000);

}

void VideoDisplay::listLoopCoverShowInfo(QNetworkReply* reply)
{
    while (QLayoutItem *item = vlayout_list_loop_wid->takeAt(0)) {
        // 获取子控件
        QWidget *widget = item->widget();
        if (widget) {
            // 删除子控件
            delete widget;
        }
        // 删除布局项
        delete item;
    }
    vec_list_loop_wids.clear();
    QByteArray byte_array=reply->readAll();

    qDebug()<<"listLoopCoverShowInfo:"<<byte_array;
    QJsonDocument json_docm=QJsonDocument::fromJson(byte_array);
    if(json_docm.isNull()){
        qDebug()<<"json document is null";
        return;
    }
    QJsonObject obj_root=json_docm.object();
    QString status=obj_root.value("status").toString();
    qDebug()<<"status:"<<status;
    if(status!="0"){
        qDebug("error CoverListLoopShowInfo");
        return;
    }
    int file_info_cnt=obj_root.value("file_info_cnt").toInt();
    //直接根据数据库返回记录数动态创建对应的VideoCover
    for(int i=0;i<file_info_cnt;i++){
        qDebug()<<"listLoopCoverShowInfo cnt:"<<i;
        VideoCoverWidListLoop* wid=new VideoCoverWidListLoop(list_loop_wid);
        connect(wid,&VideoCoverWidListLoop::sigPlay,this,[=](){
            //换播放地址，实现播放切换
            qDebug()<<"changed!!!!!!!!!!!!!!!!!!";
            this->play(wid->m_file_path,wid->m_file_md5);
            NetWorkManager net_manager;
            net_manager.http_get_recommend_audio_lists(wid->m_file_md5,wid->m_file_type,this);
            //再来一遍listLoopCoverShowInfo过程
            /*NetWorkThread* thread=new NetWorkThread();
            thread->m_net_manager->http_get_recommend_audio_lists(wid->m_file_md5,wid->m_file_type,this);
            connect(thread,&QThread::finished,this,[=](){
                qDebug()<<"listLoopCoverShowInfo thread finished";
                for(int i=0;i<vec_list_loop_wids.size();i++){
                    //delete vec_list_loop_wids.last();
                    vec_list_loop_wids.pop_back();
                }
                while (QLayoutItem *item = vlayout_list_loop_wid->takeAt(0)) {
                        // 获取子控件
                        QWidget *widget = item->widget();
                        if (widget) {
                            // 删除子控件
                            delete widget;
                        }
                        // 删除布局项
                        delete item;
                }
                thread->deleteLater();
            });*/
        });
        wid->setFixedSize(200,200);
        vlayout_list_loop_wid->addWidget(wid);

        vec_list_loop_wids.push_back(wid);

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
        qDebug()<<"=========file_title:"<<file_title;
        vec_list_loop_wids[i]->m_file_path=file_path;
        vec_list_loop_wids[i]->m_file_img_path=file_img_path;
        vec_list_loop_wids[i]->m_intro=file_title;
        vec_list_loop_wids[i]->m_file_md5=file_md5;
        vec_list_loop_wids[i]->m_username=file_username;
        vec_list_loop_wids[i]->m_file_type=file_type;
        //赋值完毕直接调用vec_list_loop_wids[i]的显示方法
        vec_list_loop_wids[i]->showCover();
        m_media_list->addMedia(QMediaContent(file_path));
    }
    //m_media_list->setPlaybackMode(QMediaPlaylist::Sequential);

    m_player->setPlaylist(m_media_list);
}

void VideoDisplay::createDanmu(const QString &text, int duration)
{
    qDebug()<<"createDanmu durayion:"<<duration;
    if(animation){
        delete animation;
    }
    //QTimer::singleShot(delay, this, [=]() {
    qDebug()<<"text:"<<text;
    danmuLabel = new QLabel(text, this);
    danmuLabel->setStyleSheet("background: black; color: white; font-size: 12px;");
    //danmuLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    //danmuLabel->setMinimumWidth(danmuLabel->fontMetrics().horizontalAdvance(danmuLabel->text()));

    danmuLabel->setMinimumSize(danmuLabel->sizeHint());
    danmuLabel->setMaximumSize(danmuLabel->sizeHint());

    danmuLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    //danmuLabel->setFixedHeight(40);
    //danmuLabel->hide();
    // 设置 QLabel 的初始位置
    /*int startX=0;
    int endX=0;
    int y=0;*/
    int startX = m_video_widget->width();
    qDebug()<<"-000000000000--------:"<<startX;
    int endX = -danmuLabel->width();
    //int y = qrand() % (m_video_widget->height() - danmuLabel->height());
    int y=50;
    // 创建动画
    int width=danmuLabel->width();
    int height=danmuLabel->height();
    animation = new QPropertyAnimation(danmuLabel, "geometry");
    animation->setDuration(duration*1000); //
    animation->setStartValue(QRect(startX-width, y, width, height));
    animation->setEndValue(QRect(endX+width, y, danmuLabel->width(), danmuLabel->height()));
    // 设置动画完成后删除 QLabel
    connect(animation, &QPropertyAnimation::finished, danmuLabel, [=](){
        if(danmuLabel){
            delete danmuLabel;
        }
    });
    // 延迟显示弹幕
    //QTimer::singleShot(delay, this, [=]() {
    danmuLabel->show();
    animation->start();

    //});
    //});
}

void VideoDisplay::analyzeDanmuMessages(QNetworkReply* reply)
{
    qDebug("enter analyzeDanmuMessages");
    if(!reply){
        qDebug()<<"analyzeDanmuMessages reply is null";
        reply->deleteLater();
        return;
    }
    QByteArray byte_array=reply->readAll();
    if(byte_array==""){
        qDebug()<<"analyzeDanmuMessages recv null";
        return;
    }
    qDebug()<<"analyzeDanmuMessages byte_array:"<<byte_array;
    QJsonDocument json_docm=QJsonDocument::fromJson(byte_array);
    if(json_docm.isNull()){
        qDebug()<<"json document is null";
        return;
    }
    QJsonObject obj_root=json_docm.object();
    QString status=obj_root.value("status").toString();
    if(status!="0"){
        qDebug("error with analyzeDanmuMessages query");
        reply->deleteLater();
        return;
    }
    QString message=obj_root.value("message").toString();

    parseMessages(message);

    for(int i=0;i<vec_messages.size();i++){
        QString str=vec_messages.at(i);
        int start_pos=str.indexOf("**",0);
        if(start_pos==-1){
            continue;
        }
        QString start_time=str.mid(start_pos+2,8);
        vec_start_time.push_back(timeToSeconds(start_time));
        start_pos=str.indexOf("- ",start_pos);
        if(start_pos==-1){
            continue;
        }
        QString end_time=str.mid(start_pos+2,8);
        vec_end_time.push_back(timeToSeconds(end_time));
        start_pos=str.indexOf("- ",start_pos+2);
        if(start_pos==-1){
            continue;
        }
        QString file_description=str.mid(start_pos,-1);
        vec_descriptions.push_back(file_description);
        qDebug()<<start_time<<" "<<end_time<<" "<<file_description;
    }
}

int VideoDisplay::timeToSeconds(const QString &qtTimeStr)
{

        // 使用 split 方法按冒号分割字符串
        QStringList parts = qtTimeStr.split(':');

        // 检查分割后的部分数量是否正确
        if (parts.size() != 3) {
            throw std::invalid_argument("时间格式不正确，应为 HH:MM:SS");
        }

        // 提取小时、分钟和秒
        bool ok;
        int hours = parts[0].toInt(&ok);
        if (!ok || hours < 0 || hours > 23) {
            throw std::invalid_argument("小时部分无效或超出范围");
        }

        int minutes = parts[1].toInt(&ok);
        if (!ok || minutes < 0 || minutes > 59) {
            throw std::invalid_argument("分钟部分无效或超出范围");
        }

        int seconds = parts[2].toInt(&ok);
        if (!ok || seconds < 0 || seconds > 59) {
            throw std::invalid_argument("秒部分无效或超出范围");
        }

        // 计算总秒数
        return hours * 3600 + minutes * 60 + seconds;

}

void VideoDisplay::parseMessages(QString &message)
{
    qDebug()<<"message:"<<message;
    int cur_head_pos=0;
    int cur_tail_pos=0;
    while(1){
        cur_head_pos=message.indexOf("**",cur_head_pos);
        if(cur_head_pos==-1){
            qDebug("break ok");
            break;
        }
        cur_tail_pos=message.indexOf("\n\n",cur_head_pos);
        if(cur_tail_pos==-1){
            return;
        }
        QString substr=message.mid(cur_head_pos,cur_tail_pos-cur_head_pos);
        qDebug()<<"parseMessages str:"<<substr;
        cur_head_pos=cur_tail_pos+2;
        vec_messages.push_back(substr);

    }
}

void VideoDisplay::checkDanmuPresentTime(int second)
{
        qDebug()<<"enter checkDanmuPresentTime";
        int i=static_cast<int>(m_ptr);
        if(i>=vec_start_time.size()){
            return;
        }
        if(vec_start_time.at(i)<=second&&vec_end_time.at(i)>second&&!vis[m_ptr]){
            qDebug()<<"createDanmu";
            qDebug()<<"m_ptr"<<m_ptr;
            qDebug()<<vis[m_ptr];
            qDebug()<<"create Danumu!!!!!";
            createDanmu(vec_descriptions.at(i),vec_end_time.at(i)-vec_start_time.at(i));
            vis[m_ptr]=true;
            m_ptr++;

        }
}

void VideoDisplay::handleModeChange()
{

    if(player_mode==SEQUENCE){
        qDebug("enter handleModeChange");
        //m_media_list->addMedia(QMediaContent(m_file_path));
        /*for(const auto wid:vec_list_loop_wids){
            //qDebug()<<"addaddadd";
            qDebug()<<"wid file_path:"<<wid->m_file_path;
            m_media_list->addMedia(QMediaContent(wid->m_file_path));
        }*/
        int count = m_media_list->mediaCount();
                            for (int i = 0; i < count; ++i) {
                                QMediaContent content = m_media_list->media(i);
                                QUrl url = content.canonicalUrl();

                                qDebug() << "Media content at index" << i << ":" << url.toString();
                }
        m_media_list->setPlaybackMode(QMediaPlaylist::Sequential);
        //m_media_list->setCurrentIndex(1);
        if(!m_player->playlist()){
            m_player->setPlaylist(m_media_list);
        }

        connect(m_media_list,&QMediaPlaylist::currentIndexChanged,this,[=](int index){
            qDebug()<<"current index:"<<index;
        });

        qDebug("out handleModeChange");
    }
    else if(player_mode==RANDOM){
        /*m_media_list->addMedia(QMediaContent(m_file_path));
        for(const auto&wid:vec_list_loop_wids){
            m_media_list->addMedia(QMediaContent(wid->m_file_path));
        }*/
        m_media_list->setPlaybackMode(QMediaPlaylist::Random);
        if(!m_player->playlist()){
            m_player->setPlaylist(m_media_list);
        }
        m_player->play();

    }
    else if(player_mode==SINGLE){

    }
    else if(player_mode==SINGLELOOP){
        /*m_media_list->addMedia(QMediaContent(m_file_path));*/
        qDebug("singloop");
        m_media_list->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

        if(!m_player->playlist()){
            m_player->setPlaylist(m_media_list);
        }


    }
    else if(player_mode==LOOP){
        /*m_media_list->addMedia(QMediaContent(m_file_path));
        for(const auto&wid:vec_list_loop_wids){
            m_media_list->addMedia(QMediaContent(wid->m_file_path));
        }*/
        m_media_list->setPlaybackMode(QMediaPlaylist::Loop);
        if(!m_player->playlist()){
            m_player->setPlaylist(m_media_list);
        }

    }
}
void VideoDisplay::updatePresentTimeLab(qint64 tim){
    m_lab_present_time->setText(integraTime(tim,0));
}
void VideoDisplay::updateDurationTimeLab(qint64 tim){
    m_lab_dur_time->setText(integraTime(tim,0));
}
void VideoDisplay::sloPlayerPause(){
    //偶数次暂停，奇数次继续
    if(m_cnt_pause_player%2){
        m_player->play();
    }
    else{
        m_player->pause();
    }
    m_cnt_pause_player++;


}
void VideoDisplay::sloMenuUnfold(){
    QPoint pos=m_btn_rate->mapToGlobal(QPoint(0,0));
    QPoint menuPos = pos - QPoint(0, m_menu_rate->sizeHint().height());
    m_menu_rate->popup(menuPos);
}
void VideoDisplay::sloAdjRate(int num){
    switch (num) {
    case 1:
        m_player->setPlaybackRate(0.5);
        break;
    case 2:
        m_player->setPlaybackRate(0.75);
        break;
    case 3:
        m_player->setPlaybackRate(1.0);
        break;
    case 4:
        m_player->setPlaybackRate(1.5);
        break;
    case 5:
        m_player->setPlaybackRate(2.0);
        break;
    default:
        break;
    }
}
//预加载，当完全加载之后才播放视频
void VideoDisplay::sloPreload(QMediaPlayer::MediaStatus status){
    if(status==QMediaPlayer::LoadedMedia){
        m_player->play();
    }
    else if(status==QMediaPlayer::EndOfMedia){
        handleModeChange();
    }
}

void VideoDisplay::sloVolumeChanged(int position)
{
    m_player->setVolume(position);

    m_btn_vlm->getLabVlm()->setText(QString::number(position));
}

Ui::VideoDisplay *VideoDisplay::getVideoDisplayUi()
{
    return ui;
}

void VideoDisplay::sloHandleBtnNext()
{
    //先获取下一集的file_path
    int num=vlayout_list_loop_wid->count();
    int randomInt = QRandomGenerator::global()->bounded(10000);
    int idx=randomInt%num;
    VideoCoverWidListLoop* wid=qobject_cast<VideoCoverWidListLoop*>(vlayout_list_loop_wid->itemAt(idx)->widget());

    emit wid->sigPlay();

    qDebug()<<"width:"<<m_video_widget->width();
    qDebug()<<"height:"<<m_video_widget->height();
}

void VideoDisplay::closeEvent(QCloseEvent *event)
{
    qDebug()<<"close event";

    int progress_value= m_slider_video_process->value();
    m_player->stop(); // 停止播放


    // 断开视频输出
    m_player->setVideoOutput(static_cast<QVideoWidget*>(nullptr));    // 清空媒体
    m_player->setMedia(QMediaContent());

    m_video_widget->close();

    //更新用户历史观看记录表
    QString username=UserManager::instance()->getUserName();
    if(username!=""){
        /*NetWorkManager net_manager{};

        net_manager.http_insert_user_history_log(username,m_file_md5,progress_value);*/
        NetWorkThread* thread=new NetWorkThread();
        thread->m_net_manager->http_insert_user_history_log(username,m_file_md5,progress_value);
        connect(thread,&NetWorkThread::finished,this,[thread](){
            thread->deleteLater();
        });
    }
    emit sigClose();
    QWidget::closeEvent(event);

}


VideoDisplay::~VideoDisplay()
{
    qDebug()<<"close close";
    //delete m_hlayout;
    //delete m_vlayout;
    delete ui;
}
