#include "aav_networkthread.h"
#include<QDebug>


NetWorkThread::NetWorkThread(QObject *parent):QThread (parent)
{
    m_net_manager=new NetWorkManager(this);
}

void NetWorkThread::run()
{
    exec();
}

QNetworkReply *NetWorkThread::http_hhhh()
{

    QString s="http://192.168.208.128:8888/api/userinfo";
    qDebug()<<"http_get_user_info:"<<s;
    QUrl url(s);
    QNetworkRequest request(url);
    QNetworkAccessManager* man=new QNetworkAccessManager(this);
    QNetworkReply* reply=man->get(request);
    qDebug()<<"66666666666六百六十六";
    return reply;
}
