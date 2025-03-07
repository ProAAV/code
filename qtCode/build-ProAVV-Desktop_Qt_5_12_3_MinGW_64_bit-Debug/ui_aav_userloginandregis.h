/********************************************************************************
** Form generated from reading UI file 'aav_userloginandregis.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AAV_USERLOGINANDREGIS_H
#define UI_AAV_USERLOGINANDREGIS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserLoginAndRegis
{
public:

    void setupUi(QWidget *UserLoginAndRegis)
    {
        if (UserLoginAndRegis->objectName().isEmpty())
            UserLoginAndRegis->setObjectName(QString::fromUtf8("UserLoginAndRegis"));
        UserLoginAndRegis->resize(400, 300);

        retranslateUi(UserLoginAndRegis);

        QMetaObject::connectSlotsByName(UserLoginAndRegis);
    } // setupUi

    void retranslateUi(QWidget *UserLoginAndRegis)
    {
        UserLoginAndRegis->setWindowTitle(QApplication::translate("UserLoginAndRegis", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserLoginAndRegis: public Ui_UserLoginAndRegis {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AAV_USERLOGINANDREGIS_H
