#include "chatting.h"
#include "ui_chatting.h"

chatting::chatting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chatting)
{
    ui->setupUi(this);

    //聊天主界面界面及部件设计
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(520, 530);
    this->move(900, 100);
    QPalette  palette (this->palette());
    palette.setColor(QPalette::Background, QColor(200, 200, 200, 90));
    this->setPalette( palette );

    ui->userName_lineEdit->setAlignment(Qt::AlignCenter);
    ui->userName_lineEdit->setGeometry(QRect(QPoint(180,0), QPoint(340, 30)));
    ui->userName_lineEdit->setStyleSheet("QLineEdit{background-color:rgb(195, 190, 195);"
                                       "font-family:'黑体';"
                                       "font-size:14px;"
                                       "border-radius:10px"
                                       "}");

    ui->connect_pushButton->setGeometry(QRect(QPoint(430,0), QPoint(510, 30)));

    ui->showContent_textEdit->setText("");
    ui->showContent_textEdit->setContentsMargins(20,0,20,0);
    ui->showContent_textEdit->adjustSize();
    ui->showContent_textEdit->setGeometry(QRect(QPoint(0,30), QPoint(530, 390)));
    ui->showContent_textEdit->setReadOnly(true);

    ui->pictureSelect_pushButton->setGeometry(QRect(QPoint(10,390), QPoint(90, 420)));
    ui->chatRecord_pushButton->setGeometry(QRect(QPoint(110,390), QPoint(190, 420)));
    ui->send_pushButton->setGeometry(QRect(QPoint(430,390), QPoint(510, 420)));

    ui->sendReady_textEdit->setGeometry(QRect(QPoint(0,420), QPoint(520, 530)));
    ui->sendReady_textEdit->installEventFilter(this);

    //初始化连接状态
    m_flag = false;
}

chatting::~chatting()
{
    delete ui;
}

/* 移动光标，使界面总是显示最新消息
*/
void chatting::moveTextCurser() {
    QTextCursor cursor=ui->showContent_textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->showContent_textEdit->setTextCursor(cursor);
}

/*	1. 向 server 发送消息
    2. 将发送内容显示到聊天显示区(showContent_label)
*/
void chatting::on_send_pushButton_clicked()
{
    if(m_socket->state() == QAbstractSocket::ConnectedState) {
        if(ui->sendReady_textEdit->toPlainText() != "") {
            QString msg = ui->sendReady_textEdit->toPlainText();
            m_socket->write(msg.toLatin1(), msg.length());
        }
        m_time = QDateTime::currentDateTime();
        ui->showContent_textEdit->append("me [" + m_time.toString("yyyy-MM-dd hh:mm:ss ddd") + "]\n" + ui->sendReady_textEdit->toPlainText());
        saveRecord("me [" + m_time.toString("yyyy-MM-dd hh:mm:ss ddd") + "]\n" + ui->sendReady_textEdit->toPlainText());
        ui->sendReady_textEdit->setText("");
        moveTextCurser();
    }
}

/*	1. 捕捉回车事件，实现发送
*/
bool chatting::eventFilter(QObject *target, QEvent *event) {
    if(target == ui->sendReady_textEdit) {
        if(event->type() == QEvent::KeyPress) {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);

             if(k->key() == Qt::Key_Return)
             {
                 on_send_pushButton_clicked();		//替换为需要响应的函数事件，以这里的按钮为例
                 return true;
             }
        }
    }
    return QWidget::eventFilter(target,event);
}

/* 辅助连接请求按键，解决未连接成功仍显示连接的错误
*/
void chatting::connect_help() {
    ui->userName_lineEdit->setText("Dear Friend");
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readData()));
    ui->connect_pushButton->setText("退出");

    m_flag = true;
}

/*	1. 发起/断开 连接
    2. 提示连接状态
    3. connect 读数据处理
    4. 调整连接状态
*/
void chatting::on_connect_pushButton_clicked()
{
    if(m_flag) {
        m_socket->disconnect();
        this->close();
    }
    else {
        m_socket = new QTcpSocket(this);
        m_socket->connectToHost(QHostAddress("127.0.0.1"), 8080);
        connect(m_socket, SIGNAL(connected()), this, SLOT(connect_help()));
    }
}

/* 显示图片
*/
void chatting::showImage(QImage image, QUrl url) {
    ui->showContent_textEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    QTextDocument * textDocument = ui->showContent_textEdit->document();
    textDocument->addResource( QTextDocument::ImageResource, url, QVariant(image));
    QTextCursor cursor = ui->showContent_textEdit->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth( image.width()/3 );
    imageFormat.setHeight( image.height()/3 );
    imageFormat.setName( url.toString() );
    cursor.insertImage(imageFormat);
    moveTextCurser();
}

/*	1. 读取 client 消息
    2. 判断收到的是图片还是文字
    3. 将接收内容显示到聊天显示区(showContent_textEdit)
*/
void chatting::readData() {
    QTcpSocket *socket = (QTcpSocket*)sender();
    if(socket == 0) return;
    QByteArray imageData;
    QImage image;
    m_time = QDateTime::currentDateTime();
//FFD8FF
    imageData.append((QByteArray)m_socket->readAll());
    if( (imageData.size() > 3) && (((unsigned char)imageData[0] == 0xFF) &&  ((unsigned char)imageData[1] == 0xD8) &&  ((unsigned char)imageData[2] == 0xFF)) ) {
        //QHostAddress addr = m_socket->peerAddress();
        ui->showContent_textEdit->append("My Friend [" + m_time.toString("yyyy-MM-dd hh:mm:ss ddd") + "]\n");
        image.loadFromData(imageData);
        QString imageName = QString("/home/lpenguin/chat/record/client_record/record%1.jpg").arg(m_time.toString("yyyy-MM-dd hh:mm:ss ddd"));
        image.save(imageName, "JPG", 100);
        QUrl url( QString("file://%1").arg(imageName));
        showImage(image, url);

        saveRecord("My Friend : \npicture -- " + imageName);
        return ;
    }

    QString msg = (QString)imageData;
    ui->showContent_textEdit->append("My Friend [" + m_time.toString("yyyy-MM-dd hh:mm:ss ddd") + "]");
    ui->showContent_textEdit->append(msg);
}

/*	1. 图片发送
*/
void chatting::on_pictureSelect_pushButton_clicked()
{
    m_time = QDateTime::currentDateTime();
    ui->showContent_textEdit->append("me [" + m_time.toString("yyyy-MM-dd hh:mm:ss ddd") + "]\n");
    QString s = QFileDialog::getOpenFileName(this, "select picture", ".", "JPEG(*.jpg)");
    QUrl url( QString("file://%1").arg(s));
    QImage image = QImageReader(s).read();
    showImage(image, url);

    QByteArray picArray;
    QBuffer buffer(&picArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "jpg");
    m_socket->write(picArray);
    saveRecord("me [" + m_time.toString("yyyy-MM-dd hh:mm:ss ddd") + "]: \npicture : " + s);
}

/* 显示聊天记录界面
*/
void chatting::on_chatRecord_pushButton_clicked() {
    record record_open;
    record_open.exec();
}

/* 以文本形式保存聊天记录
*/
void chatting::saveRecord(QString recordContent) {
    QFile record(QString("../../record/client_record/record.txt"));
    QDir dir;
    if(!record.open(QFile::Append | QFile::Text))
    {
        QMessageBox::information(this, "Error Message", QString("Please Select a Text File : %1!").arg(dir.currentPath()));
        return;
    }
    QTextStream out(&record);
    out << recordContent;
    out << "\n";
}
