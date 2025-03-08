#ifndef AAV_NETWORKTHREAD_H
#define AAV_NETWORKTHREAD_H

#include<QThread>
#include"aav_networkmanager.h"
#include<QNetworkAccessManager>
#include<QNetworkReply>
class NetWorkThread : public QThread
{
public:
    explicit NetWorkThread(QObject *parent = nullptr);
    void run();
    NetWorkManager* m_net_manager;
    QNetworkReply *http_hhhh();
protected:

private:


};
#endif // AAV_NETWORKTHREAD_H
