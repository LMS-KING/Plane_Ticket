#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QDialog>
//数据库头文件
#include <QSqlDatabase>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class schedule;
}

class schedule : public QDialog
{
    Q_OBJECT

public:
    explicit schedule(QWidget *parent = 0);
    ~schedule();

private slots:
    //bool eventFilter(QObject * watched,QEvent *event);       //事件过滤器

    void on_pushButton_clicked();

//    void planeStyle();        //实时判断机舱类别

    void on_pushButton_2_clicked();

protected:
    void paintEvent(QPaintEvent *);


private:
    Ui::schedule *ui;
    QString SeatStyle;      //机舱类型
    QString SeatStyles;
    QSqlDatabase db;       //数据库连接
    QString dsn;
    QString status;  //未订
    QString statusding;  //已订
    int seatnum;
};

#endif // SCHEDULE_H
