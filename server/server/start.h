#ifndef START_H
#define START_H

#include <QMainWindow>
#include "chatting.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

namespace Ui {
class start;
}

class start : public QMainWindow
{
    Q_OBJECT

public:
    explicit start(QWidget *parent = 0);
    ~start();

private slots:
    void on_login_pushButton_clicked();

private:
    Ui::start *ui;
};

#endif // START_H
