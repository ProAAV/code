#ifndef AVV_MAIN_VIEW_H
#define AVV_MAIN_VIEW_H

#include <QWidget>
#include"aav_uploadselect.h"
#include<QNetworkReply>
#include"aav_userloginandregis.h"
#include"aav_usrpage.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainView;
}
QT_END_NAMESPACE

class MainView : public QWidget
{
    Q_OBJECT

public:
    MainView(QWidget *parent = nullptr);
    ~MainView();

private:
    Ui::MainView *ui;
    UploadSelect* m_wid_upload_select;
    UserLoginAndRegis* m_login_and_regis_wid;
    UsrPage* m_userpage;
signals:
    void sigFilesInfo(QNetworkReply* reply);
public slots:
    void sloBtnUserPageHandle();
    void sloUserLoginSuccess();

};
#endif // AVV_MAIN_VIEW_H
