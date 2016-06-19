#ifndef TICKETMESSAGE_H
#define TICKETMESSAGE_H

#include <QDialog>
//数据库头文件
#include <QSqlDatabase>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class TicketMessage;
}

class TicketMessage : public QDialog
{
    Q_OBJECT

public:
    explicit TicketMessage(QWidget *parent = 0);
    ~TicketMessage();

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

private:
    Ui::TicketMessage *ui;
    QSqlDatabase db;       //数据库连接
    QString dsn;
};

#endif // TICKETMESSAGE_H
