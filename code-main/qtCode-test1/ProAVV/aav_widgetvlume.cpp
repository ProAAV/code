#include "aav_widgetvlume.h"
#include"aav_videodisplay.h"
WidgetVlume::WidgetVlume(QWidget *parent): QWidget{parent}
{
    /*connect(this,&WidgetVlume::sigEnter,this->parent(),[this](){
        this->parent()->slo
    });*/
}

WidgetVlume::~WidgetVlume()
{

}

void WidgetVlume::enterEvent(QEvent *event)
{
    this->show();
    sigEnter();
    
    QWidget::enterEvent(event);
}

void WidgetVlume::leaveEvent(QEvent *event)
{
    this->hide();
    QWidget::leaveEvent(event);
}
