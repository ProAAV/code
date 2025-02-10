/********************************************************************************
** Form generated from reading UI file 'aav_main_view.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AAV_MAIN_VIEW_H
#define UI_AAV_MAIN_VIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainView
{
public:
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *btn_video_list;
    QPushButton *btn_usr_page;
    QPushButton *btn_upload;
    QPushButton *btn_video_disp;
    QStackedWidget *stackw;

    void setupUi(QWidget *MainView)
    {
        if (MainView->objectName().isEmpty())
            MainView->setObjectName(QString::fromUtf8("MainView"));
        MainView->resize(800, 600);
        horizontalLayout_2 = new QHBoxLayout(MainView);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        btn_video_list = new QPushButton(MainView);
        btn_video_list->setObjectName(QString::fromUtf8("btn_video_list"));

        verticalLayout->addWidget(btn_video_list);

        btn_usr_page = new QPushButton(MainView);
        btn_usr_page->setObjectName(QString::fromUtf8("btn_usr_page"));

        verticalLayout->addWidget(btn_usr_page);

        btn_upload = new QPushButton(MainView);
        btn_upload->setObjectName(QString::fromUtf8("btn_upload"));

        verticalLayout->addWidget(btn_upload);

        btn_video_disp = new QPushButton(MainView);
        btn_video_disp->setObjectName(QString::fromUtf8("btn_video_disp"));

        verticalLayout->addWidget(btn_video_disp);


        horizontalLayout->addLayout(verticalLayout);

        stackw = new QStackedWidget(MainView);
        stackw->setObjectName(QString::fromUtf8("stackw"));

        horizontalLayout->addWidget(stackw);


        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(MainView);

        stackw->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainView);
    } // setupUi

    void retranslateUi(QWidget *MainView)
    {
        MainView->setWindowTitle(QApplication::translate("MainView", "MainView", nullptr));
        btn_video_list->setText(QApplication::translate("MainView", "PushButton", nullptr));
        btn_usr_page->setText(QApplication::translate("MainView", "PushButton", nullptr));
        btn_upload->setText(QApplication::translate("MainView", "PushButton", nullptr));
        btn_video_disp->setText(QApplication::translate("MainView", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainView: public Ui_MainView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AAV_MAIN_VIEW_H
