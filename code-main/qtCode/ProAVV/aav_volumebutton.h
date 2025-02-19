#ifndef AAV_VOLUMEBUTTON_H
#define AAV_VOLUMEBUTTON_H

#include <QPushButton>
#include<QSlider>
#include<QLabel>
#include"aav_widgetvlume.h"
#include<QTimer>
class VolumeButton : public QPushButton
{
    //这是VideoDisplay界面中的调节音量按钮的按钮类
    Q_OBJECT
public:
    explicit VolumeButton(QWidget *parent = nullptr);
    ~VolumeButton() override;
    QSlider* getSlider();
    QLabel* getLabVlm();
private:
    QSlider* m_sli_vlm;
    WidgetVlume* m_wid_vlm;
    QLabel* m_lab_vlm;
    QTimer* m_tm;
protected:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent *event) override;
public slots:
    void sloTimeOut();
    void sloTimeOutStop();
signals:
    void sigEn();
};

#endif // AAV_VOLUMEBUTTON_H
