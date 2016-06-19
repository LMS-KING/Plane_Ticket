#ifndef PASSWORD_H
#define PASSWORD_H

#include <QDialog>
//数据库头文件
#include <QSqlDatabase>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class password;
}

class password : public QDialog
{
    Q_OBJECT

public:
    explicit password(QWidget *parent = 0);
    ~password();

protected:
    void paintEvent(QPaintEvent *);

private slots:
       bool eventFilter(QObject * watched,QEvent *event);       //事件过滤器

       void on_pushButton_clicked();

       void on_pushButton_2_clicked();

       void showpassw();

private:
    Ui::password *ui;
    QSqlDatabase db;
    QString dsn;
    int i;
};

#endif // PASSWORD_H
