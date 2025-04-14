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
#include<QFile>
#include<QAction>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*QFile file(":/01_theme.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        a.setStyleSheet(styleSheet);
        file.close();
    }*/
    MainView w(&a);

    w.setObjectName("main_window");
    w.resize(1300,960);
    w.show();
    return a.exec();
}
