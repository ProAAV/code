#ifndef AVV_MAIN_VIEW_H
#define AVV_MAIN_VIEW_H

#include <QWidget>
#include"aav_uploadselect.h"
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

};
#endif // AVV_MAIN_VIEW_H
