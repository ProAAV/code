#include "aav_networkmanager.h"
#include<QDebug>
#include<QUrl>
#include<QHttpMultiPart>
#include<QFile>
#include<QFileInfo>
#include<QMimeDatabase>
#include<QEventLoop>
#include<QFile>
#include<QBuffer>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QTimer>
#include<QJsonValue>

NetWorkManager::NetWorkManager(QObject *parent): QObject{parent}
{
    m_manager=new QNetworkAccessManager(this);
    m_buffer=nullptr;
    m_files_info_reply=nullptr;
}

NetWorkManager::~NetWorkManager()
{

}

void NetWorkManager::httpRequest(const QString& method,const QString& url)
{


}

void NetWorkManager::http_upload_file(double file_playback_duration,QString& file_title,const QString& file_path,const QString &url)
{
    //nginx_upload_module对于文件上传时http请求头的content-type有着严格的约束，必须指定boundary
    qDebug()<<"enter http_upload_file";


    // 创建网络管理器


    // 创建一个多部分对象
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    const QByteArray boundary = multiPart->boundary();

    // 创建一个文件部分
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"file\"; filename=\"" + QFileInfo(file_path).fileName() + "\""));
    QFile *file = new QFile(file_path);
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件：" << file_path;
        return;
    }
    filePart.setBodyDevice(file);
    file->setParent(multiPart); // 确保文件对象在多部分对象销毁时不会泄漏

    // 将文件部分添加到多部分对象中
    multiPart->append(filePart);

    // 获取文件的 MIME 类型
    QMimeDatabase mimeDatabase;
    QMimeType mimeType = mimeDatabase.mimeTypeForFile(file_path);
    QString contentType = mimeType.isValid() ? mimeType.name() : "application/octet-stream";

    // 创建一个 `file_content_type` 字段部分
    QHttpPart contentTypePart;
    contentTypePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file_content_type\""));
    contentTypePart.setBody(contentType.toUtf8());

    // 将 `file_content_type` 字段添加到多部分对象中
    multiPart->append(contentTypePart);

    // 创建一个 `username` 字段部分
    QHttpPart userNamePart;
    userNamePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"username\""));
    QString username=UserManager::instance()->getUserName();
    qDebug()<<"uuusername:"<<username;
    userNamePart.setBody(username.toUtf8().data());

    // 将 `username` 字段添加到多部分对象中
    multiPart->append(userNamePart);


    // 创建一个 `file_title` 字段部分
    QHttpPart fileTitlePart;
    fileTitlePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file_title\""));
    fileTitlePart.setBody(file_title.toUtf8().data());

    // 将 `file_title` 字段添加到多部分对象中
    multiPart->append(fileTitlePart);



    // 创建一个 `file_playback_duration` 字段部分
    QHttpPart filePlayBackDurationPart;
    filePlayBackDurationPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file_playback_duration\""));
    QString s_duration=QString::number(file_playback_duration);
    filePlayBackDurationPart.setBody(s_duration.toUtf8().data());

    // 将 `file_title` 字段添加到多部分对象中
    multiPart->append(filePlayBackDurationPart);

    // 创建请求
    QNetworkRequest request(url);
    qDebug()<<"url:"<<url;
    qDebug()<<"file_path:"<<file_path;
    // 设置 Content-Type 头为 multipart/form-data
    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + boundary);

    // 发起 POST 请求
    QNetworkReply *reply = m_manager->post(request, multiPart);
    multiPart->setParent(reply); // 确保多部分对象在响应销毁时不会泄漏

    // 使用事件循环等待响应
    /*QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();*/

    // 处理响应
    connect(reply,&QNetworkReply::finished,this,[=](){
        if (reply->error()) {
            qDebug() << "上传失败：" << reply->errorString();
        } else {
            qDebug() << "上传成功，服务器响应：" << reply->readAll();
        }

        reply->deleteLater();
    });

    qDebug()<<"out out out";

}
//弃用
QBuffer* NetWorkManager::http_download_file()
{
    qDebug()<<"enter http_download_file ";
    QNetworkRequest request(QUrl("http://192.168.208.128:8888/group1/M00/00/03/wKjQgGe15OyAFfdVABwtxOjf0Ks7803.ts"));
    //QNetworkRequest request(QUrl("http://192.168.208.128:8888/"));
    QNetworkReply* reply=m_manager->get(request);
    QBuffer* buf=new QBuffer(this);
    connect(reply,&QNetworkReply::finished,this,[this,reply](){
        this->sloHandleDownloadData(reply);
    });
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

void NetWorkManager::http_get_files_info(VideoList* video_list_wid)
{
    QString s="http://192.168.208.128:8888/api/filesList?files=4&strategy=random&username=null";
    QUrl url(s);
    QNetworkRequest request(url);
    qDebug()<<"enter  http_get_files_info";
    QNetworkReply* reply=m_manager->get(request);
    connect(reply,&QNetworkReply::finished,this,[=](){
         video_list_wid->sloShowFilesInfo(reply);

    });
    /*QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();*/
    //qDebug()<<"yyyy:"<<reply->readAll();
    //return reply;
}

QNetworkReply *NetWorkManager::getFilesInfoReply()
{
    if(m_files_info_reply==nullptr){
        qDebug()<<"getFilesInfoReply return null";
        return nullptr;
    }
     qDebug()<<"enter  getFilesInfoReply";
    return m_files_info_reply;
}

QNetworkReply* NetWorkManager::http_get_img_cover(QString& file_img_path,QLabel* lab_img)
{
    QString s=file_img_path;

    QUrl url(s);
    QNetworkRequest request(url);
    qDebug()<<"enter  http_get_img_cover";
    QNetworkReply* reply=m_manager->get(request);
    connect(reply,&QNetworkReply::finished,this,[=](){
        QByteArray byte_arr=reply->readAll();
        QPixmap pix;
        if(!pix.loadFromData(byte_arr)){
            qDebug()<<"loadFromData is failed";
            return nullptr;
        }
        lab_img->setPixmap(pix.scaled(lab_img->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        lab_img->setAlignment(Qt::AlignCenter);
        lab_img->setScaledContents(true);
        reply->deleteLater();
    });
    /*QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    return reply;*/

}

bool NetWorkManager::http_login(const QString& account,const QString& password)
{
    qDebug()<<"login start";
    QJsonObject root;
    root.insert("account",account);
    root.insert("password",password);
    QJsonDocument js_doc(root);
    QUrl url("http://192.168.208.128:8888/api/login");
    QNetworkRequest request(url);
    QByteArray post_data = js_doc.toJson(QJsonDocument::Compact);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply=m_manager->post(request,post_data);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if(reply->error()){
        qDebug()<<"http_login reply error";
        return false;
    }
    QByteArray respon_data=reply->readAll();
    QJsonDocument respon_doc = QJsonDocument::fromJson(respon_data);
    if (respon_doc.isNull()) {
        qDebug() << "Invalid JSON data.";
        return false;
    }
    QJsonObject respon_obj = respon_doc.object();
    QString status=respon_obj.value("status").toString();
    if(status=="0"){
        return true;
    }
    return false;
}

bool NetWorkManager::http_register(const QString &account, const QString &password, const QString &nickname, const QString &repassword)
{
    qDebug()<<"http_register start";
    QJsonObject root;
    root.insert("account",account);
    root.insert("password",password);
    root.insert("nickname",nickname);

    QJsonDocument js_doc(root);
    QUrl url("http://192.168.208.128:8888/api/register");
    QNetworkRequest request(url);
    QByteArray post_data = js_doc.toJson(QJsonDocument::Compact);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply=m_manager->post(request,post_data);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if(reply->error()){
        qDebug()<<"http_register reply error";
        return false;
    }
    QByteArray respon_data=reply->readAll();
    QJsonDocument respon_doc = QJsonDocument::fromJson(respon_data);
    if (respon_doc.isNull()) {
        qDebug() << "Invalid JSON data.";
        return false;
    }
    QJsonObject respon_obj = respon_doc.object();
    QString status=respon_obj.value("status").toString();
    if(status=="0"){
        return true;
    }
    return false;
}

void NetWorkManager::http_get_user_video_lists_info(int opt,VideoList* video_list)
{
    //查看为什么出现点击一次btn_user_page结果调用两次http_get_user_video_lists_info的原因


    //利用opt实现代码复用,对于user_video_lists_info和user_history_video_lists_info这两个请求来说，通过opt传入参数不同区分,0,1


    QString username=UserManager::instance()->getUserName();
    QString s="";
    if(opt==1){
        s="http://192.168.208.128:8888/api/filesList?files=4&strategy=userhistory&username="+username;
    }
    else if(opt==0){
        s="http://192.168.208.128:8888/api/filesList?files=4&strategy=userowned&username="+username;
    }
    else{
        qDebug()<<"error with http_get_user_video_lists_info";
        return ;
    }
    qDebug()<<"uuuurl:"<<s;
    QUrl url(s);
    QNetworkRequest request(url);
    qDebug()<<"enter  http_get_user_video_lists_info";
    QNetworkReply* reply=m_manager->get(request);
    connect(reply,&QNetworkReply::finished,video_list,[=](){
        video_list->sloShowFilesInfo(reply);
    });
    /*QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QTimer timer;
    connect(&timer, &QTimer::timeout, [&]() {
        if (reply->isRunning()) {
            qWarning() << "Request timed out";
            reply->abort(); // 中止请求
            loop.quit();    // 退出事件循环
        }
    });

    timer.start(2000);
    loop.exec();
    timer.stop();*/
    qDebug()<<"http_get_user_video_lists_info out";


}

void NetWorkManager::http_get_search_video_lists_info(VideoList *video_list, const QString &key)
{
    QString username=UserManager::instance()->getUserName();
    if(username==""){
        username="null";
    }
    QString s="http://192.168.208.128:8888/api/filesList?files=4&strategy=search&username="+username+"&key="+key;

    QUrl url(s);
    QNetworkRequest request(url);
    qDebug()<<"enter  http_get_search_video_lists_info";
    QNetworkReply* reply=m_manager->get(request);
    connect(reply,&QNetworkReply::finished,video_list,[=](){

        video_list->sloShowFilesInfo(reply);
    });
    qDebug()<<"http_get_search_video_lists_info out";
}

void NetWorkManager::http_insert_user_history_log(QString& username,QString& file_md5,int progress_)
{
    qDebug()<<"enter http_insert_user_history_log";
    QString s="http://192.168.208.128:8888/api/uphistory";
    QUrl url(s);
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject root;

    root.insert("username",username);
    root.insert("file_md5",file_md5);
    root.insert("progress_data",QString::number(progress_));
    QJsonDocument js_doc(root);
    QByteArray post_data = js_doc.toJson(QJsonDocument::Compact);
    qDebug()<<"http_insert_user_history_log mid";
    QNetworkReply* reply=m_manager->post(request,post_data);
    connect(reply,&QNetworkReply::finished,this,[=](){
        qDebug()<<"http_insert_user_history_log finished";
        reply->deleteLater();
    });
    /*QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();*/
}

void NetWorkManager::http_get_user_info(QLabel* lab1,QLabel* lab2)
{
    QString username=UserManager::instance()->getUserName();
    QString s="http://192.168.208.128:8888/api/userinfo?username="+username;
    qDebug()<<"http_get_user_info:"<<s;
    QUrl url(s);
    QNetworkRequest request(url);
    QNetworkReply* reply=m_manager->get(request);
    connect(reply,&QNetworkReply::finished,this,[=](){
        onGetUserInfo(lab1,lab2,reply);
    });
    /*QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();*/
    return;
}

void NetWorkManager::http_insert_search_log(QString& search_key)
{
    QString username=UserManager::instance()->getUserName();
    QString s="http://192.168.208.128:8888/api/searchlog";
    QUrl url(s);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject root;

    if(username==""){
        //游客状态搜索
        return;
    }
    else{
        //登录状态搜索
        root.insert("username",username);
        root.insert("search_key",search_key);

        QJsonDocument js_doc(root);
        QByteArray post_data = js_doc.toJson(QJsonDocument::Compact);
        QNetworkReply* reply=m_manager->post(request,post_data);
        connect(reply,&QNetworkReply::finished,this,[=](){
            if(reply->error()){
                qDebug()<<"error http_insert_search_log with reply";
                return;
            }
        });
    }
}
//疑似弃用
QNetworkReply* NetWorkManager::http_get_search_log(const QString& key)
{
    QString username=UserManager::instance()->getUserName();
    QString s="http://192.168.208.128:8888/api/getsearchlog?username="+username+"&key="+key;
    QUrl url(s);
    QNetworkRequest request(url);
    QNetworkReply* reply=m_manager->get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    return reply;
}

void NetWorkManager::http_get_recommend_audio_lists(const QString& file_md5,const QString& file_type,VideoDisplay* vdis)
{
    //先上传当前播放的音频文件属于什么类型的，后端查询标签是什么样的，返回类似的
    QString username=UserManager::instance()->getUserName();
    //QString s="http://192.168.208.128:8888/api/listloop?username="+username+"&file_md5="+file_md5;
    QString s="http://192.168.208.128:8888/api/listloop";
    /*QString feature_1;
    QString feature_2;
    QString feature_3;
    QString feature_4;
    QString feature_5;*/
    QJsonObject root;

    if(username==""){
        //游客状态推荐
        username="null";
        root.insert("username",username);
        root.insert("file_md5",file_md5);
        root.insert("file_type",file_type);

        QJsonDocument js_doc(root);
        QByteArray post_data = js_doc.toJson(QJsonDocument::Compact);
        QUrl url(s);
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QNetworkReply* reply=m_manager->post(request,post_data);
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, [&]() {  // 确保 loop 不是常量
            vdis->listLoopCoverShowInfo(reply);
            reply->deleteLater();
            loop.quit();  // 正确调用 quit()
        });
        loop.exec();
        /*connect(reply,&QNetworkReply::finished,this,[=](){
            if(reply->error()){
                qDebug()<<"error http_insert_search_log with reply";
                reply->deleteLater();
                return;
            }

            vdis->listLoopCoverShowInfo(reply);
            reply->deleteLater();
        });*/
        return;
    }
    else{
        //登录状态搜索
        //username="null";
        root.insert("username",username);
        root.insert("file_md5",file_md5);
        root.insert("file_type",file_type);

        QJsonDocument js_doc(root);
        QByteArray post_data = js_doc.toJson(QJsonDocument::Compact);
        QUrl url(s);
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QNetworkReply* reply=m_manager->post(request,post_data);
        /*connect(reply,&QNetworkReply::finished,this,[=](){
            if(reply->error()){
                qDebug()<<"error http_insert_search_log with reply";
                reply->deleteLater();
                return;
            }
            vdis->listLoopCoverShowInfo(reply);
            reply->deleteLater();

        });*/
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, [&]() {  // 确保 loop 不是常量
            vdis->listLoopCoverShowInfo(reply);
            reply->deleteLater();
            loop.quit();  // 正确调用 quit()
        });
        loop.exec();
        return;
    }
    /*QNetworkReply* reply=m_manager->get(request);
    connect(reply, &QNetworkReply::finished, this,[=](){
        this->sloHandleRecommendFeatures(reply);
    });*/
}

void NetWorkManager::http_get_random_audios(VideoList* video_list_wid)
{
    QString s="http://192.168.208.128:8888/api/filesList?files=4&strategy=random&username=null&key=0";
    QUrl url(s);
    QNetworkRequest request(url);
    qDebug()<<"enter  http_get_files_info";
    QNetworkReply* reply=m_manager->get(request);
    connect(reply,&QNetworkReply::finished,this,[=](){
        qDebug()<<"http_get_random_audios--------------";
         video_list_wid->sloShowFilesInfo(reply);
        qDebug()<<"out http_get_random_audios--------------";
    });
}

void NetWorkManager::http_get_random_videos(VideoList *video_list_wid)
{
    QString s="http://192.168.208.128:8888/api/filesList?files=4&strategy=random&username=null&key=1";
    QUrl url(s);
    QNetworkRequest request(url);
    qDebug()<<"enter  http_get_files_info";
    QNetworkReply* reply=m_manager->get(request);
    connect(reply,&QNetworkReply::finished,this,[=](){
        qDebug()<<"http_get_random_audios--------------";
         video_list_wid->sloShowFilesInfo(reply);
        qDebug()<<"out http_get_random_audios--------------";
    });
}

void NetWorkManager::http_get_search_audios(VideoList *video_list_wid, QString key_search)
{
    QString s="http://192.168.208.128:8888/api/filesList?files=4&strategy=random&username=null&key=0&key_search=%1";
    s=s.arg(key_search);
    QUrl url(s);
    QNetworkRequest request(url);
    qDebug()<<"enter  http_get_files_info";
    QNetworkReply* reply=m_manager->get(request);
    connect(reply,&QNetworkReply::finished,this,[=](){
        qDebug()<<"http_get_random_audios--------------";
         video_list_wid->sloShowFilesInfo(reply);
        qDebug()<<"out http_get_random_audios--------------";
    });
}

void NetWorkManager::http_get_search_videos(VideoList *video_list_wid, QString key_search)
{
    QString s="http://192.168.208.128:8888/api/filesList?files=4&strategy=random&username=null&key=1&key_search=%1";
    s=s.arg(key_search);
    QUrl url(s);
    QNetworkRequest request(url);
    qDebug()<<"enter  http_get_files_info";
    QNetworkReply* reply=m_manager->get(request);
    connect(reply,&QNetworkReply::finished,this,[=](){
        qDebug()<<"http_get_random_audios--------------";
         video_list_wid->sloShowFilesInfo(reply);
        qDebug()<<"out http_get_random_audios--------------";
    });
}
//未完
void NetWorkManager::http_get_new_video_lists_file_info(int flag,VideoList* video_list,int offset)
{
    if(!video_list){
        qDebug("http_get_new_video_lists_file_info invalid params");
        return;
    }
    qDebug()<<"enter http_get_new_video_lists_file_info";
    //根据flag来判断到底是什么新增，比如在home下的随机新增，在userown的用户上传新增，\
    或者用户观看历史新增，或者用户搜索新增，或者home_audios新增,home_videos新增,search_audios新增，search_videos新增\
    都是复用这一个函数
    //依次0,1,2,3，4,5
    QString s="";
    //QNetworkReply* reply=nullptr;
    if(flag==0){
        s="http://192.168.208.128:8888/api/filesList?files=4&offset=%1&strategy=random&username=null";
        s=s.arg(offset);
        QUrl url(s);
        QNetworkRequest request(url);
        QNetworkReply* reply=m_manager->get(request);
        connect(reply,&QNetworkReply::finished,this,[=](){
            video_list->sloShowFilesInfo(reply);
            //reply->deleteLater();
        });
    }
    else if(flag==1){
        qDebug()<<"enter http_get_new_video_lists_file_info and flag == 1";
        QString username=UserManager::instance()->getUserName();
        s="http://192.168.208.128:8888/api/filesList?files=4&offset=%1&strategy=userowned&username="+username;
        s=s.arg(offset);
        QUrl url(s);
        QNetworkRequest request(url);
        QNetworkReply* reply=m_manager->get(request);
        connect(reply,&QNetworkReply::finished,this,[=](){
            video_list->sloShowFilesInfo(reply);
            //reply->deleteLater();
        });
    }
    else if(flag==2){
        qDebug()<<"enter http_get_new_video_lists_file_info and flag == 2";
        QString username=UserManager::instance()->getUserName();
        s="http://192.168.208.128:8888/api/filesList?files=4&offset=%1&strategy=userhistory&username="+username;
        s=s.arg(offset);
        QUrl url(s);
        QNetworkRequest request(url);
        QNetworkReply* reply=m_manager->get(request);
        connect(reply,&QNetworkReply::finished,this,[=](){
            video_list->sloShowFilesInfo(reply);
            //reply->deleteLater();
        });
    }

}

void NetWorkManager::http_get_video_analysis_messages(const QString& file_md5,VideoDisplay* vdis,int index)
{
    //实际上是获取一条弹幕信息

    QString s="http://192.168.208.128:8888/api/analysismessages?file_md5="+file_md5;
    QUrl url(s);
    QNetworkRequest request(url);
    qDebug()<<"enter  http_get_video_analysis_messages";
    QNetworkReply* reply=m_manager->get(request);
    connect(reply,&QNetworkReply::finished,this,[=](){
        vdis->analyzeDanmuMessages(reply);
    });
}



QBuffer* NetWorkManager::sloHandleDownloadData(QNetworkReply* reply)
{
    qDebug()<<"enter sloHandleDownloadData";
    QByteArray byte_array=reply->readAll();
    /*QFile file("/home/hcc/ttttt.ts");
    if(!file.open(QIODevice::WriteOnly)){
        qDebug()<<"open file failed";
    }
    file.write(byte_array);
    file.close();*/
    QBuffer* buffer=new QBuffer(this);
    buffer->setData(byte_array);
    buffer->open(QIODevice::ReadOnly);
    if(buffer){
        qDebug()<<"sloHandleDownloadData buffer have";
    }
    //qDebug()<<"sloHandleDownloadData buffer:"<<buffer->buffer();
    m_buffer=buffer;
}

QBuffer *NetWorkManager::sloHandleVideosInfo(QNetworkReply *reply)
{
    if(reply->error()==QNetworkReply::NoError){
        qDebug()<<"error";
        return nullptr;
    }
    m_files_info_reply=reply;
    qDebug()<<"enter  sloHandleVideosInfo";
    /*QByteArray byte_array=reply->readAll();
    QJsonDocument json_docm=QJsonDocument::fromJson(byte_array);
    if(json_docm.isNull()){
        qDebug()<<"json document is null";
        return nullptr;
    }
    QJsonObject obj_root=json_docm.object();
    QString status=obj_root.value("status").toString();
    int file_info_cnt=obj_root.value("file_info_cnt").toInt();
    for(int i=0;i<file_info_cnt;i++){

    }*/
}

void NetWorkManager::onGetUserInfo(QLabel *lab1, QLabel *lab2,QNetworkReply* reply)
{

    QByteArray byte_array=reply->readAll();
    qDebug()<<"hhhhhhhhhhhhh:"<<byte_array;
    QJsonDocument json_docm=QJsonDocument::fromJson(byte_array);
    if(json_docm.isNull()){
        qDebug()<<"json document is null";
        return;
    }
    QJsonObject obj_root=json_docm.object();
    QString status=obj_root.value("status").toString();
    QString username=obj_root.value("username").toString();
    QString nickname=obj_root.value("nickname").toString();
    if(status=="0"){
        qDebug()<<"user info get failed";
        return;
    }

    lab1->setText(username);
    lab2->setText(nickname);
}

void NetWorkManager::sloHandleRecommendFeatures(QNetworkReply *reply)
{
    QByteArray byte_array=reply->readAll();
    QJsonDocument json_docm=QJsonDocument::fromJson(byte_array);
    if(json_docm.isNull()){
        qDebug()<<"json document is null";
        return;
    }
    QJsonObject obj_root=json_docm.object();
    QString status=obj_root.value("status").toString();
    if(status=="1"){
        qDebug()<<"error reply";
        return;
    }
    QString f1=obj_root.value("file_feature_1").toString();
    QString f2=obj_root.value("file_feature_2").toString();
    QString f3=obj_root.value("file_feature_3").toString();
    QString f4=obj_root.value("file_feature_4").toString();
    QString f5=obj_root.value("file_feature_5").toString();
    //再发送请求获取推荐音频列表

}
