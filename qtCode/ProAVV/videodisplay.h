#ifndef VIDEODISPLAY_H
#define VIDEODISPLAY_H

#include <QWidget>

namespace Ui {
class VideoDisplay;
}

class VideoDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit VideoDisplay(QWidget *parent = nullptr);
    ~VideoDisplay();

private:
    Ui::VideoDisplay *ui;
};

#endif // VIDEODISPLAY_H
