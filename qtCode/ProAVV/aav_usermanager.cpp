#include "aav_usermanager.h"
UserManager* UserManager::m_instance = nullptr;

UserManager* UserManager::instance()
{
    if (!m_instance) {
        m_instance = new UserManager();
    }
    return m_instance;
}
UserManager::UserManager(QObject *parent) : QObject(parent), m_user_name(""){

}
QString UserManager::getUserName() const
{
    return m_user_name;
}

void UserManager::setUserName(const QString &user_name)
{
    m_user_name = user_name;
}
