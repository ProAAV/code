#include "aav_lineeditsearch.h"
#include<QStringList>
#include<QCompleter>
#include<QStringListModel>
#include"aav_networkthread.h"
#include"aav_usermanager.h"
#include"aav_networkmanager.h"
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QJsonValue>
#include <QAbstractItemView>
LineEditSearch::LineEditSearch(QWidget *parent):QLineEdit (parent)
{
    QStringListModel* str_list_model=new QStringListModel(this);
    QCompleter* completer=new QCompleter(str_list_model,this);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    this->setCompleter(completer);
    connect(this,&QLineEdit::textChanged,this,[=](const QString& text){
        this->sloHandleTextChanged(completer,str_list_model,text);
    });
}



LineEditSearch::~LineEditSearch()
{
    /*completer->setModel(nullptr);
    setCompleter(nullptr);*/
    qDebug()<<"~LineEditSearch";
}



void LineEditSearch::sloHandleTextChanged(QCompleter* completer,QStringListModel* str_list_model,const QString& str)
{
    qDebug()<<"focusInEvent!!!!!!!!!!!";
    QStringList string_list;

    NetWorkManager net_manager{};
    if(UserManager::instance()->getUserName()==""){
        return;
    }
    qDebug()<<"focusInEvent enter";
    QNetworkReply* reply=net_manager.http_get_search_log(str);

    if(reply->error()){
        qDebug()<<"reply->error()";

        return;
    }
    QByteArray byte_array=reply->readAll();
    qDebug()<<"focusInEvent byte_array:"<<byte_array;
    QJsonDocument json_docm=QJsonDocument::fromJson(byte_array);
    if(json_docm.isNull()){
        qDebug()<<"json document is null";
        return;
    }
    QJsonObject obj_root=json_docm.object();
    QString status=obj_root.value("status").toString();
    if(status=="1"){
        qDebug()<<"http_get_search_log error";
        return;
    }
    int search_log_cnt=obj_root.value("search_log_cnt").toInt();
    qDebug()<<"focusInEvent search_log_cnt:"<<search_log_cnt;
    for(int i=0;i<search_log_cnt;i++){
        QJsonObject sobj=obj_root.value(QString::number(i)).toObject();
        QString file_title=sobj.value("file_title").toString();
        qDebug()<<"focusInEvent file_title:"<<file_title;
        string_list<<file_title;
    }
    qDebug()<<"focusInEvent string_list:"<<string_list;
    str_list_model->setStringList(string_list);
    completer->complete();

}
