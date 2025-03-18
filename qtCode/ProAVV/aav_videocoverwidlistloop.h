#ifndef AAV_VIDEOCOVERWIDLISTLOOP_H
#define AAV_VIDEOCOVERWIDLISTLOOP_H

#include <QWidget>
#include<QLabel>
#include<QNetworkReply>
namespace Ui {
class VideoCoverWidListLoop;
}

class VideoCoverWidListLoop : public QWidget
{
    Q_OBJECT

public:
    explicit VideoCoverWidListLoop(QWidget *parent = nullptr);
    ~VideoCoverWidListLoop();
    void showCover();
public:
    QString m_file_path;
    QString m_file_img_path;
    QString m_file_md5;
    QString m_date_time;
    QString m_duration;
    QString m_intro;
    QString m_progress_data;
    QString m_username;
    QString m_file_type;

private:
    Ui::VideoCoverWidListLoop *ui;
    QLabel* lab_cover_img;
    QLabel* lab_intro;
    QLabel* lab_auth;
    QLabel* la;
protected:
    //bool eventFilter(QObject *obj, QEvent *event)override;
protected:
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void sigPlay();

};

#endif // AAV_VIDEOCOVERWIDLISTLOOP_H
