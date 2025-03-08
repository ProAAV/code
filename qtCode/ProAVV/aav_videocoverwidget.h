#ifndef AAV_VIDEOCOVERWIDGET_H
#define AAV_VIDEOCOVERWIDGET_H

#include <QWidget>
#include<QLabel>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include"aav_videodisplay.h"
namespace Ui {
class VideoCoverWidget;
}

class VideoCoverWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VideoCoverWidget(QWidget *parent = nullptr);
    ~VideoCoverWidget();


    QString m_file_path;
    QString m_file_img_path;
    QString m_file_md5;
    QString m_date_time;
    QString m_duration;
    QString m_intro;
    QString m_progress_data;

    Ui::VideoCoverWidget *ui;
    QLabel* m_lab_img;
    QLabel* m_lab_duration;
    QLabel* m_lab_upload_date;
    QLabel* m_lab_intro;
    QLabel* m_lab_auth;
    QHBoxLayout* m_hlayout;
    QVBoxLayout* m_vlayout;
    VideoDisplay* m_vdis;
protected:
    bool eventFilter(QObject *obj, QEvent *event)override;
signals:
    void sigRequestImg(QString& file_img_path);
public slots:
    void sloRequestImg(QString& file_img_path);
    void sloCloseDisplayer();

};

#endif // AAV_VIDEOCOVERWIDGET_H
