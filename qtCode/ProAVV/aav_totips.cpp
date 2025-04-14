#include "aav_totips.h"
#include "ui_aav_totips.h"
#include<QHBoxLayout>
ToTips::ToTips(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ToTips)
{
    ui->setupUi(this);
    m_lab_show_tips=new QLabel(this);
    QHBoxLayout* hlayout=new QHBoxLayout(this);
    hlayout->setContentsMargins(0, 0, 0, 0);
    this->resize(300,50);
    hlayout->addWidget(m_lab_show_tips);
    setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground);

}

ToTips::~ToTips()
{
    delete ui;
}

QLabel *ToTips::getLab()
{
    return m_lab_show_tips;
}
