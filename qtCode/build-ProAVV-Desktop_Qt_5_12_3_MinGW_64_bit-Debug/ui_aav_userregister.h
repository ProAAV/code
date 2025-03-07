/********************************************************************************
** Form generated from reading UI file 'aav_userregister.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AAV_USERREGISTER_H
#define UI_AAV_USERREGISTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserRegister
{
public:

    void setupUi(QWidget *UserRegister)
    {
        if (UserRegister->objectName().isEmpty())
            UserRegister->setObjectName(QString::fromUtf8("UserRegister"));
        UserRegister->resize(400, 300);

        retranslateUi(UserRegister);

        QMetaObject::connectSlotsByName(UserRegister);
    } // setupUi

    void retranslateUi(QWidget *UserRegister)
    {
        UserRegister->setWindowTitle(QApplication::translate("UserRegister", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserRegister: public Ui_UserRegister {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AAV_USERREGISTER_H
