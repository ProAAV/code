/********************************************************************************
** Form generated from reading UI file 'aav_videolist.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AAV_VIDEOLIST_H
#define UI_AAV_VIDEOLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VideoList
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout;

    void setupUi(QWidget *VideoList)
    {
        if (VideoList->objectName().isEmpty())
            VideoList->setObjectName(QString::fromUtf8("VideoList"));
        VideoList->resize(626, 426);
        verticalLayout_2 = new QVBoxLayout(VideoList);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(VideoList);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 600, 400));
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(VideoList);

        QMetaObject::connectSlotsByName(VideoList);
    } // setupUi

    void retranslateUi(QWidget *VideoList)
    {
        VideoList->setWindowTitle(QApplication::translate("VideoList", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VideoList: public Ui_VideoList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AAV_VIDEOLIST_H
