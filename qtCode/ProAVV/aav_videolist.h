#ifndef VIDEOLIST_H
#define VIDEOLIST_H
#include <QWidget>

#include<QNetworkReply>


#include<aav_videocoverwidget.h>


#include<QVector>

namespace Ui {
class VideoList;
}
class NetWorkThread;
class VideoList : public QWidget
{
    Q_OBJECT

public:
    explicit VideoList(int flag,QWidget *parent = nullptr);
    ~VideoList();
    void setUserVideoListsInfo();
    void setUserHistoryVideoListsInfo();
    void getNewVideoListsInfo(VideoList* list);
    VideoCoverWidget* addVideoCoverWidget(int x,int y);
    void reload();
private:
    Ui::VideoList *ui;
    QVector<VideoCoverWidget*> m_vec_video_cover_wids;

    int m_x;
    int m_y;

    int m_flag;

    int m_idx;
    int m_offset;
    NetWorkThread* m_thread;

public slots:
    void sloShowFilesInfo(QNetworkReply* reply);
    void sloHandleScrollBarValueChanged(int value);
};

#endif // VIDEOLIST_H
