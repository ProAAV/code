#ifndef AAV_NETWORKMANAGER_H
#define AAV_NETWORKMANAGER_H
#include<QObject>
#include<QLabel>
#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QJsonDocument>
#include<QBuffer>
#include"aav_usermanager.h"
#include"aav_videolist.h"
#include<QStringList>
#include<QStringListModel>
#include<QCompleter>
#include"aav_lineeditsearch.h"
class NetWorkManager : public QObject
{
    //貌似对于qt来说它的http的post请求总是请求头和请求体分两次传输
    Q_OBJECT
public:
    explicit NetWorkManager(QObject *parent = nullptr);
    ~NetWorkManager();
    void httpRequest(const QString& method,const QString& url);
    void http_upload_file(double file_playback_duration,QString& file_title,const QString& file_path,const QString& url);
    QBuffer* http_download_file();
    QBuffer *getBuffer();
    QNetworkReply* http_get_files_info();
    QBuffer* m_buffer;
    QNetworkReply* getFilesInfoReply();
    QNetworkReply *http_get_img_cover(QString& file_img_path);
    bool http_login(const QString& account,const QString& password);
    bool http_register(const QString& account,const QString& password,const QString& nickname,const QString& repassword);
    //改造完成
    void http_get_user_video_lists_info(int opt,VideoList* video_list);
    void http_get_search_video_lists_info(VideoList* video_list,const QString& key);
    void http_insert_user_history_log(QString& username,QString& file_md5,int progress_);
    //完成
    void http_get_user_info(QLabel* lab1,QLabel* lab2);
    void http_insert_search_log(QString& search_key);
    //自动补全
    QNetworkReply* http_get_search_log(const QString& key);
private:
    QNetworkAccessManager* m_manager;
    QNetworkReply* m_files_info_reply;
signals:
public slots:
    QBuffer* sloHandleDownloadData(QNetworkReply* reply);
    QBuffer* sloHandleVideosInfo(QNetworkReply* reply);
    void onGetUserInfo(QLabel* lab1,QLabel* lab2,QNetworkReply* reply);
};

#endif // AAV_NETWORKMANAGER_H
