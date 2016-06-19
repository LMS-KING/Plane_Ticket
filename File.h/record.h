#ifndef RECORD_H
#define RECORD_H

#include <QDialog>
//数据库头文件
#include <QSqlDatabase>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QSqlQuery>
#include <Qtsql>

#include <QStandardItem>

namespace Ui {
class Record;
}

class Record : public QDialog
{
    Q_OBJECT

public:
    explicit Record(QWidget *parent = 0);
    ~Record();

private slots:
    void timemove();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Record *ui;
    QSqlDatabase db;       //数据库连接
    QString dsn;
    QAbstractItemModel *model;//查询记录中特定的行列值
    QStandardItemModel *ticket_model;
};

#endif // RECORD_H
