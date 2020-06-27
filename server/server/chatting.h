#ifndef CHATTING_H
#define CHATTING_H

#include <QDialog>
#include <QKeyEvent>
#include <QTcpSocket>
#include <QTcpServer>
#include <QFileDialog>
#include <QTextDocumentFragment>
#include <QImage>
#include <QTextDocument>
#include <QTextImageFormat>
#include <QImageReader>
#include <QBuffer>
#include <QMessageBox>
#include <QDateTime>

#include "record.h"

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
    void acceptConnection();                	//接收客户端连接
    void readData();                            //读取客户端消息
    void disconnection();                       //断开连接处理
    void sleep(int msec);                       //延时
    void showImage(QImage image, QUrl url);		//显示图片
    void moveTextCurser();						//移动光标

    void on_send_pushButton_clicked();          //发送消息处理
    void on_connect_pushButton_clicked();       //监听处理
    void on_pictureSelect_pushButton_clicked(); //选择发送图片
    void on_chatRecord_pushButton_clicked();	//显示聊天记录

private:
    Ui::chatting *ui;

    QTcpServer * 	m_server;	//用于监听连接
    QTcpSocket * 	m_socket;	//接收客户端连接对象
    QDateTime       m_time;     //时间
};

#endif // CHATTING_H

