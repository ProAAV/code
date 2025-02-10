/********************************************************************************
** Form generated from reading UI file 'aav_uploadselect.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AAV_UPLOADSELECT_H
#define UI_AAV_UPLOADSELECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UploadSelect
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *btn_select_file;

    void setupUi(QWidget *UploadSelect)
    {
        if (UploadSelect->objectName().isEmpty())
            UploadSelect->setObjectName(QString::fromUtf8("UploadSelect"));
        UploadSelect->resize(400, 300);
        verticalLayout = new QVBoxLayout(UploadSelect);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        btn_select_file = new QPushButton(UploadSelect);
        btn_select_file->setObjectName(QString::fromUtf8("btn_select_file"));

        verticalLayout->addWidget(btn_select_file);


        retranslateUi(UploadSelect);

        QMetaObject::connectSlotsByName(UploadSelect);
    } // setupUi

    void retranslateUi(QWidget *UploadSelect)
    {
        UploadSelect->setWindowTitle(QApplication::translate("UploadSelect", "Form", nullptr));
        btn_select_file->setText(QApplication::translate("UploadSelect", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UploadSelect: public Ui_UploadSelect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AAV_UPLOADSELECT_H
