#ifndef AAV_WIDGETVLUME_H
#define AAV_WIDGETVLUME_H
#include<QWidget>
class WidgetVlume : public QWidget
{
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
