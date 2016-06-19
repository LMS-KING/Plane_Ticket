#include "ticketmessage.h"
#include "ui_ticketmessage.h"
//机票表单头文件
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QErrorMessage>
#include "schedule.h"
#include <QDateTime>

extern QString hangbanNum;      //航班号
extern QString GoPlace;         //出发地
extern QString ToPlace;         //目的地
extern QString GoTimes;         //出发时间
extern QString mess;            //登录者


TicketMessage::TicketMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TicketMessage)
{
    ui->setupUi(this);

//    People = mess;  //获取登录者账号
    ui->label->hide();  //开始隐藏没找到的信息

    QStandardItemModel *ticket_model = new QStandardItemModel();
       ticket_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("航班编号")));
       ticket_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("出发地")));
       ticket_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("目的地")));
       ticket_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("起飞时间")));
       ticket_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("到达时间")));
       ticket_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("经济舱票数")));
       ticket_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("经济舱票价")));
       ticket_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("商务舱票数")));
       ticket_model->setHorizontalHeaderItem(8, new QStandardItem(QObject::tr("商务舱票价")));
       //利用setModel()方法将数据模型与QTableView绑定
       ui->tableView->setModel(ticket_model);
       //整行选择
       ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

          //设置时间显示列的宽度
          ui->tableView->setColumnWidth(3,150);
          ui->tableView->setColumnWidth(4,150);

       ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  //表不可编辑

       //当前时间,已过当前时间的记录不显示
       QDateTime currenttime = QDateTime::currentDateTime();
       QString strtime = currenttime.toString("yyyy-MM-dd hh:mm");

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

       int i = 0;       //动态添加行
       int j = 0;       //判断是否有记录
       QSqlQuery query(db);

      if(GoTimes.isEmpty())        //没选择起飞时间,就默认大于当前时间
          {
                query.exec("select *from Air_Plane where PlaneGo = '" + GoPlace + "' and PlaneTo = '" + ToPlace + "' and PlaneGoTime > '"+strtime+"'");
           }
      else         //选择时间
           {
               query.exec("select *from Air_Plane where PlaneGo = '" + GoPlace + "' and PlaneTo = '" + ToPlace + "' and PlaneGoTime > '" + GoTimes + "'");
           }

       while(query.next())
       {
               ticket_model->setItem(i, 0, new QStandardItem(query.value(1).toString().trimmed()));
               ticket_model->setItem(i, 1, new QStandardItem(query.value(2).toString().trimmed()));
               ticket_model->setItem(i, 2, new QStandardItem(query.value(3).toString().trimmed()));
               ticket_model->setItem(i, 3, new QStandardItem(query.value(6).toString().trimmed()));
               ticket_model->setItem(i, 4, new QStandardItem(query.value(7).toString().trimmed()));
               ticket_model->setItem(i, 5, new QStandardItem(query.value(8).toString().trimmed()));
               ticket_model->setItem(i, 6, new QStandardItem(query.value(10).toString().trimmed()));
               ticket_model->setItem(i, 7, new QStandardItem(query.value(9).toString().trimmed()));
               ticket_model->setItem(i, 8, new QStandardItem(query.value(11).toString().trimmed()));

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

               j++;
               i++;
       }
       if(j == 0)
       {
           ui->label->show();
           ui->pushButton->setEnabled(false);
       }
}

TicketMessage::~TicketMessage()
{
    delete ui;
}

void TicketMessage::on_pushButton_4_clicked()       //退出
{
    this->close();
}

void TicketMessage::on_pushButton_clicked()         //预订
{
    if(mess.isEmpty())
    {
        QErrorMessage *error = new QErrorMessage(this);
        error->setWindowTitle(tr("提示"));
        error->showMessage(tr("您还没有登录，不能订票!"));
        return;
    }

    //获取当前时间
    QDateTime currenttime = QDateTime::currentDateTime();
   // QString times = currenttime.toString("yyyy-MM-dd hh:mm");

    //把航班编号传给预订窗口
    QAbstractItemModel *model = ui->tableView->model();
    int curow = ui->tableView->currentIndex().row();    //当前行
    //data为航班名
    QModelIndex index = model->index(curow,0);
    QVariant data = model->data(index);

    hangbanNum = data.toString().trimmed();
    if(hangbanNum.isEmpty())
    {
        QErrorMessage *error = new QErrorMessage(this);
        error->setWindowTitle(tr("提示"));
        error->showMessage(tr("请选择预订航班!"));
        return;
    }

    QSqlQuery query(db);
    query.exec("select PlaneGoTime,PlaneLowSeatCount,PlaneHighSeatCount from Air_Plane where PlaneName = '" + hangbanNum + "'");
    while(query.next())
    {
        if(query.value(0).toDateTime() < currenttime)
        {
            QErrorMessage *error = new QErrorMessage(this);
            error->setWindowTitle(tr("提示"));
            error->showMessage(tr("对不起，该航班已起飞！不可预定"));
            return;
        }
        if((query.value(1).toInt() + query.value(2).toInt() == 0))
        {
            QErrorMessage *error = new QErrorMessage(this);
            error->setWindowTitle(tr("提示"));
            error->showMessage(tr("对不起，该航班票已经售完"));
            return;
        }
    }
    //判断用户状态
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

    schedule *shd = new schedule(this);
    shd->move(170,100);
    shd->show();
}
