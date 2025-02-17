#include "aav_videodisplay.h"
#include "ui_aav_videodisplay.h"
#include<QMediaPlayer>
#include<QVideoWidget>
#include<QLabel>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QSlider>
#include<QPushButton>
#include<QMenu>
#include<QVariantList>

VideoDisplay::VideoDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoDisplay)
{
    ui->setupUi(this);
    m_cnt_pause_player=0;
    /*
     视频播放类
    */
    //视频播放类mediaplayer和视频显示类videowidget的创建和交互，作为该页面的第一板块
    m_player=new QMediaPlayer(this);
    m_video_widget=new QVideoWidget(this);
    m_player->setVideoOutput(m_video_widget);
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
    //视频上一集，暂停，下一集,倍速，音量的按钮
    QPushButton* btn_video_pre=new QPushButton(this);
    btn_video_pre->setText("ooooo");
    QPushButton* btn_video_pause=new QPushButton(this);
    btn_video_pause->setText("kkkkk");
    QPushButton* btn_video_next=new QPushButton(this);
    btn_video_next->setText("jjjjj");
    m_btn_rate=new QPushButton(this);
    m_menu_rate=new QMenu(this);
    QAction* act_1=new QAction(this);
    act_1->setText("0.5X");
    QAction* act_2=new QAction(this);
    act_2->setText("0.75X");
    QAction* act_3=new QAction(this);
    act_3->setText("1.0X");
    QAction* act_4=new QAction(this);
    act_4->setText("1.5X");
    QAction* act_5=new QAction(this);
    act_5->setText("2.0X");
    m_menu_rate->addAction(act_1);
    m_menu_rate->addAction(act_2);
    m_menu_rate->addAction(act_3);
    m_menu_rate->addAction(act_4);
    m_menu_rate->addAction(act_5);
    m_btn_rate->setMenu(m_menu_rate);

    m_btn_vlm=new VolumeButton(this);
    m_btn_vlm->setText("volume");

    m_hlayout=new QHBoxLayout;
    m_hlayout->addWidget(btn_video_pre);
    m_hlayout->addWidget(btn_video_pause);
    m_hlayout->addWidget(btn_video_next);
    m_hlayout->addWidget(m_btn_rate);
    m_hlayout->addWidget(m_btn_vlm);

    //实现暂停，倍速播放
    connect(btn_video_pause,&QPushButton::clicked,this,&VideoDisplay::sloPlayerPause);
    connect(m_btn_rate,&QPushButton::clicked,this,&VideoDisplay::sloMenuUnfold);
    connect(act_1,&QAction::triggered,this,[=](){
        this->sloAdjRate(1);
    });
    connect(act_2,&QAction::triggered,this,[=](){
        this->sloAdjRate(2);
    });
    connect(act_3,&QAction::triggered,this,[=](){
        this->sloAdjRate(3);
    });
    connect(act_4,&QAction::triggered,this,[=](){
        this->sloAdjRate(4);
    });
    connect(act_5,&QAction::triggered,this,[=](){
        this->sloAdjRate(5);
    });
    //最后用一个垂直布局将三个板块合并
    m_vlayout=new QVBoxLayout;
    m_vlayout->addWidget(m_video_widget);

    m_vlayout->addLayout(hlayout_slider);
    m_vlayout->addLayout(m_hlayout);
    ui->frame->setLayout(m_vlayout);
    //播放视频
    m_player->setMedia(QUrl::fromLocalFile("/home/hcc/share/my.mkv"));
    connect(m_player,&QMediaPlayer::mediaStatusChanged,this,&VideoDisplay::sloPreload);
    //m_player->play();
    //实现进度条与视频播放的同步
    connect(m_player,&QMediaPlayer::durationChanged,this,&VideoDisplay::sloSetSliderDura);
    connect(m_player,&QMediaPlayer::positionChanged,this,&VideoDisplay::sloSetSliderPos);
    connect(m_slider_video_process,&QSlider::sliderMoved,this,&VideoDisplay::sloPlayerMove);
    //实现音频同步
    connect(m_btn_vlm->getSlider(),&QSlider::sliderMoved,this,[this](int position){
        this->sloVolumeChanged(position);
    });
}
void VideoDisplay::sloSetSliderDura(qint64 dur){
    int val=static_cast<int>(dur);
    m_slider_video_process->setRange(0,val);
    updateDurationTimeLab(dur);
}
void VideoDisplay::sloSetSliderPos(qint64 dur){
    int val=static_cast<int>(dur);
    m_slider_video_process->setValue(val);
    updatePresentTimeLab(dur);
}
void VideoDisplay::sloPlayerMove(int pos){
    m_player->setPosition(pos);
}

QString VideoDisplay::integraTime(qint64 tim){
    //tim是毫秒，先得到秒
    qint64 time_as=tim/1000;
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
void VideoDisplay::updatePresentTimeLab(qint64 tim){
    m_lab_present_time->setText(integraTime(tim));
}
void VideoDisplay::updateDurationTimeLab(qint64 tim){
    m_lab_dur_time->setText(integraTime(tim));
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
}

void VideoDisplay::sloVolumeChanged(int position)
{
    m_player->setVolume(position);

    m_btn_vlm->getLabVlm()->setText(QString::number(position));
}


VideoDisplay::~VideoDisplay()
{
    delete m_hlayout;
    delete m_vlayout;
    delete ui;
}
