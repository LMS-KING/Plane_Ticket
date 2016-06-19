#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"             //登录
#include "userregist.h"         //注册
#include "peoplemessage.h"      //个人信息
#include "ticketmessage.h"      //机票信息
#include "schedule.h"           //机票预订
#include "record.h"             //订票记录
#include "password.h"           //修改密码
#include "xinshiye.h"           //关于新视野，关于系统，旅客须知三合一
#include <QPixmap>
#include <QMovie>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QDate>
#include <QErrorMessage>


Dialog *dialog;         //登录窗口
userregist *userrg;     //注册窗口

QString mess;           //全局信息传递（判断登录前后）
QString hangbanNum;     //给机票信息和订票窗口传送航班号
QString idwidow;

QString GoPlace;        //给机票信息窗口传送地名
QString ToPlace;
QString GoTimes;

int checkfor;      //判断预订前是否已经查询


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->calendarWidget->hide();     //日历开始隐藏
    ui->kedingpiao->hide();
    ui->kedingpiao_2->hide();
    ui->label_4->hide();
    ui->label_5->hide();

    i = 0;      //滚动字幕的横坐标
    j = 1;      //记录时间按钮按下的次数
    checkfor = 0;

    dialog = new Dialog(this);      //  要和主窗口关联，必须先new对象
    dialog->hide();
    userrg = new userregist(this);
    userrg->hide();

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timemove()));//关联定时器计满信号和相应的槽函数

    connect(dialog,SIGNAL(senddate(QString)),this,SLOT(recdate(QString)));  //登录窗口关联
    connect(userrg,SIGNAL(sendmessage()),this,SLOT(recmessage()));  //注册窗口关联

    timer->start(100);//定时器开始计时，其中100表示100ms

    QMovie *movie = new QMovie(":/picture/F:/下载/qq图像/airplane");    //动态图片
    ui->label_6->setMovie(movie);
    movie->start();

//    ui->statusBar->showMessage(tr("欢迎登录新视野航空"),10000);        //临时标签10s
//    QLabel * label = new QLabel(this);                          //永久标签
//    label->setFrameStyle(QFrame::Box|QFrame::Sunken);
//    label->setText("何事秋风悲画扇@FLY");
//    ui->statusBar->addPermanentWidget(label);

    ui->comboBox->setEditable(true);     //下拉框可编辑
    ui->comboBox_2->setEditable(true);
    ui->kedingpiao->setReadOnly(true);  //余票数只可读
    ui->lineEditTime->setReadOnly(true);    //时间显示框只读
    ui->lcdNumber->setPalette(Qt::blue);     //时间显示框颜色

    QString place = "北京,上海,长沙,深圳,香港,宿州";      //添加默认热门城市
    QString place1 =  "上海,北京,宿州,深圳,香港,长沙";
    QStringList places = place.split(",",QString::SkipEmptyParts);
    QStringList places1 = place1.split(",",QString::SkipEmptyParts);
    ui->comboBox->addItems(places);
    ui->comboBox_2->addItems(places1);


    //连接数据库
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
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timemove()     //时间更新
{
    i += 3;
    if(i % 20 == 0)
    {
        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
        QString strTime = time.toString("yyyy-MM-dd hh:mm");//设置系统时间显示格式
        ui->lcdNumber->display(strTime);//在lcdNumber上显示时间
    }

    if(i != 570)
        ui->label_7->move(570-i,30);
    else
    {
        ui->label_7->move(i,30);
        i = 0;
    }

    //判断查询方式，以便改变航班号输入框的可读性
    if(ui->radioButton_2->isChecked())          //按航班查询
    {
        ui->comboBox->setEnabled(false);        //查询出的航班信息只读
        ui->comboBox_2->setEnabled(false);
        ui->btntime->setEnabled(false);     //时间只读
        ui->kedingpiao_2->setReadOnly(false);
        //不跳转，航班和余票都显示
        ui->label_4->show();
        ui->label_5->show();
        ui->kedingpiao->show();
        ui->kedingpiao_2->show();
        ui->btnchaxun_2->setEnabled(true);     //预订功能可用

    }
    else            //按起始地点查询
    {
        ui->comboBox->setEnabled(true);        //查询出的航班信息只读
        ui->comboBox_2->setEnabled(true);
        ui->btntime->setEnabled(true);     //时间只读
        ui->kedingpiao_2->setReadOnly(true);
        //要跳转到Ticketmessage界面，所以隐藏航班和余票
        ui->label_4->hide();
        ui->label_5->hide();
        ui->kedingpiao->hide();
        ui->kedingpiao_2->hide();
        ui->btnchaxun_2->setEnabled(false);     //预订功能不可用
    }
}

void MainWindow::recdate(QString date)      //接收旅客登录窗口的用户名
{
    mess = date;        //获取用户名，用于查询数据库

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

    QLabel * label = new QLabel(this);                          //永久标签
    label->setFrameStyle(QFrame::Box|QFrame::Sunken);
    label->setText(date + tr("欢迎登录"));
    ui->statusBar->addPermanentWidget(label);
    ui->label_7->setText(tr("可查询航班:") + name + tr(",祝您旅途愉快!"));

    //delete (ui->menuBar);
//    QMenuBar *menb = new QMenuBar(this);
//    menb->resize(680,23);
//    menb->addMenu(tr("个人中心"));
//    menb->addMenu(tr("乘机须知"));
//    menb->addMenu(tr("退出登录"));
//    menb->show();

    ui->menuBar->clear();       //清除没登陆前的菜单栏的所有事件

    QMenu *peopel = new QMenu(ui->menuBar);         //添加新菜单
    peopel = ui->menuBar->addMenu(tr("个人中心"));
    QAction *message = peopel->addAction(tr("基本资料"));       //添加新事件
    QAction *tickets = peopel->addAction(tr("已完成订单(退/改)"));     //添加新事件
    QAction *mima = peopel->addAction(tr("修改密码"));     //添加新事件

    QMenu *peopleknows = new QMenu(ui->menuBar);        //添加新菜单
    peopleknows = ui->menuBar->addMenu(tr("旅客须知"));
    QAction *knows = peopleknows->addAction(tr("乘机须知"));    //添加新事件

    QMenu *out = new QMenu(ui->menuBar);        //添加新菜单
    out = ui->menuBar->addMenu(tr("退出登录"));
    QAction *safeout = out->addAction(tr("安全退出"));      //添加新事件

    //给新添加的事件关联处理函数
    connect(message,SIGNAL(triggered(bool)),this,SLOT(peoplemessage()));    //个人信息

    connect(tickets,SIGNAL(triggered(bool)),this,SLOT(ticketsmessage()));   //订票记录

    connect(mima,SIGNAL(triggered(bool)),this,SLOT(checkpaw()));    //修改密码

    connect(knows,SIGNAL(triggered(bool)),this,SLOT(peopleknow()));     //旅客须知

    connect(safeout,SIGNAL(triggered(bool)),this,SLOT(saout()));        //退出
}

void MainWindow::recmessage()       //注册成功后直接显示登录窗口
{
    MainWindow::on_action_triggered();
}

void MainWindow::on_action_triggered()      //用户登录
{
    dialog->move(250,105);
    dialog->show();
    return;
}

void MainWindow::on_action_8_triggered()        //退出
{
    this->close();
}

void MainWindow::on_action_6_triggered()        //用户注册
{
    userrg->move(250,85);
    userrg->show();
}

void MainWindow::on_btntime_clicked()       //出发时间选择
{
     j++;
     if(j % 2 == 0)
     {
         //选择时间时，下面控件隐藏
         ui->label_5->hide();
         ui->label_4->hide();
         ui->kedingpiao->hide();
         ui->kedingpiao_2->hide();
         ui->btnchaxun->hide();
         ui->btnchaxun_2->hide();
         //j本身为1，当点击奇数次，日历显示
         ui->calendarWidget->show();
         ui->lineEditTime->setText(ui->calendarWidget->selectedDate().toString("yyyy-MM-dd dddd"));
     }
    else
     {
         ui->label_5->show();
         ui->label_4->show();
         ui->kedingpiao->show();
         ui->kedingpiao_2->show();
         ui->btnchaxun->show();
         ui->btnchaxun_2->show();
         //点击偶数次，日历隐藏
         ui->calendarWidget->hide();
         ui->lineEditTime->setText(ui->calendarWidget->selectedDate().toString("yyyy-MM-dd"));
     }
}

void MainWindow::on_btnchaxun_clicked()     //查询
{
    checkfor++;

    if(!ui->radioButton->isChecked() && !ui->radioButton_2->isChecked())
    {
        QErrorMessage *error = new QErrorMessage(this);
        error->setWindowTitle(tr("提示"));
        error->showMessage(tr("请选择查询条件！"));
        return;
    }

    if(ui->radioButton->isChecked())        //按出发地查询
    {
        GoPlace = ui->comboBox->currentText();
        ToPlace = ui->comboBox_2->currentText();
        GoTimes = ui->lineEditTime->text();
        TicketMessage *ticmess = new TicketMessage(this);
        ticmess->setModal(true);
        ticmess->setWindowTitle(tr("航班详细信息表"));
        ticmess->show();
        return;
    }
    else if(ui->radioButton_2->isChecked())     //按航班查询
    {
        hangbanNum = ui->kedingpiao_2->text();  //获取航班号

        db.open();
        QSqlQuery query(db);

        query.exec("select PlaneGo,PlaneTo,PlaneGoTime from Air_Plane where PlaneName = '" + ui->kedingpiao_2->text() + "'");
        while(query.next())
        {
            ui->comboBox->setCurrentText(query.value(0).toString());
            ui->comboBox_2->setCurrentText(query.value(1).toString());
            ui->lineEditTime->setText(query.value(2).toDateTime().toString("yyyy-MM-dd hh:mm"));
        }
        ui->comboBox->setEnabled(false);        //查询出的航班信息只读
        ui->comboBox_2->setEnabled(false);
        ui->btntime->setEnabled(false);     //时间只读

        //查询航班座位信息
        char weiding = '1';     //机票已订状态
        query.exec("select COUNT(*) from Air_Ticket where PlaneName = '" +hangbanNum+ "' and TicketStatus != '" + weiding + "'");     //航班号，机票状态
        while(query.next())
        {
           ui->kedingpiao->setText(query.value(0).toString());
        }
        db.close();
    }
}

void MainWindow::on_btnchaxun_2_clicked()       //预定
{
    QDateTime currenttime = QDateTime::currentDateTime();

    bool loginOr = false;       //判断是否登录

    if(mess.isEmpty())
    {
        loginOr = true;
        MainWindow::on_action_triggered();
        return;
    }


    if(!ui->radioButton_2->isChecked() || ui->kedingpiao_2->text().isEmpty())
    {
            QErrorMessage *error = new QErrorMessage(this);
            error->setWindowTitle(tr("提示"));
            error->showMessage(tr("请选择合适的航班!"));
            return;
    }
//判断剩余票数
    if(ui->kedingpiao->text().toInt() == 0 && !ui->kedingpiao_2->text().isEmpty() && checkfor != 0)
    {
        QErrorMessage *error = new QErrorMessage(this);
        error->setWindowTitle(tr("提示"));
        error->showMessage(tr("对不起，该航班票已经售完"));
        return;
    }

    //判断飞机起飞时间
    db.open();
    QSqlQuery query(db);
    query.exec("select PlaneGoTime from Air_Plane where PlaneName = '" + hangbanNum + "'");
    while(query.next())
    {
        if(query.value(0).toDateTime() < currenttime)
        {
            QErrorMessage *error = new QErrorMessage(this);
            error->setWindowTitle(tr("提示"));
            error->showMessage(tr("对不起，该航班已起飞！不可预定"));
            return;
        }
    }
    //判断用户状态
    query.exec("select PlaneName,TicketStatus from Air_Record where UserName = '" + mess + "'");
    while(query.next())
    {
        if(query.value(0) == hangbanNum && query.value(1).toString() == "1")
        {
            QErrorMessage *error = new QErrorMessage(this);
            error->setWindowTitle(tr("提示"));
            error->showMessage(tr("您已预订该航班，不可重复订票！"));
            return;
        }
    }

    if(ui->radioButton_2->isChecked() && !(ui->kedingpiao_2->text().isEmpty()) || loginOr)     //首页预定只支持按航班查询
    {
        hangbanNum = ui->kedingpiao_2->text();
        schedule *scheduleTicket = new schedule(this);
        scheduleTicket->move(250,105);
        scheduleTicket->show();
        return;
    }
    db.close();
}

void MainWindow::peoplemessage()        //个人信息查询
{
    Peoplemessage *ppm = new Peoplemessage(this);
    ppm->setWindowTitle(tr("个人中心"));
    ppm->move(250,105);
    ppm->show();
}

void MainWindow::ticketsmessage()       //旅客订票记录
{
    Record *rec = new Record(this);
    rec->setModal(true);
    rec->setWindowTitle(tr("订、退、改签记录信息表"));
    rec->show();
}

void MainWindow::checkpaw()             //修改密码
{
    password *paw = new password(this);
    paw->move(250,105);
    paw->show();
}

void MainWindow::peopleknow()       //旅客须知
{
    idwidow = "lkxz";
    xinshiye *xsy = new xinshiye(this);
    xsy->setWindowTitle(tr("乘机须知"));
    xsy->setModal(true);
    xsy->show();
}

void MainWindow::on_action_3_triggered()        //关于系统
{
    idwidow = "xt";
    xinshiye *xsy = new xinshiye(this);
    xsy->setWindowTitle(tr("关于系统"));
    xsy->setModal(true);
    xsy->show();
}

void MainWindow::on_action_7_triggered()        //关于新视野
{
    idwidow = "xsy";
    xinshiye *xsy = new xinshiye(this);
    xsy->setWindowTitle(tr("关于新视野"));
    xsy->setModal(true);
    xsy->show();
}

void MainWindow::saout()        //退出
{
    this->close();
}
