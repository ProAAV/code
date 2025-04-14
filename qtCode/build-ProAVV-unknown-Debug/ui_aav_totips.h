/********************************************************************************
** Form generated from reading UI file 'aav_totips.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AAV_TOTIPS_H
#define UI_AAV_TOTIPS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_ToTips
{
public:

    void setupUi(QDialog *ToTips)
    {
        if (ToTips->objectName().isEmpty())
            ToTips->setObjectName(QString::fromUtf8("ToTips"));
        ToTips->resize(400, 300);

        retranslateUi(ToTips);

        QMetaObject::connectSlotsByName(ToTips);
    } // setupUi

    void retranslateUi(QDialog *ToTips)
    {
        ToTips->setWindowTitle(QApplication::translate("ToTips", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ToTips: public Ui_ToTips {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AAV_TOTIPS_H
