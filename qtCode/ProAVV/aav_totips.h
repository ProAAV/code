#ifndef AAV_TOTIPS_H
#define AAV_TOTIPS_H

#include <QDialog>
#include<QLabel>
namespace Ui {
class ToTips;
}

class ToTips : public QDialog
{
    Q_OBJECT

public:
    explicit ToTips(QWidget *parent = nullptr);
    ~ToTips();
    QLabel *getLab();
private:
    Ui::ToTips *ui;
    QLabel* m_lab_show_tips;
};

#endif // AAV_TOTIPS_H
