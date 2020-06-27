#ifndef CHATTING_H
#define CHATTING_H

#include <QDialog>
#include <QKeyEvent>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QFileDialog>
#include <QTextDocumentFragment>
#include <QImage>
#include <QTextDocument>
#include <QTextImageFormat>
#include <QImageReader>
#include <QBuffer>
#include <QMessageBox>
#include "record.h"
#include <QDir>
#include <QTime>
#include <QDateTime>
#include <QTextCursor>

namespace Ui {
class chatting;
}

class chatting : public QDialog
{
    Q_OBJECT

public:
    explicit chatting(QWidget *parent = 0);
    ~chatting();

protected:
    bool eventFilter(QObject *target, QEvent *event);	//事件过滤器
    void saveRecord(QString recordContent);

private slots:
    void readData();                            //读取服务端数据
    void connect_help();						//辅助连接
    void moveTextCurser();						//移动光标
    void showImage(QImage image, QUrl url);		//显示图片

    void on_send_pushButton_clicked();          //发送消息处理
    void on_connect_pushButton_clicked();       //连接请求处理
    void on_pictureSelect_pushButton_clicked(); //选择发送图片
    void on_chatRecord_pushButton_clicked();	//显示聊天记录

private:
    Ui::chatting *ui;

    QTcpSocket *    m_socket;	//用于连接请求
    bool            m_flag;		//当前连接状态
    QDateTime       m_time;     //时间
};

#endif // CHATTING_H
