#ifndef AAV_WIDGETVLUME_H
#define AAV_WIDGETVLUME_H
#include<QWidget>
class WidgetVlume : public QWidget
{
    /*这是VolumeButton按钮点击后弹出的音量滑块的背景类，这个背景包含一个lab来显示音量值和具体的
    slider控件*/
    /*也可以用来做倍速的布局背景*/
    Q_OBJECT
public:
    explicit WidgetVlume(QWidget *parent = nullptr);
    ~WidgetVlume() override;
protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
signals:
    void sigEnter();
};

#endif // AAV_WIDGETVLUME_H
