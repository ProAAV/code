#ifndef AAV_VIDEOCOVERWIDLISTLOOP_H
#define AAV_VIDEOCOVERWIDLISTLOOP_H

#include <QWidget>
#include<QLabel>
namespace Ui {
class VideoCoverWidListLoop;
}

class VideoCoverWidListLoop : public QWidget
{
    Q_OBJECT

public:
    explicit VideoCoverWidListLoop(QWidget *parent = nullptr);
    ~VideoCoverWidListLoop();

private:
    Ui::VideoCoverWidListLoop *ui;
    QLabel* lab_cover_img;
};

#endif // AAV_VIDEOCOVERWIDLISTLOOP_H
