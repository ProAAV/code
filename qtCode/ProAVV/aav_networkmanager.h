#ifndef AAV_NETWORKMANAGER_H
#define AAV_NETWORKMANAGER_H
#include<QObject>
#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
class NetWorkManager : public QObject
{
    //貌似对于qt来说它的http的post请求总是请求头和请求体分两次传输
    Q_OBJECT
public:
    explicit NetWorkManager(QObject *parent = nullptr);
    ~NetWorkManager();
    void httpRequest(const QString& method,const QString& url);
    void http_upload_file(const QString& file_path,const QString& url);
    void http_download_file();
private:
    QNetworkAccessManager* m_manager;
signals:
public slots:

};

#endif // AAV_NETWORKMANAGER_H
