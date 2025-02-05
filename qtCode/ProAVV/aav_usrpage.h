#ifndef USRPAGE_H
#define USRPAGE_H

#include <QWidget>

namespace Ui {
class UsrPage;
}

class UsrPage : public QWidget
{
    Q_OBJECT

public:
    explicit UsrPage(QWidget *parent = nullptr);
    ~UsrPage();

private:
    Ui::UsrPage *ui;
};

#endif // USRPAGE_H
