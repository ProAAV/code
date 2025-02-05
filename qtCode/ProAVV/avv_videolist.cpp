#include "avv_videolist.h"
#include "ui_aav_videolist.h"
#include<QPushButton>
#include<QLineEdit>
#include<QStandardItemModel>
#include<QListView>
#include<QWidget>
VideoList::VideoList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoList)
{
    ui->setupUi(this);
    /*
    视频列表展示类
    */
    QWidget* widget_1=new QWidget(this);
    ui->gridLayout->addWidget(widget_1,0,0);
    widget_1->setStyleSheet("background-color: red;");

    QWidget* widget_2=new QWidget(this);
    ui->gridLayout->addWidget(widget_2,0,1);
    widget_2->setStyleSheet("background-color: green;");

    QWidget* widget_3=new QWidget(this);
    ui->gridLayout->addWidget(widget_3,1,0);
    widget_3->setStyleSheet("background-color: black;");

    QWidget* widget_4=new QWidget(this);
    ui->gridLayout->addWidget(widget_4,1,1);
    widget_4->setStyleSheet("background-color: blue;");

}

VideoList::~VideoList()
{
    delete ui;
}
