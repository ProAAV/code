#ifndef LINEEDITSEARCH_H
#define LINEEDITSEARCH_H
#include<QLineEdit>
#include<QNetworkReply>
#include<QStringListModel>
#include<QCompleter>
class LineEditSearch : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEditSearch(QWidget *parent = nullptr);
    ~LineEditSearch();
protected:

public slots:
    void sloHandleTextChanged(QCompleter* completer,QStringListModel* str_list_model,const QString& str);
private:
    /*QCompleter* completer;
    QStringListModel* str_list_model;*/

};

#endif // LINEEDITSEARCH_H
