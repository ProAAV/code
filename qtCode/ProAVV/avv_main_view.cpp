#include "avv_main_view.h"
#include "ui_avv_main_view.h"
#include<QWidget>
#include<QDebug>
#include"usrpage.h"
#include"videolist.h"
#include"videodisplay.h"
MainView::MainView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainView)
{
    ui->setupUi(this);
    /*
     暂时是提供两个窗口，一个是usrpage,一个是videolist,通过左边导航栏不同按钮可以
     切换不同的窗口，usrpage窗口就是用户窗口，提供用户各种信息，videolist窗口是视
     频列表窗口，程序启动就默认显示这个窗口。
    */
    UsrPage* w1=new UsrPage(this);
    VideoList* w2=new VideoList(this);
    ui->stackedWidget->addWidget(w2);
    ui->stackedWidget->addWidget(w1);

    connect(ui->btn_usr_page,&QPushButton::clicked,ui->stackedWidget,[=](){
        ui->stackedWidget->setCurrentWidget(w1);
    });
    connect(ui->btn_video_list,&QPushButton::clicked,ui->stackedWidget,[=](){
        ui->stackedWidget->setCurrentWidget(w2);
    });

    connect(ui->btn_video_disp,&QPushButton::clicked,ui->stackedWidget,[=](){
        VideoDisplay* vdis=new VideoDisplay();
        vdis->show();
    });
}

MainView::~MainView()
{
    delete ui;
}
