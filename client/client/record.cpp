#include "record.h"
#include "ui_record.h"

record::record(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::record)
{
    ui->setupUi(this);

    this->move(800, 100);

    this->setWindowFlags(Qt::FramelessWindowHint);
    QFile record(QString("../../record/server_record/record.txt"));
    if(!record.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(this, "Error Message", "Please Select a Text File!");
        return;
    }
    QTextStream in(&record);
    ui->record_textEdit->setText(in.readAll());
    ui->record_textEdit->setReadOnly(true);
}

record::~record()
{
    delete ui;
}

void record::on_close_pushButton_clicked()
{
    this->close();
}
