#ifndef USRPAGE_H
#define USRPAGE_H

#include <QWidget>
#include<QStackedWidget>
#include<QWidget>
#include<QPushButton>
#include<QLabel>
#include"aav_videolist.h"

namespace Ui {
class UsrPage;
}

class UsrPage : public QWidget
{
    Q_OBJECT

public:
    explicit UsrPage(QWidget *parent = nullptr);
    ~UsrPage();
    void setUserInfo();
    QLabel* lab_username;
    QLabel* lab_usernickname;
private:
    Ui::UsrPage *ui;
    QStackedWidget* m_stack_wid;
    QWidget* m_user_info_wid;
    QPushButton* m_btn_videos;
    QPushButton* m_btn_history;
    VideoList* m_video_lists_wid;
    VideoList* m_video_lists_history_wid;

    QPushButton* m_btn_logout;


protected:
    void showEvent(QShowEvent *event) override;
};

#endif // USRPAGE_H
