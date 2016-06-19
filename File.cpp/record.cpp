#include "record.h"
#include "ui_record.h"
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QTimer>
#include <QDateTime>
#include <QErrorMessage>
#include <QMessageBox>

//主窗口传递的参数
extern QString mess;
extern QString hangbanNum;

Record::Record(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Record)
{
    ui->setupUi(this);
    ui->label->hide();

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timemove()));//关联定时器计满信号和相应的槽函数
    timer->start(100);  //定时器开始计时，其中100表示100ms

    ticket_model = new QStandardItemModel();

       ticket_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("航班编号")));
       ticket_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("出发地")));
       ticket_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("目的地")));
       ticket_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("起飞时间")));
       ticket_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("到达时间")));
       ticket_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("机票类别")));
       ticket_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("机票座位")));
       ticket_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("机票票价")));
       ticket_model->setHorizontalHeaderItem(8, new QStandardItem(QObject::tr("操作时间")));
 ticket_model->setHorizontalHeaderItem(9, new QStandardItem(QObject::tr("机票状态")));

       //利用setModel()方法将数据模型与QTableView绑定
       ui->tableView->setModel(ticket_model);
       //整行选择
       ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

          //设置时间显示列的宽度
          ui->tableView->setColumnWidth(3,150);
          ui->tableView->setColumnWidth(4,150);
          ui->tableView->setColumnWidth(8,150);
        //表不可编辑
       ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
       ui->tableView->sortByColumn(10,Qt::AscendingOrder);

    //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QODBC");

    dsn = QString::fromLocal8Bit("DRIVER={SQL SERVER};SERVER=LMS-KING;DATABASE=Air_Plane");
    db.setDatabaseName(dsn);
    db.setUserName("sa");
    db.setPassword("521ai3849");
    if(!db.open())
    {
          qDebug()<<db.lastError().text();
          QMessageBox::critical(0,QObject::tr("Database Error"),db.lastError().text());
          return;
    }

    int i = 0;
    QSqlQuery query(db);
    //先查询订票记录表，给出座位号
    query.exec("select TicketSeat,PlaneName from Air_Record where UserName = '"+mess+"'");
    while(query.next())
    {
        ticket_model->setItem(i, 6, new QStandardItem(query.value(0).toString().trimmed()));

        //再根据座位号和航班号查询机票类型和订票时间及机票状态
        QSqlQuery query1(db);
        query1.exec("select TicketStyle,TicketTime,TicketStatus,TicketTimes from Air_Ticket where TicketSeat  = '"+QString::number(query.value(0).toInt())+"' and PlaneName = '"+query.value(1).toString().trimmed()+"'");

        while(query1.next())
        {
            ticket_model->setItem(i, 5, new QStandardItem(query1.value(0).toString().trimmed()));
            if(query1.value(2).toString().trimmed() == "1")
            {
                ticket_model->setItem(i, 9, new QStandardItem(tr("已预订")));
                ticket_model->setItem(i, 8, new QStandardItem(query1.value(1).toString().trimmed()));
                //设置状态单元格为红色
                ticket_model->item(i, 9)->setForeground(QBrush(QColor(0,0,255)));
            }
            else if(query1.value(2).toString().trimmed() == "2")
            {
                ticket_model->setItem(i, 9, new QStandardItem(tr("已退票")));
                ticket_model->setItem(i, 8, new QStandardItem(query1.value(3).toString().trimmed()));
                 //设置状态单元格为红色
                 ticket_model->item(i, 9)->setForeground(QBrush(QColor(255, 0, 0)));
            }
            else if(query1.value(2).toString().trimmed() == "3")
            {
                ticket_model->setItem(i, 9, new QStandardItem(tr("已改签")));
                ticket_model->setItem(i, 8, new QStandardItem(query1.value(3).toString().trimmed()));
                //设置状态单元格为红色
                ticket_model->item(i, 9)->setForeground(QBrush(QColor(0, 255, 0)));
            }
            else
            {
                ticket_model->setItem(i, 9, new QStandardItem(tr("错误")));
            }

            //最后查询飞机表
            QSqlQuery query2(db);
            if(query1.value(0).toString().trimmed() == "经济舱")
            {
                query2.exec("select PlaneName,PlaneGo,PlaneTo,PlaneGoTime,PlaneTotime,PlaneLowSeatPrice from Air_Plane where PlaneName = '"+query.value(1).toString().trimmed()+"'");
            }
            else if(query1.value(0).toString().trimmed() == "商务舱")
            {
                query2.exec("select PlaneName,PlaneGo,PlaneTo,PlaneGoTime,PlaneTotime,PlaneHighSeatPrice from Air_Plane where PlaneName = '"+query.value(1).toString().trimmed()+"'");
            }  
            else
            {
                QErrorMessage *error = new QErrorMessage(this);
                error->setWindowTitle(tr("提示"));
                error->showMessage(tr("未知错误！"));
                return;
            }

            while(query2.next())
            {
                 ticket_model->setItem(i, 0, new QStandardItem(query2.value(0).toString().trimmed()));
                  ticket_model->setItem(i, 1, new QStandardItem(query2.value(1).toString().trimmed()));
                   ticket_model->setItem(i, 2, new QStandardItem(query2.value(2).toString().trimmed()));
                    ticket_model->setItem(i, 3, new QStandardItem(query2.value(3).toString().trimmed()));
                     ticket_model->setItem(i, 4, new QStandardItem(query2.value(4).toString().trimmed()));
                      ticket_model->setItem(i, 7, new QStandardItem(query2.value(5).toString().trimmed()));
            }
        }
        //设置单元格文本居中
           ticket_model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
           ticket_model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
           ticket_model->item(i, 2)->setTextAlignment(Qt::AlignCenter);
           ticket_model->item(i, 3)->setTextAlignment(Qt::AlignCenter);
           ticket_model->item(i, 4)->setTextAlignment(Qt::AlignCenter);
           ticket_model->item(i, 5)->setTextAlignment(Qt::AlignCenter);
           ticket_model->item(i, 6)->setTextAlignment(Qt::AlignCenter);
           ticket_model->item(i, 7)->setTextAlignment(Qt::AlignCenter);
           ticket_model->item(i, 8)->setTextAlignment(Qt::AlignCenter);
           ticket_model->item(i, 9)->setTextAlignment(Qt::AlignCenter);          

        i++;
    }
    if(i == 0)
    {
        ui->label->show();
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        return;
    }
}

Record::~Record()
{
    delete ui;
}

void Record::timemove()     //时间触发器函数
{
    model = ui->tableView->model();
    int curow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(curow,9);
    QVariant data = model->data(index);
    if(data.toString().trimmed() == "已退票" || data.toString().trimmed() == "已改签")
    {
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
    }
    else if(data.toString().trimmed() == "已预订")
    {
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
    }
}

void Record::on_pushButton_3_clicked()      //删除记录
{
    QDateTime rectime = QDateTime::currentDateTime();

    model = ui->tableView->model();
    int curow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(curow,0);
    QVariant data = model->data(index);
    QString recd = data.toString().trimmed();

    if(recd.isEmpty())
    {
        QErrorMessage *error = new QErrorMessage(this);
        error->setWindowTitle(tr("提示"));
        error->showMessage(tr("请选择操作对象!"));
        return;
    }
    QSqlQuery query(db);
    query.exec("select PlaneGoTime from Air_Plane where PlaneName = '"+recd+"'");
    while(query.next())
    {
        if(query.value(0).toDateTime() > rectime)
        {
            QErrorMessage *error = new QErrorMessage(this);
            error->setWindowTitle(tr("提示"));
            error->showMessage(tr("该航班还未起飞，该记录暂不可删除!"));
            return;
        }
    }

    bool tf = query.exec("delete from Air_Record where PlaneName = '"+recd+"'");
    if(tf)
    {
        int curow = ui->tableView->currentIndex().row();
        model->removeRow(curow);

        QErrorMessage *error = new QErrorMessage(this);
        error->setWindowTitle(tr("提示"));
        error->showMessage(tr("删除成功!"));
        return;
    }
    else
    {
        qDebug()<<recd;
        QErrorMessage *error = new QErrorMessage(this);
        error->setWindowTitle(tr("提示"));
        error->showMessage(tr("删除未完成!"));
        return;
    }
}

void Record::on_pushButton_clicked()        //退票
{
    QDateTime rectime = QDateTime::currentDateTime();

    model = ui->tableView->model();
    int curow = ui->tableView->currentIndex().row();
    //data为航班名
    QModelIndex index = model->index(curow,0);
    QVariant data = model->data(index);
    //datas为机舱类型
    QModelIndex indexs = model->index(curow,5);
    QVariant datas = model->data(indexs);
    //dataseat为座位号
    QModelIndex indexss = model->index(curow,6);
    QVariant dataseat = model->data(indexss);

    if(!ui->label->isHidden())      //没有记录，退，改删都不可执行
    {
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        return;
    }
    else if(data.toString().trimmed().isEmpty())
    {
        QErrorMessage *error = new QErrorMessage(this);
        error->setWindowTitle(tr("提示"));
        error->showMessage(tr("请选择操作对象!"));
        return;
    }

    QString strstatus = "2";       //机票状态:退票

    QSqlQuery query(db);
    //更新飞机信息表
    query.exec("select PlaneGoTime,PlaneLowSeatCount,PlaneHighSeatCount from Air_Plane where PlaneName = '"+data.toString().trimmed()+"'");
    while(query.next())
    {
        QSqlQuery query1(db);
        if(query.value(0).toDateTime() < rectime)
        {
            QErrorMessage *error = new QErrorMessage(this);
            error->setWindowTitle(tr("提示"));
            error->showMessage(tr("对不起，该航班已起飞，无法退票!"));
            return;
        }
        if(datas.toString().trimmed() == "经济舱")      //经济舱座位加一
        {
            query1.exec("update Air_Plane set PlaneLowSeatCount = '"+QString::number((query.value(1).toInt()+1))+"' where PlaneName = '"+data.toString().trimmed()+"'");
        }
        else if(datas.toString().trimmed() == "商务舱")            //商务舱座位加一
        {
            query1.exec("update Air_Plane set PlaneHighSeatCount = '"+QString::number((query.value(2).toInt()+1))+"' where PlaneName = '"+data.toString().trimmed()+"'");
        }
        else
        {
            QErrorMessage *error = new QErrorMessage(this);
            error->setWindowTitle(tr("提示"));
            error->showMessage(tr("退票未完成!"));
            return;
        }
    }

    //事务开始
    QSqlDatabase::database().transaction();

    //更新机票状态
        query.exec("delete from Air_Ticket where TicketSeat = '"+QString::number(dataseat.toInt())+"' and PlaneName = '"+data.toString().trimmed()+"'");

        //插入
        query.prepare("insert into Air_Ticket(TicketSeat,TicketStyle,TicketStatus,TicketTimes,PlaneName) values(?,?,?,?,?)");
        query.addBindValue(tr("%1").arg(dataseat.toInt()));
        query.addBindValue(tr("%1").arg(datas.toString().trimmed()));
        query.addBindValue(tr("%1").arg(strstatus));
        query.addBindValue(tr("%1").arg(rectime.toString("yyyy-MM-dd hh:mm")));
        query.addBindValue(tr("%1").arg(data.toString().trimmed()));
        query.exec();


    //更新订票记录
    query.exec("update Air_Record set TicketStatus = '"+strstatus+"' where PlaneName = '"+data.toString().trimmed()+"'");

     QSqlDatabase::database().commit();//事务结束

    QErrorMessage *error = new QErrorMessage(this);
    error->setWindowTitle(tr("提示"));
    error->showMessage(tr("退票完成!"));

    ticket_model->setItem(curow, 9, new QStandardItem(tr("已退票")));
    //设置状态单元格为红色
    ticket_model->item(curow, 9)->setForeground(QBrush(QColor(255, 0, 0)));
    //居中
    ticket_model->item(curow, 9)->setTextAlignment(Qt::AlignCenter);

    return;
}

void Record::on_pushButton_2_clicked()      //改签
{
    QDateTime rectime = QDateTime::currentDateTime();

    model = ui->tableView->model();
    int curow = ui->tableView->currentIndex().row();
    //data为航班名
    QModelIndex index = model->index(curow,0);
    QVariant data = model->data(index);
    //datastyle为机舱类型
    QModelIndex indexs = model->index(curow,5);
    QVariant datastyle = model->data(indexs);
    //dataseat为座位号
    QModelIndex indexss = model->index(curow,6);
    QVariant dataseat = model->data(indexss);

    QString strweiding = "0";   //机票状态：未订
    QString strding = "1";      //机票状态：已订
    QString strstatus = "3";       //机票状态：改签

    if(data.toString().trimmed().isEmpty())
    {
        QErrorMessage *error = new QErrorMessage(this);
        error->setWindowTitle(tr("提示"));
        error->showMessage(tr("请选择操作对象!"));
        return;
    }

    QSqlQuery query(db);
    query.exec("select PlaneGo,PlaneTo,PlaneGoTime,PlaneLowSeatCount,PlaneHighSeatCount  from Air_Plane where PlaneName = '"+data.toString().trimmed()+"'");
    while(query.next())
    {
        QSqlQuery query1(db);   //查询符合条件的航班号
        query1.exec("select PlaneName,PlaneLowSeatCount,PlaneHighSeatCount,PlaneGoTime from Air_Plane where PlaneGo = '"+query.value(0).toString().trimmed()+"' and PlaneTo = '"+query.value(1).toString().trimmed()+"' and PlaneGoTime > '"+query.value(2).toString().trimmed()+"'");
        while(query1.next())
        {
              QSqlQuery query5(db);
              query5.exec("select PlaneName from Air_Record where UserName = '"+mess+"' and TicketStatus = '"+strding.trimmed()+"'");
              while(query5.next())
              {
                  if(query5.value(0).toString().trimmed() == query1.value(0).toString().trimmed())
                  {
                      QErrorMessage *error = new QErrorMessage(this);
                      error->setWindowTitle(tr("提示"));
                      error->showMessage(tr("没有可改签的航班!"));
                      return;
                  }
              }

                 //先判断符合改签条件的航班是否有余票
                  QSqlQuery query2(db);
                  query2.exec("select COUNT(*) from Air_Ticket where PlaneName = '"+query1.value(0).toString().trimmed()+"' and TicketStatus = '"+strweiding.trimmed()+"'");
                  while(query2.next())
                  {
                      if(query2.value(0).toInt() > 0)       //有余票
                      {
                          int sure = QMessageBox::question(this,tr("改签确认"),tr("可为您改签至航班：%1\n起飞时间：%2\n所有机票只可改签一次").arg(query1.value(0).toString().trimmed(),query1.value(3).toString().trimmed()),QMessageBox::Yes,QMessageBox::No);
                          if(sure == QMessageBox::No)
                          {
                              QErrorMessage *error = new QErrorMessage(this);
                              error->setWindowTitle(tr("提示"));
                              error->showMessage(tr("改签未完成!"));
                              return;
                          }

                         QSqlQuery query3(db);

                             query3.exec("select TicketSeat from Air_Ticket where PlaneName = '"+query1.value(0).toString().trimmed()+"' and TicketStyle = '"+datastyle.toString().trimmed()+"' and TicketStatus = '"+strweiding.trimmed()+"'");

                             while(query3.next())
                             {
                                   if(query3.value(0).toInt() > 0)
                                   {
                                       QSqlQuery query4(db);     //更新改签到的航班机票表
                                       query4.exec("update Air_Ticket set TicketStatus = '"+strding.trimmed()+"',TicketTime = '"+rectime.toString("yyyy-MM-dd hh:mm")+"' where PlaneName = '"+query1.value(0).toString().trimmed()+"' and TicketSeat = '"+QString::number(query3.value(0).toInt())+"'");//此处TicketTime为订票时间
                                       //更新改签的航班机票表
                                           query4.exec("delete from Air_Ticket where TicketSeat = '"+QString::number(dataseat.toInt())+"' and PlaneName = '"+data.toString().trimmed()+"'");
                                           //插入
                                           query4.prepare("insert into Air_Ticket(TicketSeat,TicketStyle,TicketStatus,TicketTimes,PlaneName) values(?,?,?,?,?)");
                                           query4.addBindValue(tr("%1").arg(dataseat.toInt()));
                                           query4.addBindValue(tr("%1").arg(datastyle.toString().trimmed()));
                                           query4.addBindValue(tr("%1").arg(strstatus));
                                           query4.addBindValue(tr("%1").arg(rectime.toString("yyyy-MM-dd hh:mm")));
                                           query4.addBindValue(tr("%1").arg(data.toString().trimmed()));
                                           query4.exec();

                                   //更新改签的航班订票记录表
                                        query4.exec("update Air_Record set TicketStatus = '"+strstatus.trimmed()+"' where PlaneName = '"+data.toString().trimmed()+"' and TicketSeat = '"+QString::number(dataseat.toInt())+"'");
                                        //订票记录插入新数据
                                        query4.prepare("insert into Air_Record(UserName,PlaneName,TicketSeat,TicketStatus) values(?,?,?,?)");
                                        query4.addBindValue(tr("%1").arg(mess));
                                        query4.addBindValue(tr("%1").arg(query1.value(0).toString().trimmed()));
                                        query4.addBindValue(tr("%1").arg(query3.value(0).toString().trimmed()));
                                        query4.addBindValue(tr("%1").arg(strding.trimmed()));

                                        query4.exec();

                                        //更新改签到的航班表
                                        if(datastyle.toString().trimmed() == "经济舱")
                                        {
                                            query4.exec("update Air_Plane set PlaneLowSeatCount = '"+QString::number(query1.value(1).toInt()-1)+"' where PlaneName = '"+query1.value(0).toString().trimmed()+"'");
                                        //更新改签的航班表
                                            query4.exec("update Air_Plane set PlaneLowSeatCount = '"+QString::number(query.value(3).toInt()+1)+"' where PlaneName =  '"+data.toString().trimmed()+"'");
                                        }
                                        else if(datastyle.toString().trimmed() == "商务舱")
                                        {
                                            query4.exec("update Air_Plane set PlaneHighSeatCount = '"+QString::number(query1.value(2).toInt()-1)+"' where PlaneName = '"+query1.value(0).toString().trimmed()+"'");

                                        //更新改签的航班表
                                            query4.exec("update Air_Plane set PlaneHighSeatCount = '"+QString::number(query.value(4).toInt()+1)+"' where PlaneName =  '"+data.toString().trimmed()+"'");
                                        }

                                        else
                                        {
                                            qDebug()<<"nnnnnnnnnnn";
                                        }

                                        int sures = QMessageBox::information(this,tr("提示"),tr("已为您改签到航班:%1,座位号:%2").arg(query1.value(0).toString().trimmed(),query3.value(0).toString().trimmed()),QMessageBox::Ok);
                                        if(sures == QMessageBox::Ok)
                                        {
                                            ticket_model->setItem(curow, 9, new QStandardItem(tr("已改签")));
                                            //设置状态单元格为红色
                                            ticket_model->item(curow, 9)->setForeground(QBrush(QColor(255, 0, 0)));
                                            //居中
                                            ticket_model->item(curow, 9)->setTextAlignment(Qt::AlignCenter);

                                        return;
                                        }

                                   }
                                   else
                                   {
                                        QErrorMessage *error = new QErrorMessage(this);
                                        error->setWindowTitle(tr("提示"));
                                        error->showMessage(tr("改签未完成!"));
                                        return;
                                   }
                             }

                             QErrorMessage *error = new QErrorMessage(this);
                             error->setWindowTitle(tr("提示"));
                             error->showMessage(tr("改签未完成!"));
                             return;
                      }
                      else
                      {
                          QErrorMessage *error = new QErrorMessage(this);
                          error->setWindowTitle(tr("提示"));
                          error->showMessage(tr("改签未完成!"));
                          return;
                      }
                  }

                  QErrorMessage *error = new QErrorMessage(this);
                  error->setWindowTitle(tr("提示"));
                  error->showMessage(tr("改签未完成!"));
                  return;
        }

        QErrorMessage *error = new QErrorMessage(this);
        error->setWindowTitle(tr("提示"));
        error->showMessage(tr("没有可改签的航班!"));
        return;
    }
}

void Record::on_pushButton_4_clicked()      //退出
{
      this->close();
}
