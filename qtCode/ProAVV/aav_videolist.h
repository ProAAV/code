#ifndef VIDEOLIST_H
#define VIDEOLIST_H

#include <QWidget>
#include<QNetworkReply>
#include<aav_videocoverwidget.h>
#include<QVector>
namespace Ui {
class VideoList;
}

class VideoList : public QWidget
{
    Q_OBJECT

public:
    explicit VideoList(QWidget *parent = nullptr);
    ~VideoList();
    void setUserVideoListsInfo();
    void setUserHistoryVideoListsInfo();
private:
    Ui::VideoList *ui;
    VideoCoverWidget* m_widget_1;
    VideoCoverWidget* m_widget_2;
    VideoCoverWidget* m_widget_3;
    VideoCoverWidget* m_widget_4;
    QVector<VideoCoverWidget*> vec_wids;

public slots:
    void sloShowFilesInfo(QNetworkReply* reply);
};

#endif // VIDEOLIST_H
