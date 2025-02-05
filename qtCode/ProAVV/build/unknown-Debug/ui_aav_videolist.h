/********************************************************************************
** Form generated from reading UI file 'aav_videolist.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AAV_VIDEOLIST_H
#define UI_AAV_VIDEOLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VideoList
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
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
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEdit = new QLineEdit(VideoList);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        pushButton = new QPushButton(VideoList);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);

        scrollArea = new QScrollArea(VideoList);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 610, 382));
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
        VideoList->setWindowTitle(QCoreApplication::translate("VideoList", "Form", nullptr));
        pushButton->setText(QCoreApplication::translate("VideoList", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VideoList: public Ui_VideoList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AAV_VIDEOLIST_H
