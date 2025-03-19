/********************************************************************************
** Form generated from reading UI file 'aav_main_view.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AAV_MAIN_VIEW_H
#define UI_AAV_MAIN_VIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
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
    QSpacerItem *verticalSpacer_2;
    QPushButton *btn_video_list;
    QSpacerItem *verticalSpacer_3;
    QPushButton *btn_usr_page;
    QSpacerItem *verticalSpacer_4;
    QPushButton *btn_upload;
    QSpacerItem *verticalSpacer;
    QPushButton *btn_set;
    QSpacerItem *verticalSpacer_5;
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
        verticalSpacer_2 = new QSpacerItem(20, 120, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer_2);

        btn_video_list = new QPushButton(MainView);
        btn_video_list->setObjectName(QString::fromUtf8("btn_video_list"));

        verticalLayout->addWidget(btn_video_list);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer_3);

        btn_usr_page = new QPushButton(MainView);
        btn_usr_page->setObjectName(QString::fromUtf8("btn_usr_page"));

        verticalLayout->addWidget(btn_usr_page);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer_4);

        btn_upload = new QPushButton(MainView);
        btn_upload->setObjectName(QString::fromUtf8("btn_upload"));

        verticalLayout->addWidget(btn_upload);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        btn_set = new QPushButton(MainView);
        btn_set->setObjectName(QString::fromUtf8("btn_set"));

        verticalLayout->addWidget(btn_set);

        verticalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer_5);


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
        btn_video_list->setText(QApplication::translate("MainView", "home", nullptr));
        btn_usr_page->setText(QApplication::translate("MainView", "\347\224\250\346\210\267", nullptr));
        btn_upload->setText(QApplication::translate("MainView", "\344\270\212\344\274\240", nullptr));
        btn_set->setText(QApplication::translate("MainView", "\350\256\276\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainView: public Ui_MainView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AAV_MAIN_VIEW_H
