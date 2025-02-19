#include "aav_uploadselect.h"
#include "ui_aav_uploadselect.h"
#include<QFileDialog>
#include<QDebug>
UploadSelect::UploadSelect(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UploadSelect)
{
    ui->setupUi(this);
    connect(ui->btn_select_file,&QPushButton::clicked,this,&UploadSelect::sloSelectFile);

}

UploadSelect::~UploadSelect()
{
    delete ui;
}

void UploadSelect::sloSelectFile()
{
    QString video_file_path=QFileDialog::getOpenFileName(this,"选择视频文件上传",QDir::currentPath());
    m_wid_upload_file=new UploadFile(video_file_path);
    this->hide();
    m_wid_upload_file->show();

}
