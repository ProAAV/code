/********************************************************************************
** Form generated from reading UI file 'avv_main_view.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AVV_MAIN_VIEW_H
#define UI_AVV_MAIN_VIEW_H

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
    QPushButton *btn_video_disp;
    QStackedWidget *stackedWidget;

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

        btn_video_disp = new QPushButton(MainView);
        btn_video_disp->setObjectName(QString::fromUtf8("btn_video_disp"));

        verticalLayout->addWidget(btn_video_disp);


        horizontalLayout->addLayout(verticalLayout);

        stackedWidget = new QStackedWidget(MainView);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));

        horizontalLayout->addWidget(stackedWidget);


        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(MainView);

        stackedWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainView);
    } // setupUi

    void retranslateUi(QWidget *MainView)
    {
        MainView->setWindowTitle(QApplication::translate("MainView", "MainView", nullptr));
        btn_video_list->setText(QApplication::translate("MainView", "PushButton", nullptr));
        btn_usr_page->setText(QApplication::translate("MainView", "PushButton", nullptr));
        btn_video_disp->setText(QApplication::translate("MainView", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainView: public Ui_MainView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AVV_MAIN_VIEW_H
