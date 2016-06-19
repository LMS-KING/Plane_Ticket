#include "schedule.h"
#include "ui_schedule.h"
#include <QPainter>
#include <QErrorMessage>
#include <QDateTime>
#include <QDebug>

//主窗口传递的参数
extern QString mess;
extern QString hangbanNum;

//按地点查询时机票窗口传递的参数
//extern QString HBN;
//extern QString People;


schedule::schedule(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::schedule)
{
    ui->setupUi(this);
    status = "0";  //未订
    statusding = "1";  //已订

    ui->jjps->setReadOnly(true);
    ui->jjpj->setReadOnly(true);
    ui->swpj->setReadOnly(true);
    ui->swps->setReadOnly(true);
    ui->seatnum->setReadOnly(true);

   //ui->style->installEventFilter(this);


    //  this->setWindowOpacity(0.8);      //窗体透明
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

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

    QSqlQuery query(db);
    query.exec("select PlaneLowSeatCount,PlaneHighSeatCount"
               ",PlaneLowSeatPrice,PlaneHighSeatPrice from Air_Plane where PlaneName = '"+hangbanNum+"'");

    while(query.next())
    {
        ui->jjps->setText(query.value(0).toString().trimmed());
        ui->jjpj->setText(query.value(2).toString().trimmed());
        ui->swps->setText(query.value(1).toString().trimmed());
        ui->swpj->setText(query.value(3).toString().trimmed());
    }
}

schedule::~schedule()
{
    delete ui;
}

//bool schedule::eventFilter(QObject *watched, QEvent *event)     //事件过滤函数
//{
//    if(watched == ui->style)        //选择好舱别后，系统自动分配座位
//    {
//        if(event->type() == ui->style->focusOutEvent())
//        {
//            SeatStyle = ui->style->currentText();

//            QSqlQuery query(db);
//            query.exec("select TicketSeat from Air_Ticket where TicketStyle = '"+SeatStyle+"' and PlaneName = '"+hangbanNum+"' and TicketStatus != '"+statusding+"'");
//            while(query.next())
//            {
//                 seatnum = query.value(0).toInt();
//                 ui->seatnum->setText(query.value(0).toString());
//                 break;
//            }
//            return false;
//         }
//        else
//            return false;
//    }
//    return schedule::eventFilter(watched, event);
//}

void schedule::paintEvent(QPaintEvent *)        //窗体重绘
{

    QPainter painter(this);
    painter.fillRect(rect(),QColor(255,255,255,100));
}

void schedule::on_pushButton_clicked()      //确认预订
{
    QDateTime dingtime = QDateTime::currentDateTime();
    QString dtime = dingtime.toString("yyyy-MM-dd hh:mm");

    db.open();
    QSqlQuery query(db);

    query.exec("select PlaneName,TicketStatus from Air_Record where UserName = '" + mess + "'");
    while(query.next())
    {
        if(query.value(0) == hangbanNum && query.value(1).toString().trimmed() == "1")
        {
            QErrorMessage *error = new QErrorMessage(this);
            error->setWindowTitle(tr("提示"));
            error->showMessage(tr("您已预订该航班，不可重复订票！"));
            return;
        }
    }

     //系统分配座位
    SeatStyle = ui->style->currentText();
    query.exec("select TicketSeat from Air_Ticket where TicketStyle = '"+SeatStyle+"' and PlaneName = '"+hangbanNum+"' and TicketStatus != '"+statusding+"'");
    while(query.next())
    {
         seatnum = query.value(0).toInt();
         ui->seatnum->setText(query.value(0).toString());
         break;
    }

    //更新飞机信息表
    if(SeatStyle.trimmed() == "经济舱")      //经济舱座位减一
    {
        query.exec("update Air_Plane set PlaneLowSeatCount = '"+QString::number((ui->jjps->text().toInt()-1))+"' where PlaneName = '"+hangbanNum+"'");
    }
    else if(SeatStyle.trimmed() == "商务舱")         //舱座位减一
    {
        query.exec("update Air_Plane set PlaneHighSeatCount = '"+QString::number((ui->swps->text().toInt()-1))+"' where PlaneName = '"+hangbanNum+"'");
    }
    else
    {
        QErrorMessage *error = new QErrorMessage(this);
        error->setWindowTitle(tr("提示"));
        error->showMessage(tr("预订成功未完成!"));
        return;
    }

    //事务开始
    QSqlDatabase::database().transaction();

    //更新机票状态方法一：更新机票状态
    query.exec("update Air_Ticket set TicketStatus = '"+statusding+"',TicketTime   = '"+dtime+"' where PlaneName = '"+hangbanNum+"' and TicketSeat = '"+QString::number(seatnum)+"'");

    //更新机票状态方法二：
//    query.exec("delete from Air_Ticket where TicketSeat = '"+QString::number(seatnum)+"' and PlaneName = '"+hangbanNum+"'");
//    //插入
//    query.prepare("insert into Air_Ticket(TicketSeat,TicketStyle,TicketStatus,TicketTime,PlaneName) values(?,?,?,?,?)");
//    query.addBindValue(tr("%1").arg(seatnum));
//    query.addBindValue(tr("%1").arg(SeatStyle));
//    query.addBindValue(tr("%1").arg(statusding));
//    query.addBindValue(tr("%1").arg(dtime));
//    query.addBindValue(tr("%1").arg(hangbanNum));
//    query.exec();


    //更新订票记录
    QSqlQuery query1(db);
    query1.prepare("insert into Air_Record(UserName,PlaneName,TicketSeat,TicketStatus) values(?,?,?,?)");
    query1.addBindValue(tr("%1").arg(mess));
    query1.addBindValue(tr("%1").arg(hangbanNum));
    query1.addBindValue(tr("%1").arg(seatnum));
    query1.addBindValue(tr("%1").arg(statusding));
    query1.exec();

    QSqlDatabase::database().commit();//事务结束

    QErrorMessage *error = new QErrorMessage(this);
    error->setWindowTitle(tr("提示"));
    error->showMessage(tr("预订成功！座位号:")+QString::number(seatnum));
    return;
}

void schedule::on_pushButton_2_clicked()
{
    this->close();
}
