#include "usrpage.h"
#include "ui_usrpage.h"
#include<QPushButton>
UsrPage::UsrPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsrPage)
{
    ui->setupUi(this);
    /*
    用户界面展示类
    */

}

UsrPage::~UsrPage()
{
    delete ui;
}
