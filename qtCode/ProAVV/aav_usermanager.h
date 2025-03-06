#ifndef AAV_USERMANAGER_H
#define AAV_USERMANAGER_H

#include <QObject>

class UserManager:public QObject
{
    Q_OBJECT
public:
    static UserManager *instance();
    QString getUserName() const;
    void setUserName(const QString &user_id);
private:
    UserManager(QObject *parent = nullptr);
    static UserManager *m_instance;
    QString m_user_name;
};

#endif // AAV_USERMANAGER_H
