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
    MainView w;
    w.resize(1300,960);
    w.show();
    return a.exec();
}
