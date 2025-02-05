#ifndef VIDEODISPLAY_H
#define VIDEODISPLAY_H
#include<QHBoxLayout>
#include<QVBoxLayout>
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
    QVBoxLayout* m_vlayout;
    QHBoxLayout* m_hlayout;
};

#endif // VIDEODISPLAY_H
