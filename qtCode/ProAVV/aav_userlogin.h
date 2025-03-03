#ifndef AAV_USERLOGIN_H
#define AAV_USERLOGIN_H

#include <QWidget>
#include<QLineEdit>
#include<QString>
namespace Ui {
class UserLogin;
}

class UserLogin : public QWidget
{
    Q_OBJECT

public:
    explicit UserLogin(QWidget *parent = nullptr);
    ~UserLogin();

private:
    Ui::UserLogin *ui;
    QLineEdit* m_ledit_account;
    QLineEdit* m_ledit_password;
private slots:
    void sloHandleLogin();

signals:
    void sigLoginSuccess();
    void sigLoginFailed();
};

#endif // AAV_USERLOGIN_H
