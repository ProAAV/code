#ifndef VIDEOLIST_H
#define VIDEOLIST_H

#include <QWidget>

namespace Ui {
class VideoList;
}

class VideoList : public QWidget
{
    Q_OBJECT

public:
    explicit VideoList(QWidget *parent = nullptr);
    ~VideoList();

private:
    Ui::VideoList *ui;
};

#endif // VIDEOLIST_H
