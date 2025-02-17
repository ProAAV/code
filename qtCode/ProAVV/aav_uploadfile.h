#ifndef AAV_UPLOADFILE_H
#define AAV_UPLOADFILE_H

#include <QWidget>
#define HOST_IP "192.168.208.128"
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
    explicit UploadFile(QString& file_path,QWidget *parent = nullptr);
    ~UploadFile();
    void captureAnPicture();
    void sendVdoFile(const QString& file_path);
private:
    Ui::UploadFile *ui;
    QString m_file_path;
signals:

};

#endif // AAV_UPLOADFILE_H
