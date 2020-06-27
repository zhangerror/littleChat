#ifndef RECORD_H
#define RECORD_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

namespace Ui {
class record;
}

class record : public QDialog
{
    Q_OBJECT

public:
    explicit record(QWidget *parent = 0);
    ~record();

private slots:
    void on_close_pushButton_clicked();

private:
    Ui::record *ui;
};

#endif // RECORD_H
