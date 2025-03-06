#ifndef AAV_USERREGISTER_H
#define AAV_USERREGISTER_H

#include <QWidget>
#include<QLineEdit>
#include<QHBoxLayout>
#include<QVBoxLayout>
namespace Ui {
class UserRegister;
}

class UserRegister : public QWidget
{
    Q_OBJECT

public:
    explicit UserRegister(QWidget *parent = nullptr);
    ~UserRegister();
    void handleRegister();
private:
    Ui::UserRegister *ui;
    QLineEdit* m_ledit_account;
    QLineEdit* m_ledit_password;
    QLineEdit* m_ledit_nickname;
    QLineEdit* m_ledit_repassword;
signals:
    void sigRegisterSuccess();
    void sigRegisterFailed();
};

#endif // AAV_USERREGISTER_H
