#include "control.h"
#include "ui_control.h"
//数据库头文件
#include <QSqlDatabase>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QSqlQuery>
#include <Qtsql>

#include <QErrorMessage>
#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <QPalette>
#include <QStringList>

Control::Control(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Control)
{
    ui->setupUi(this);
    i = 0;      //判断管理员操作
    position = 0;   //设置文字滚动
    ui->LEbianhao->installEventFilter(this);

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(artilemove()));
    timer->start(100);

    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    //QString strTime = time.toString("yyyy-MM-dd hh:mm:ss");//设置系统时间显示格式
    ui->dateTimeEdit->setDateTime(time);
    ui->dateTimeEdit_2->setDateTime(time);

    ui->LEbianhao->setReadOnly(true);
    ui->LEticketcount->setReadOnly(true);
    ui->combGO->setEditable(false);
    ui->combTO->setEditable(false);
    ui->dateTimeEdit->setEnabled(false);
    ui->dateTimeEdit_2->setEnabled(false);
    ui->jjcpiaoshu->setReadOnly(true);
    ui->swcpiaoshu->setReadOnly(true);
    ui->jjcpiaojia->setReadOnly(true);
    ui->swcpiaojia->setReadOnly(true);
    ui->GoAirport->setReadOnly(true);
    ui->ToAirport->setReadOnly(true);

    QSqlDatabase db;       //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QODBC");

    QString dsn = QString::fromLocal8Bit("DRIVER={SQL SERVER};SERVER=LMS-KING;DATABASE=Air_Plane");
    db.setDatabaseName(dsn);
    db.setUserName("sa");
    db.setPassword("521ai3849");
    if(!db.open())
    {
          qDebug()<<db.lastError().text();
          QMessageBox::critical(0,QObject::tr("Database Error"),db.lastError().text());
          return;
    }

    //设置字幕
    QStringList names;
    QString name;
    QSqlQuery query(db);
   //查询航班名称信息
    query.exec("select PlaneName from Air_Plane");

    while(query.next())
    {
        names<<query.value(0).toString();
    }
    name = names.join(",");

    ui->label_13->setText(tr("现有航班:")+name);

}

Control::~Control()
{
    delete ui;
}

void Control::on_pushButton_clicked()       //增加航班 i = 1
{
    i = 1;
    //改变空件可读性
    ui->LEbianhao->setReadOnly(false);
    ui->LEticketcount->setReadOnly(false);
    ui->combGO->setEditable(true);
    ui->combTO->setEditable(true);
    ui->combGO->setEnabled(true);
    ui->combTO->setEnabled(true);
    ui->dateTimeEdit->setEnabled(true);
    ui->dateTimeEdit_2->setEnabled(true);
    ui->jjcpiaoshu->setReadOnly(false);
    ui->swcpiaoshu->setReadOnly(false);
    ui->jjcpiaojia->setReadOnly(false);
    ui->swcpiaojia->setReadOnly(false);
    ui->GoAirport->setReadOnly(false);
    ui->ToAirport->setReadOnly(false);
    //清空当前信息
    ui->LEbianhao->clear();
    ui->LEticketcount->clear();
    ui->combGO->clearEditText();
    ui->combTO->clearEditText();
    ui->dateTimeEdit->clearMask();
    ui->dateTimeEdit_2->clear();
    ui->jjcpiaoshu->clear();
    ui->swcpiaoshu->clear();
    ui->jjcpiaojia->clear();
    ui->swcpiaojia->clear();
    ui->GoAirport->clear();
    ui->ToAirport->clear();
}

void Control::on_pushButton_2_clicked()     //修改航班 i = 2
{
    i = 2;
    //改变空件可读性
    ui->LEbianhao->setReadOnly(false);
    ui->LEticketcount->setReadOnly(false);
    ui->combGO->setEditable(true);
    ui->combTO->setEditable(true);
    ui->combGO->setEnabled(true);      //
    ui->combTO->setEnabled(true);
    ui->dateTimeEdit->setEnabled(true);
    ui->dateTimeEdit_2->setEnabled(true);
    ui->jjcpiaoshu->setReadOnly(false);
    ui->swcpiaoshu->setReadOnly(false);
    ui->jjcpiaojia->setReadOnly(false);
    ui->swcpiaojia->setReadOnly(false);
    ui->GoAirport->setReadOnly(false);
    ui->ToAirport->setReadOnly(false);
    //清空当前信息
    ui->LEbianhao->clear();
    ui->LEticketcount->clear();
    ui->combGO->clearEditText();
    ui->combTO->clearEditText();
    ui->dateTimeEdit->clear();
    ui->dateTimeEdit_2->clear();
    ui->jjcpiaoshu->clear();
    ui->swcpiaoshu->clear();
    ui->jjcpiaojia->clear();
    ui->swcpiaojia->clear();
    ui->GoAirport->clear();
    ui->ToAirport->clear();
}

void Control::on_pushButton_3_clicked()     //删除航班 i = 3
{
     i= 3;
     //改变空件可读性
     ui->LEbianhao->setReadOnly(false);
     ui->LEticketcount->setReadOnly(true);
     ui->combGO->setEditable(false);
     ui->combTO->setEditable(false);
     ui->dateTimeEdit->setEnabled(false);
     ui->dateTimeEdit_2->setEnabled(false);
     ui->jjcpiaoshu->setReadOnly(true);
     ui->swcpiaoshu->setReadOnly(true);
     ui->jjcpiaojia->setReadOnly(true);
     ui->swcpiaojia->setReadOnly(true);
     ui->GoAirport->setReadOnly(true);
     ui->ToAirport->setReadOnly(true);
     //清空当前信息
     ui->LEbianhao->clear();
     ui->LEticketcount->clear();
     ui->combGO->clearEditText();
     ui->combTO->clearEditText();
     ui->dateTimeEdit->clear();
     ui->dateTimeEdit_2->clear();
     ui->jjcpiaoshu->clear();
     ui->swcpiaoshu->clear();
     ui->jjcpiaojia->clear();
     ui->swcpiaojia->clear();
     ui->GoAirport->clear();
     ui->ToAirport->clear();
}

void Control::on_pushButton_4_clicked()     //查询航班 i = 4
{
    i= 4;
    //改变空件可读性
    ui->LEbianhao->setReadOnly(false);
    ui->LEticketcount->setReadOnly(true);
    ui->combGO->setEditable(false);
    ui->combTO->setEditable(false);
    ui->dateTimeEdit->setEnabled(false);
    ui->dateTimeEdit_2->setEnabled(false);
    ui->jjcpiaoshu->setReadOnly(true);
    ui->swcpiaoshu->setReadOnly(true);
    ui->jjcpiaojia->setReadOnly(true);
    ui->swcpiaojia->setReadOnly(true);
    ui->GoAirport->setReadOnly(true);
    ui->ToAirport->setReadOnly(true);
    //清空当前信息
    ui->LEbianhao->clear();
    ui->LEticketcount->clear();
    ui->combGO->clearEditText();
    ui->combTO->clearEditText();
    ui->dateTimeEdit->clear();
    ui->dateTimeEdit_2->clear();
    ui->jjcpiaoshu->clear();
    ui->swcpiaoshu->clear();
    ui->jjcpiaojia->clear();
    ui->swcpiaojia->clear();
    ui->GoAirport->clear();
    ui->ToAirport->clear();
}

void Control::on_pushButton_5_clicked()
{
    if(i == 0)
    {
        QErrorMessage *error = new QErrorMessage(this);
        error->setWindowTitle(tr("提示"));
        error->showMessage(tr("请选择操作"));
        return;
    }
    if(i == 1 || i == 2 || i == 3)      //增加航班，修改航班，删除航班需要再次连接数据库
    {
        QDateTime currenttime = QDateTime::currentDateTime();
        QDateTime GoTime = ui->dateTimeEdit->dateTime();
        QDateTime ToTime = ui->dateTimeEdit_2->dateTime();
        QString strGoTime;
        QString strToTime;
        strGoTime = GoTime.toString("yyyy-MM-dd hh:mm");
        strToTime = ToTime.toString("yyyy-MM-dd hh:mm");

        QSqlDatabase db;       //数据库连接
        if(QSqlDatabase::contains("qt_sql_default_connection"))
          db = QSqlDatabase::database("qt_sql_default_connection");
        else
          db = QSqlDatabase::addDatabase("QODBC");

        QString dsn = QString::fromLocal8Bit("DRIVER={SQL SERVER};SERVER=LMS-KING;DATABASE=Air_Plane");
        db.setDatabaseName(dsn);
        db.setUserName("sa");
        db.setPassword("521ai3849");
        if(!db.open())
        {
              qDebug()<<db.lastError().text();
              QMessageBox::critical(0,QObject::tr("Database Error"),db.lastError().text());
              return;
        }

        QSqlQuery query(db);
        if(i == 1)      //增加航班
        {
            //判断信息完整性
            if(ui->LEbianhao->text().isEmpty() || ui->LEticketcount->text().isEmpty() || ui->combGO->currentText().isEmpty() || ui->combTO->currentText().isEmpty() || ui->GoAirport->text().isEmpty() || ui->ToAirport->text().isEmpty() || ui->jjcpiaojia->text().isEmpty() || ui->jjcpiaoshu->text().isEmpty() || ui->swcpiaojia->text().isEmpty() || ui->swcpiaoshu->text().isEmpty())
            {
                QErrorMessage *error = new QErrorMessage(this);
                error->setWindowTitle(tr("提示"));
                error->showMessage(tr("请填写完整信息!"));
                return;
            }
            else if(ui->dateTimeEdit->dateTime() >= ui->dateTimeEdit_2->dateTime() || ui->dateTimeEdit->dateTime() <= currenttime)
            {
                QErrorMessage *error = new QErrorMessage(this);
                error->setWindowTitle(tr("提示"));
                error->showMessage(tr("航班时间信息有误!"));
                return;
            }
            //判断机票总数是否等于经济舱和商务舱的和
            else if((ui->jjcpiaoshu->text().toInt()+ui->swcpiaoshu->text().toInt()) != ui->LEticketcount->text().toInt())
            {
                QErrorMessage *error = new QErrorMessage(this);
                error->setWindowTitle(tr("提示"));
                error->showMessage(tr("机票数量有误!"));
                return;
            }

            //增加航班
            query.exec("select PlaenName from Air_Plane");
            while(query.next())
            {
                 if(query.value(0).toString() == ui->LEbianhao->text())
                 {
                     QErrorMessage *error = new QErrorMessage(this);
                     error->setWindowTitle(tr("提示"));
                     error->showMessage(tr("该航班已存在!"));
                     return;
                 }
            }

             //事务开始---更新航班信息表和机票信息表
            QSqlDatabase::database().transaction();

            //更新航班信息表
            query.prepare("insert into           Air_Plane(PlaneName,PlaneGo,PlaneTo,PlaneGoAirport,PlaneToAirport,PlaneGoTime,PlaneToTime,PlaneLowSeatCount,PlaneHighSeatCount,PlaneLowSeatPrice,PlaneHighSeatPrice) values(?,?,?,?,?,?,?,?,?,?,?)");

            query.addBindValue(tr("%1").arg(ui->LEbianhao->text()));
            query.addBindValue(tr("%1").arg(ui->combGO->currentText()));
            query.addBindValue(tr("%1").arg(ui->combTO->currentText()));
            query.addBindValue(tr("%1").arg(ui->GoAirport->text()));
            query.addBindValue(tr("%1").arg(ui->ToAirport->text()));
            query.addBindValue(tr("%1").arg(strGoTime));
            query.addBindValue(tr("%1").arg(strToTime));
            query.addBindValue(tr("%1").arg(ui->jjcpiaoshu->text().toInt()));
            query.addBindValue(tr("%1").arg(ui->swcpiaoshu->text().toInt()));
            query.addBindValue(tr("%1").arg(ui->jjcpiaojia->text().toInt()));
            query.addBindValue(tr("%1").arg(ui->swcpiaojia->text().toInt()));
            query.exec();

            //更新机票信息表
            QString strjjc = "经济舱";
            QString strswc = "商务舱";
            int k = 1;
            query.prepare("insert into Air_Ticket(TicketSeat,TicketStyle,TicketStatus,PlaneName) values(?,?,?,?)");
            for(k;k <= ui->swcpiaoshu->text().toInt();k++)
            {
               query.addBindValue(k);
               query.addBindValue(tr("%1").arg(strswc));
               query.addBindValue("0");
               query.addBindValue(tr("%1").arg(ui->LEbianhao->text()));
               query.exec();
            }
            for(k;k <= ui->LEticketcount->text().toInt();k++)
            {
               query.addBindValue(k);
               query.addBindValue(tr("%1").arg(strjjc));
               query.addBindValue("0");
               query.addBindValue(tr("%1").arg(ui->LEbianhao->text()));
               query.exec();
            }
            QSqlDatabase::database().commit();//事务结束


            QErrorMessage *error = new QErrorMessage(this);
            error->setWindowTitle(tr("提示"));
            error->showMessage(tr("增加航班成功!"));
            return;

        }
        else if(i == 2)     //修改航班
        {
            int yn = QMessageBox::question(this,tr("提示"),tr("是否确定修改该航班？"),QMessageBox::Yes,QMessageBox::No);
            if(yn == QMessageBox::Yes)      //确定修改，更新数据库
            {
                if(ui->LEticketcount->text().toInt() != (ui->jjcpiaoshu->text().toInt()+ui->swcpiaoshu->text().toInt()))
                {
                    QErrorMessage *error = new QErrorMessage(this);
                    error->setWindowTitle(tr("提示"));
                    error->showMessage(tr("修改航班座位数量有误!"));
                    return;
                }
                 //事务开始---更新航班信息表
                QSqlDatabase::database().transaction();


             //先删除
                query.exec("delete from Air_Plane where PlaneName = '" + ui->LEbianhao->text() +"'");
                query.exec("delete from Air_Ticket where PlaneName = '" + ui->LEbianhao->text() +"'");
                //再重新插入
                query.prepare("insert into           Air_Plane(PlaneName,PlaneGo,PlaneTo,PlaneGoAirport,PlaneToAirport,PlaneGoTime,PlaneToTime,PlaneLowSeatCount,PlaneHighSeatCount,PlaneLowSeatPrice,PlaneHighSeatPrice) values(?,?,?,?,?,?,?,?,?,?,?)");
                query.addBindValue(tr("%1").arg(ui->LEbianhao->text()));
                query.addBindValue(tr("%1").arg(ui->combGO->currentText()));
                query.addBindValue(tr("%1").arg(ui->combTO->currentText()));
                query.addBindValue(tr("%1").arg(ui->GoAirport->text()));
                query.addBindValue(tr("%1").arg(ui->ToAirport->text()));
                query.addBindValue(tr("%1").arg(strGoTime));
                query.addBindValue(tr("%1").arg(strToTime));
                query.addBindValue(tr("%1").arg(ui->jjcpiaoshu->text().toInt()));
                query.addBindValue(tr("%1").arg(ui->swcpiaoshu->text().toInt()));
                query.addBindValue(tr("%1").arg(ui->jjcpiaojia->text().toInt()));
                query.addBindValue(tr("%1").arg(ui->swcpiaojia->text().toInt()));
                query.exec();

                //更新机票信息表
                QString strjjc = "经济舱";
                QString strswc = "商务舱";
                int j = 1;
                query.prepare("insert into Air_Ticket(TicketSeat,TicketStyle,TicketStatus,PlaneName) values(?,?,?,?)");
                for(j;j <= ui->swcpiaoshu->text().toInt();j++)
                {
                   query.addBindValue(j);
                   query.addBindValue(tr("%1").arg(strswc));
                   query.addBindValue("0");
                   query.addBindValue(tr("%1").arg(ui->LEbianhao->text()));
                   query.exec();
                }
                for(j;j <= ui->LEticketcount->text().toInt();j++)
                {
                   query.addBindValue(j);
                   query.addBindValue(tr("%1").arg(strjjc));
                   query.addBindValue("0");
                   query.addBindValue(tr("%1").arg(ui->LEbianhao->text()));
                   query.exec();
                }
                QSqlDatabase::database().commit();//事务结束

                QErrorMessage *error = new QErrorMessage(this);
                error->setWindowTitle(tr("提示"));
                error->showMessage(tr("修改航班成功!"));
                return;

            }
            else
                return;
        }
        else if(i == 3)     //删除航班
        {
            int yn = QMessageBox::question(this,tr("提示"),tr("是否确定删除该航班？"),QMessageBox::Yes,QMessageBox::No);
            if(yn == QMessageBox::Yes)      //确定修改，更新数据库
            {
                  query.exec("delete from Air_Plane where PlaneName = '" + ui->LEbianhao->text() +"'");
                  query.exec("delete from Air_Ticket where PlaneName = '" + ui->LEbianhao->text() +"'");
                  QErrorMessage *error = new QErrorMessage(this);
                  error->setWindowTitle(tr("提示"));
                  error->showMessage(tr("删除航班成功!"));
                  //清空当前信息
                  ui->LEbianhao->clear();
                  ui->LEticketcount->clear();
                  ui->combGO->clearEditText();
                  ui->combTO->clearEditText();
                  ui->dateTimeEdit->clear();
                  ui->dateTimeEdit_2->clear();
                  ui->jjcpiaoshu->clear();
                  ui->swcpiaoshu->clear();
                  ui->jjcpiaojia->clear();
                  ui->swcpiaojia->clear();
                  ui->GoAirport->clear();
                  ui->ToAirport->clear();
                  return;
            }
            else
                return;
        }
    }
    else if(i == 4)     //查询航班不用再次连接数据库
    {
        return;
    }
    else
    {
        QErrorMessage *error = new QErrorMessage(this);
        error->setWindowTitle(tr("提示"));
        error->showMessage(tr("未知错误!"));
        return;
    }
}

bool Control::eventFilter(QObject *watched, QEvent *event)  //航班输入框失去焦点的事件判断
{
    if(watched == ui->LEbianhao)     //航班编号输入框
    {
            if(!ui->LEbianhao->isReadOnly())       //输入框可编辑时触发失去焦点事件
            {
            if(event->type() == QEvent::FocusOut)     //控件失去焦点事件
            {
                if(i == 0 || i == 1)        //如果是增加航班，忽略失去焦点事件
                    return true;
                else      //否则显示航班信息
                {
                    QSqlDatabase db;       //数据库连接
                    if(QSqlDatabase::contains("qt_sql_default_connection"))
                    db = QSqlDatabase::database("qt_sql_default_connection");
                    else
                    db = QSqlDatabase::addDatabase("QODBC");

                    QString dsn = QString::fromLocal8Bit("DRIVER={SQL SERVER};SERVER=LMS-KING;DATABASE=Air_Plane");
                    db.setDatabaseName(dsn);
                    db.setUserName("sa");
                    db.setPassword("521ai3849");

                    if(!db.open())
                    {
                        qDebug()<<db.lastError().text();
                        QMessageBox::critical(0,QObject::tr("Database Error"),db.lastError().text());
                      return false;
                    }

                    QErrorMessage *error = new QErrorMessage(this);     //提示修改信息
                    error->setWindowTitle(tr("提示"));
                    error->showMessage(tr("航班原始信息"));

                    QSqlQuery query(db);
                    query.exec("select *from Air_Plane");

                    if(i == 2)      //如果是更改航班
                    {
                        ui->LEbianhao->setReadOnly(true);       //航班号不可改
                        while(query.next())
                        {
                            if(query.value(1).toString().trimmed() == ui->LEbianhao->text())
                            {
                                ui->LEticketcount->setText(QString::number(query.value(8).toInt()+query.value(9).toInt()));
                                ui->combGO->setCurrentText(query.value(2).toString().trimmed());
                                ui->combTO->setCurrentText(query.value(3).toString().trimmed());
                                ui->dateTimeEdit->setDateTime(query.value(6).toDateTime());
                                ui->dateTimeEdit_2->setDateTime(query.value(7).toDateTime());
                                ui->jjcpiaoshu->setText(query.value(8).toString().trimmed());
                                ui->swcpiaoshu->setText(query.value(9).toString().trimmed());
                                ui->jjcpiaojia->setText(query.value(10).toString().trimmed());
                                ui->swcpiaojia->setText(query.value(11).toString().trimmed());
                                ui->GoAirport->setText(query.value(4).toString().trimmed());
                                ui->ToAirport->setText(query.value(5).toString().trimmed());
                                break;
                            }
                        }
                    }
                    else        //否则先要设置地名下拉框的可读性
                    {
                        while(query.next())
                        {
                            if(query.value(1).toString().trimmed() == ui->LEbianhao->text())
                            {
                                ui->LEticketcount->setText(QString::number(query.value(8).toInt()+query.value(9).toInt()));
                                ui->combGO->setEditable(true);      //
                                ui->combTO->setEditable(true);      //
                                ui->combGO->setCurrentText(query.value(2).toString().trimmed());
                                ui->combTO->setCurrentText(query.value(3).toString().trimmed());
                                ui->combGO->setEnabled(false);      //
                                ui->combTO->setEnabled(false);      //
                                ui->dateTimeEdit->setDateTime(query.value(6).toDateTime());
                                ui->dateTimeEdit_2->setDateTime(query.value(7).toDateTime());
                                ui->jjcpiaoshu->setText(query.value(8).toString().trimmed());
                                ui->swcpiaoshu->setText(query.value(9).toString().trimmed());
                                ui->jjcpiaojia->setText(query.value(10).toString().trimmed());
                                ui->swcpiaojia->setText(query.value(11).toString().trimmed());
                                ui->GoAirport->setText(query.value(4).toString().trimmed());
                                ui->ToAirport->setText(query.value(5).toString().trimmed());
                                break;
                            }
                        }
                    }

                    if(ui->LEticketcount->text().isEmpty())
                    {
                        QErrorMessage *error = new QErrorMessage(this);     //提示修改信息
                        error->setWindowTitle(tr("提示"));
                        error->showMessage(tr("航班不存在!"));
                    }

                    ui->LEbianhao->setReadOnly(true);       //航班编号不可变
                }
            }
            else
                return false;
        }
            else
                return false;
    }
    else
        return Control::eventFilter(watched, event);
}

void Control::on_pushButton_6_clicked()
{
    this->close();
}

void Control::artilemove()      //字幕滚动
{
    position += 3;
    if(position != 600)
        ui->label_13->move(600-position,42);
    else
    {
        ui->label_13->move(position,42);
        position = 0;
    }
}
