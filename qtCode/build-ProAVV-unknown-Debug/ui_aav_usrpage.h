/********************************************************************************
** Form generated from reading UI file 'aav_usrpage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AAV_USRPAGE_H
#define UI_AAV_USRPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UsrPage
{
public:

    void setupUi(QWidget *UsrPage)
    {
        if (UsrPage->objectName().isEmpty())
            UsrPage->setObjectName(QString::fromUtf8("UsrPage"));
        UsrPage->resize(654, 421);

        retranslateUi(UsrPage);

        QMetaObject::connectSlotsByName(UsrPage);
    } // setupUi

    void retranslateUi(QWidget *UsrPage)
    {
        UsrPage->setWindowTitle(QApplication::translate("UsrPage", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UsrPage: public Ui_UsrPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AAV_USRPAGE_H
