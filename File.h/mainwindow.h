#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//数据库头文件
#include <QSqlDatabase>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void timemove();

    void recdate(QString date);

    void recmessage();

private slots:

    void on_action_triggered();             //登录

    void on_action_8_triggered();           //注册

    void on_action_6_triggered();           //退出

    void on_btntime_clicked();              //选择时间

    void on_btnchaxun_clicked();            //查询航班

    void peoplemessage();               //个人信息查询

    void ticketsmessage();                  //旅客订票记录

    void checkpaw();                    //修改密码

    void peopleknow();                  //旅客乘机须知

    void saout();                       //安全退出

    void on_btnchaxun_2_clicked();

    void on_action_3_triggered();

    void on_action_7_triggered();

private:
    Ui::MainWindow *ui;
    int i;      //文字滚动
    int j;      //时间日历
    QSqlDatabase db;       //数据库连接
    QString dsn;
};

#endif // MAINWINDOW_H
