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
namespace Ui {
class VideoDisplay;
}

class VideoDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit VideoDisplay(QWidget *parent = nullptr);
    ~VideoDisplay();
    void updatePresentTimeLab(qint64 tim);
    void updateDurationTimeLab(qint64 tim);
    QString integraTime(qint64 tim);
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
    VolumeButton* m_btn_vlm;
public slots:
    void sloSetSliderDura(qint64 dur);
    void sloSetSliderPos(qint64 dur);
    void sloPlayerMove(int pos);
    void sloPlayerPause();
    void sloMenuUnfold();
    void sloAdjRate(int num);
    void sloPreload(QMediaPlayer::MediaStatus status);
    void sloVolumeChanged(int position);

};

#endif // VIDEODISPLAY_H
