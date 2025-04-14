#ifndef AVV_MAIN_VIEW_H
#define AVV_MAIN_VIEW_H

#include <QWidget>
#include"aav_uploadselect.h"
#include<QNetworkReply>
#include"aav_userloginandregis.h"
#include"aav_usrpage.h"
#include"aav_lineeditsearch.h"
#include<QApplication>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainView;
}
QT_END_NAMESPACE

class MainView : public QWidget
{
    Q_OBJECT

public:
    MainView(QApplication* app,QWidget *parent = nullptr);
    ~MainView();
    QMenu* menu_set;
    QAction* act_theme_01;
    QAction* act_theme_02;
private:
    Ui::MainView *ui;
    UploadSelect* m_wid_upload_select;
    UserLoginAndRegis* m_login_and_regis_wid;
    UsrPage* m_userpage;
    LineEditSearch* m_ledit_search;
    VideoList* m_search_video_lists;
    QStackedWidget* stack_video_page_wid;
    bool status_is_search;
    QApplication* m_app;


signals:
    void sigFilesInfo(QNetworkReply* reply);
public slots:
    void sloBtnUserPageHandle();
    void sloUserLoginSuccess();
    void sloUserLoginSuccess2();
    void sloBtnSearch();
    void sloSetTheme01();
    void sloSetTheme02();
};
#endif // AVV_MAIN_VIEW_H
