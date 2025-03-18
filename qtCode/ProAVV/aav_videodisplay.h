#ifndef VIDEODISPLAY_H
#define VIDEODISPLAY_H
#include<QHBoxLayout>
#include<QVBoxLayout>
#include <QWidget>
#include<QSlider>
#include<QMediaPlayer>
#include<QLabel>
#include<QPushButton>
#include"aav_volumebutton.h"
#include"aav_videocoverwidlistloop.h"
#include<QNetworkReply>
#include<bitset>
#include<QPropertyAnimation>
namespace Ui {
class VideoDisplay;
}
const int N=100;
class VideoDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit VideoDisplay(QString& file_md5,QString& file_type,QWidget *parent = nullptr);
    ~VideoDisplay() override;
    void updatePresentTimeLab(qint64 tim);
    void updateDurationTimeLab(qint64 tim);
    static QString integraTime(qint64 tim,int opt);
    void setVideoFilePath(QString& file_path);
    void setVideoFileProgressData(qint64 progress);
    void play(const QString& file_path,const QString& file_md5);
    void listLoopCoverShowInfo(QNetworkReply* reply);
    void createDanmu(const QString &text, int duration);
    void analyzeDanmuMessages(QNetworkReply* reply);
    int timeToSeconds(const QString &qtTimeStr);
    void parseMessages(QString& message);
    void checkDanmuPresentTime(int second);
private:
    Ui::VideoDisplay *ui;
    QVBoxLayout* m_vlayout;
    QHBoxLayout* m_hlayout;
    QSlider* m_slider_video_process;

    QMediaPlayer* m_player;
    QVideoWidget* m_video_widget;

    QLabel* m_lab_present_time;
    QLabel* m_lab_dur_time;
    qint64 m_cnt_pause_player;
    QPushButton* m_btn_rate;
    QMenu* m_menu_rate;
    QMenu* m_menu_mode;
    VolumeButton* m_btn_vlm;
    QString m_file_path;
    QString m_file_md5;
    QString m_file_type;

    QVector<VideoCoverWidListLoop*> vec_list_loop_wids;

    QWidget* list_loop_wid;
    QVBoxLayout* vlayout_list_loop_wid;

    size_t m_ptr;
    std::bitset<N> vis;

    QVector<QString> vec_messages;
    QVector<int> vec_start_time;
    QVector<int> vec_end_time;
    QVector<QString> vec_descriptions;
    QPropertyAnimation *animation;
    QLabel *danmuLabel;
public slots:
    void sloSetSliderDura(qint64 dur);
    void sloSetSliderPos(qint64 dur);
    void sloPlayerMove(int pos);
    void sloPlayerPause();
    void sloMenuUnfold();
    void sloAdjRate(int num);
    void sloPreload(QMediaPlayer::MediaStatus status);
    void sloVolumeChanged(int position);
    Ui::VideoDisplay *getVideoDisplayUi();
    void sloHandleBtnNext();
protected:
    void closeEvent(QCloseEvent *event) override;
signals:
    void sigClose();

};

#endif // VIDEODISPLAY_H
