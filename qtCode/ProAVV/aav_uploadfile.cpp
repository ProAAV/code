#include "aav_uploadfile.h"
#include "ui_aav_uploadfile.h"
#include<QDebug>
#include<QPixmap>
#include<QPushButton>
#include<QLabel>
#include<QLineEdit>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include"aav_networkthread.h"
#include"aav_networkmanager.h"
extern "C"{
    #include<libavcodec/avcodec.h>
    #include<libavformat/avformat.h>
    #include<libavutil/avutil.h>
    #include<libswscale/swscale.h>
    #include<libavutil/imgutils.h>
}

UploadFile::UploadFile(QString& file_path,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UploadFile)
{
    ui->setupUi(this);
    qDebug()<<"hhh";

    if(file_path.isEmpty()){
        qDebug()<<"file_path is null";
        return;
    }
    //设计上传视频文件窗口
    QLabel* lab_set_intro=new QLabel(this);
    lab_set_intro->setText("输入音频/视频文件简介");
    QHBoxLayout* hlayout=new QHBoxLayout();

    QLineEdit* line_edit=new QLineEdit(this);

    QPushButton* btn_commit=new QPushButton(this);
    btn_commit->setText("上传");
    QVBoxLayout* vlayout=new QVBoxLayout();
    hlayout->addWidget(lab_set_intro);
    hlayout->addWidget(line_edit);
    vlayout->addLayout(hlayout);
    vlayout->addWidget(btn_commit);
    this->setLayout(vlayout);

    connect(btn_commit,&QPushButton::clicked,this,[=](){
        //获取用户输入的上传简介/标题
        QString file_title=line_edit->text();

        m_file_path=file_path;
        //首先捕获一张封面
        //captureAnPicture();
        //获取到音视频播放时长
        double file_playback_duration=getVideoFileDuration();
        //然后再上传视频文件和捕获的封面图片
        sendVdoFile(file_title,file_playback_duration);
    });

    /*m_file_path=file_path;
    //首先捕获一张封面
    captureAnPicture();
    //然后再上传视频文件和捕获的封面图片
    sendVdoFile(file_path);*/
}
void UploadFile::sendVdoFile(QString& file_title,double file_playback_duration){
    qDebug()<<"send";
    NetWorkThread* thread=new NetWorkThread();
    thread->start();
    thread->m_net_manager->http_upload_file(file_playback_duration,file_title,m_file_path,"http://192.168.208.128:8888/api/upload");

}

double UploadFile::getVideoFileDuration()
{
        AVFormatContext *fmt_ctx = nullptr;

        // 打开音视频文件
        if (avformat_open_input(&fmt_ctx, m_file_path.toUtf8().constData(), nullptr, nullptr) != 0) {
            qDebug()<<"open file failed";
            return 1;
        }

        // 获取流信息
        if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
            qDebug()<<"can not get stream info";
            avformat_close_input(&fmt_ctx);
            return 1;
        }

        // 获取总时长（单位：微秒）
        double durationInSeconds=0.0;
        int64_t duration = fmt_ctx->duration;
        if (duration == AV_NOPTS_VALUE) {
            qDebug()<<"duration is failed to get";
        } else {
            // 转换为秒
            durationInSeconds = duration / (double)AV_TIME_BASE;
        }

        // 释放资源
        avformat_close_input(&fmt_ctx);
        return durationInSeconds;
}
/*void UploadFile::captureAnPicture(){
    AVFormatContext* format_context=nullptr;
    //open video file firstly
    int ret=avformat_open_input(&format_context,m_file_path.toUtf8().constData(),nullptr,nullptr);
    if(ret!=0){
        qDebug()<<"avformat_open_input failed";
        return;
    }
    //find stream info ,for example:resolution,frame rate,audio sampling rate
    ret=avformat_find_stream_info(format_context,nullptr);
    if(ret<0){
        qDebug()<<"avformat_find_stream_info failed";
        //reason for avformat_open_input() was called before,so there need to call the close func
        avformat_close_input(&format_context);
        return;
    }
    //next,we need to found video_stream_index
    int video_stream_index=-1;
    for(unsigned i=0;i<format_context->nb_streams;i++){
        if(format_context->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_VIDEO){
            video_stream_index=i;
            break;
        }
    }
    if(video_stream_index==-1){
        qDebug()<<"can not found video stream";
        avformat_close_input(&format_context);
        return;
    }
    //found this video_stream's decoder
    AVCodecParameters* decoder_param=format_context->streams[video_stream_index]->codecpar;
    const AVCodec* decodec=avcodec_find_decoder(decoder_param->codec_id);
    if(!decodec){
        qDebug()<<"cant not found decodec";
        avformat_close_input(&format_context);
        return;
    }
    //get a decoder context
    AVCodecContext* decoder_context=avcodec_alloc_context3(decodec);
    if(!decoder_context){
        qDebug()<<"cant not get decodec context";
        avformat_close_input(&format_context);
        return;
    }
    //copy decoder info parameters to decoder_context
    ret=avcodec_parameters_to_context(decoder_context,decoder_param);
    if(ret<0){
        qDebug()<<"cant not copy decoder info parameters to decoder_context";
        avcodec_free_context(&decoder_context);
        avformat_close_input(&format_context);
        return;
    }
    //open decoder
    ret=avcodec_open2(decoder_context,decodec,nullptr);
    if(ret<0){
        qDebug()<<"can not open decoder";
        avcodec_free_context(&decoder_context);
        avformat_close_input(&format_context);
        return;
    }
    AVPacket packet;
    AVFrame* frame=av_frame_alloc();
    AVFrame* rgb_frame=av_frame_alloc();
    if(!frame||!rgb_frame){
        qDebug()<<"faield to av_frame_alloc()";
        av_frame_free(&frame);
        av_frame_free(&rgb_frame);
        avcodec_free_context(&decoder_context);
        avformat_close_input(&format_context);
        return;
    }
    int frame_num=0;
    int frame_count=0;
    while(av_read_frame(format_context,&packet)>=0){
        if(packet.stream_index!=video_stream_index){
            av_packet_unref(&packet);
            continue;
        }
        ret=avcodec_send_packet(decoder_context,&packet);
        if(ret<0){
            qDebug()<<"failed to send packet to decoder";
            break;
        }
        while(avcodec_receive_frame(decoder_context,frame)==0){
            frame_count++;
            //turn to RGB format
            struct SwsContext* swsContext = sws_getContext(
                decoder_context->width,
                decoder_context->height,
                decoder_context->pix_fmt,
                decoder_context->width,
                decoder_context->height,
                AV_PIX_FMT_RGB24,
                SWS_BILINEAR,
                nullptr,
                nullptr,
                nullptr
                );
            if (!swsContext) {
                qDebug() << "can not init image transfer context";
                break;
            }
            int numBytes =  av_image_get_buffer_size(AV_PIX_FMT_RGB24, decoder_context->width, decoder_context->height, 1);
            uint8_t* buffer = (uint8_t*)av_malloc(numBytes);
            av_image_fill_arrays(rgb_frame->data, rgb_frame->linesize, buffer, AV_PIX_FMT_RGB24, decoder_context->width, decoder_context->height, 1);
            sws_scale(
                swsContext,
                frame->data,
                frame->linesize,
                0,
                decoder_context->height,
                rgb_frame->data,
                rgb_frame->linesize
            );
            sws_freeContext(swsContext);
            //av_freep(&buffer);
            //turn RGB to QImage
            QImage image(
                rgb_frame->data[0],
                decoder_context->width,
                decoder_context->height,
                QImage::Format_RGB888
            );
            //save picture
            QString outputImagePath = "/home/hcc/share/"+QString("%1.png").arg(frame_num);
            if(image.isNull()){
                qDebug()<<"image is null";
                goto end;
            }
            image.save(outputImagePath);
            qDebug() << "成功捕获第" << frame_num << "帧并保存为" << outputImagePath;
            if(frame_count>0){
                goto end;
            }
        }
    }
    end:
    av_packet_unref(&packet);
}*/
UploadFile::~UploadFile()
{
    delete ui;
}
