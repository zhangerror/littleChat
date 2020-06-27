#include "start.h"
#include "ui_start.h"

start::start(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::start)
{
    ui->setupUi(this);

    this->setWindowTitle("登录");
    ui->passwd_lineEdit->setEchoMode(QLineEdit::Password);
}

start::~start()
{
    delete ui;
}

void start::on_login_pushButton_clicked()
{
    QFile user("../../user/client.txt");
    user.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&user);
    bool end = true;

    while(!in.atEnd()) {
        QString userID = "", userPasswd = "";
        in.readLine(-1);
        in>>userID>>userPasswd;
        if( (ui->id_lineEdit->text()!="" && ui->passwd_lineEdit->text()!="") && (userID == ui->id_lineEdit->text()) && (userPasswd == ui->passwd_lineEdit->text())) {
            qDebug() << userID;
            qDebug() << userPasswd;
            end = false;
            chatting new_chat;
            this->close();
            new_chat.exec();
        }
    }
    if(end) {
        QMessageBox::information(this, "Error Message", QString("非法用户：帐号或密码错误!"));
        ui->passwd_lineEdit->setText("");
    }
}
