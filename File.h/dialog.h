#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
    void timemove();

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void senddate(QString);

private:
    int i;      //文字滚动
    int user;      //判断用户登录成功与否
    int crl;   //判断管理员登录成功与否
    Ui::Dialog *ui;
};

#endif // DIALOG_H
