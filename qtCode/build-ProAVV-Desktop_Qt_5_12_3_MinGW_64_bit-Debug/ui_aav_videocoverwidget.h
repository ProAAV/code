/********************************************************************************
** Form generated from reading UI file 'aav_videocoverwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AAV_VIDEOCOVERWIDGET_H
#define UI_AAV_VIDEOCOVERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VideoCoverWidget
{
public:

    void setupUi(QWidget *VideoCoverWidget)
    {
        if (VideoCoverWidget->objectName().isEmpty())
            VideoCoverWidget->setObjectName(QString::fromUtf8("VideoCoverWidget"));
        VideoCoverWidget->resize(400, 300);

        retranslateUi(VideoCoverWidget);

        QMetaObject::connectSlotsByName(VideoCoverWidget);
    } // setupUi

    void retranslateUi(QWidget *VideoCoverWidget)
    {
        VideoCoverWidget->setWindowTitle(QApplication::translate("VideoCoverWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VideoCoverWidget: public Ui_VideoCoverWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AAV_VIDEOCOVERWIDGET_H
