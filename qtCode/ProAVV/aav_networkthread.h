#ifndef AAV_NETWORKTHREAD_H
#define AAV_NETWORKTHREAD_H

#include <QThread>
#include "aav_networkmanager.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

class NetWorkThread : public QThread
{
public:
    explicit NetWorkThread(QObject *parent = nullptr);
    void run();

    QNetworkReply *http_hhhh();
    NetWorkManager* m_net_manager;
protected:
private:
};
#endif

