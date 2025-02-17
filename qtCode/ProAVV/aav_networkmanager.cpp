#include "aav_networkmanager.h"
#include<QDebug>
#include<QUrl>
#include<QHttpMultiPart>
#include<QFile>
#include<QFileInfo>
#include<QEventLoop>
NetWorkManager::NetWorkManager(QObject *parent): QObject{parent}
{
    m_manager=new QNetworkAccessManager(this);


}

NetWorkManager::~NetWorkManager()
{

}

void NetWorkManager::httpRequest(const QString& method,const QString& url)
{


}

void NetWorkManager::http_upload_file(const QString& file_path,const QString &url)
{
    //nginx_upload_module对于文件上传时http请求头的content-type有着严格的约束，必须指定boundary
    qDebug()<<"enter http_upload_file";


    // 创建网络管理器
    QNetworkAccessManager networkManager;

    // 创建一个多部分对象
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    const QByteArray boundary = multiPart->boundary();

    // 创建一个文件部分
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"" + QFileInfo(file_path).fileName() + "\""));
    QFile *file = new QFile(file_path);
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件：" << file_path;
        return;
    }
    filePart.setBodyDevice(file);
    file->setParent(multiPart); // 确保文件对象在多部分对象销毁时不会泄漏

    // 将文件部分添加到多部分对象中
    multiPart->append(filePart);

    // 创建请求
    QNetworkRequest request(url);

    // 设置 Content-Type 头为 multipart/form-data
    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data;boundary=" + boundary);

    // 发起 POST 请求
    QNetworkReply *reply = networkManager.post(request, multiPart);
    multiPart->setParent(reply); // 确保多部分对象在响应销毁时不会泄漏

    // 使用事件循环等待响应
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished,&loop,&QEventLoop::quit);
    loop.exec();

    // 处理响应
    if (reply->error()) {
        qDebug() << "上传失败：" << reply->errorString();
    } else {
    qDebug() << "上传成功，服务器响应：" << reply->readAll();
    }

    reply->deleteLater();


}
