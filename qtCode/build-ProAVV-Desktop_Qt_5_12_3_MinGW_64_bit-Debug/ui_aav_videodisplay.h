/********************************************************************************
** Form generated from reading UI file 'aav_videodisplay.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AAV_VIDEODISPLAY_H
#define UI_AAV_VIDEODISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VideoDisplay
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;

    void setupUi(QWidget *VideoDisplay)
    {
        if (VideoDisplay->objectName().isEmpty())
            VideoDisplay->setObjectName(QString::fromUtf8("VideoDisplay"));
        VideoDisplay->resize(764, 535);
        verticalLayout_2 = new QVBoxLayout(VideoDisplay);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        frame = new QFrame(VideoDisplay);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(frame);

        scrollArea = new QScrollArea(VideoDisplay);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 207, 509));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout->addWidget(scrollArea);

        horizontalLayout->setStretch(0, 5);
        horizontalLayout->setStretch(1, 2);

        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(VideoDisplay);

        QMetaObject::connectSlotsByName(VideoDisplay);
    } // setupUi

    void retranslateUi(QWidget *VideoDisplay)
    {
        VideoDisplay->setWindowTitle(QApplication::translate("VideoDisplay", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VideoDisplay: public Ui_VideoDisplay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AAV_VIDEODISPLAY_H
