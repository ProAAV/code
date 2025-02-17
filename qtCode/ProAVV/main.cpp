#include "aav_main_view.h"
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QNetworkAccessManager>
#include<QByteArray>
#include<QVariantList>
#include<QFile>
#include<QFileInfo>
#include<QHttpMultiPart>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

        /*QString filePath = "/home/hcc/share/test.txt"; // 替换为你的文件路径
        QUrl url("http://192.168.208.128:8888/api/upload"); // 替换为目标 URL

        // 创建网络管理器
        QNetworkAccessManager networkManager;

        // 创建一个多部分对象
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        const QByteArray boundary = multiPart->boundary();

        // 创建一个文件部分
        QHttpPart filePart;
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"" + QFileInfo(filePath).fileName() + "\""));
        QFile *file = new QFile(filePath);
        if (!file->open(QIODevice::ReadOnly)) {
            qDebug() << "无法打开文件：" << filePath;
            return -1;
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

        // 连接信号和槽处理响应
        QObject::connect(reply, &QNetworkReply::finished, [&]() {
            if (reply->error()) {
                qDebug() << "上传失败：" << reply->errorString();
            } else {
                qDebug() << "上传成功，服务器响应：" << reply->readAll();
            }
            reply->deleteLater();
            a.quit(); // 退出事件循环
        });*/


    MainView w;
    w.show();
    return a.exec();
}
