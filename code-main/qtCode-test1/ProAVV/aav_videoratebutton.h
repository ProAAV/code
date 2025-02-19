#ifndef AAV_VIDEORATEBUTTON_H
#define AAV_VIDEORATEBUTTON_H
#include<QSlider>
#include<QVBoxLayout>
#include<QEvent>
#include<QPushButton>
#include"aav_widgetvlume.h"
#include<QLabel>
#include<QTimer>
class Videoratebutton:public QPushButton
{
public:
   Videoratebutton(QWidget *parent);
   ~Videoratebutton();

   QSlider* getSlider();
    QSlider* m_sli_rate;
    QLabel* m_lab_rate;
protected:
   QLabel* getLabVlm();
private:
   WidgetVlume* m_wid_rate;
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

#endif // AAV_VIDEORATEBUTTON_H
