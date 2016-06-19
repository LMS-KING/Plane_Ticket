#ifndef PEOPLEMESSAGE_H
#define PEOPLEMESSAGE_H

#include <QDialog>
//数据库头文件
#include <QSqlDatabase>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class Peoplemessage;
}

class Peoplemessage : public QDialog
{
    Q_OBJECT

public:
    explicit Peoplemessage(QWidget *parent = 0);
    ~Peoplemessage();

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void on_pushButton_clicked();

    bool eventFilter(QObject * watched,QEvent *event);       //事件过滤器

    void on_pushButton_2_clicked();

private:
    Ui::Peoplemessage *ui;
    QString passw;      //记录密码
    bool tf;            //记录状态
    int i;              //记录购票数量
    int j;              //标记修改是否合法
    QSqlDatabase db;       //数据库连接
    QString dsn;
};

#endif // PEOPLEMESSAGE_H
