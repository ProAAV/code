#ifndef AAV_UPLOADSELECT_H
#define AAV_UPLOADSELECT_H

#include <QWidget>
#include"aav_uploadfile.h"
namespace Ui {
class UploadSelect;
}

class UploadSelect : public QWidget
{
    Q_OBJECT
    /*
     * 这是当用户点击main_view中的upload按钮时显示的文件选择窗口
     *
    */
public:
    explicit UploadSelect(QWidget *parent = nullptr);
    ~UploadSelect();

private:
    Ui::UploadSelect *ui;
    UploadFile* m_wid_upload_file;
public slots:
    void sloSelectFile();
};

#endif // AAV_UPLOADSELECT_H
