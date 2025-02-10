#ifndef AAV_UPLOADFILE_H
#define AAV_UPLOADFILE_H

#include <QWidget>

namespace Ui {
class UploadFile;
}

class UploadFile : public QWidget
{
    Q_OBJECT
    /*
     * 这是当用户点击uploadselect中的select_file按钮后弹出的文件上传窗口
     */
public:
    explicit UploadFile(QString file_path,QWidget *parent = nullptr);
    ~UploadFile();
    void captureAnPicture();
private:
    Ui::UploadFile *ui;
    QString m_file_path;
signals:

};

#endif // AAV_UPLOADFILE_H
