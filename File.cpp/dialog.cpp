#include "dialog.h"
#include "ui_dialog.h"
#include "control.h"
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <QErrorMessage>
#include <QMessageBox>

//数据库头文件
#include <QSqlDatabase>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QSqlQuery>
#include <Qtsql>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);       //密码输入框为暗码
    i = 0;
    user = 0;
    crl = 0;

  //  this->setWindowOpacity(0.8);      //窗体透明
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timemove()));//关联定时器计满信号和相应的槽函数
    timer->start(100);//定时器开始计时，其中100表示100ms即1秒//新建定时器
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::timemove()     //定时器槽函数触发首页字幕滚动
{
    i += 2;
    if(i != 584)
        ui->label->move(422-i,25);
    else
    {
        ui->label->move(i,25);
        i = 0;
    }
}

void Dialog::paintEvent(QPaintEvent *)      //登录窗体重绘
{
    QPainter painter(this);
    painter.fillRect(rect(),QColor(255,255,255,100));
    this->setModal(true);
}

void Dialog::on_pushButton_clicked()        //登录
{
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    if(username.isEmpty() || password.isEmpty())
    {
        QErrorMessage *error = new QErrorMessage(this);
        error->setWindowTitle(tr("提示"));
        error->showMessage(tr("请输入完整信息"));
    }
    else if(!ui->radioButton->isChecked() && !ui->radioButton_2->isChecked()) //没选择身份
    {
        QErrorMessage *message = new QErrorMessage(this);
        message->setWindowTitle(tr("提示"));
        message->showMessage(tr("请选择身份"));
        return;
    }

    QSqlDatabase db;       //数据库连接

    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QODBC");

     db.setDatabaseName("DRIVER={SQL SERVER};SERVER=LMS-KING;DATABASE=Air_Plane;uid=sa;pwd=521ai3849;");
     if(!db.open())
     {
          qDebug()<<db.lastError().text();
          QMessageBox::critical(0,QObject::tr("Database Error"),db.lastError().text());
          return;
     }

     QSqlQuery query(db);
     if(ui->radioButton->isChecked())       //旅客登录
     {

        query.exec("select UserPaw from Air_Users where UserName = '" + username +"'");
        while(query.next())
        {
            if(query.value(0).toString() == password)
            {
                user++;
                QMessageBox::information(this,tr("欢迎%1").arg(username),
                                         tr("登录成功!"),QMessageBox::Ok);
                //由于登录窗口需要向主窗口传数据，在主窗口定义为全局变量，只绘制一次，所以每次注册后信息要清空
                emit senddate(ui->lineEdit->text());    //触发事件
                ui->radioButton->setChecked(false);
                ui->radioButton_2->setChecked(false);
                ui->lineEdit->clear();
                ui->lineEdit_2->clear();
                this->close();
            }    
        }

        if(user == 0)
        {
            QErrorMessage *message = new QErrorMessage(this);
            message->setWindowTitle(tr("提示"));
            message->showMessage(tr("用户名或密码错误!"));
        }
     }
     else if(ui->radioButton_2->isChecked())        //管理员登录
     {
         query.exec("select ControlName,ControlPaw from Air_Control");
         while(query.next())
         {
             if(query.value(0).toString() == username && query.value(1).toString() == password)
             {                
                 crl++;
                 QMessageBox::information(this,tr("欢迎%1").arg(username),
                                          tr("登录成功!"),QMessageBox::Ok);
                 //由于登录窗口需要向主窗口传数据，在主窗口定义为全局变量，只绘制一次，所以每次注册后信息要清空
                 ui->lineEdit->clear();
                 ui->lineEdit_2->clear();
                 ui->radioButton->setChecked(false);
                 ui->radioButton_2->setChecked(false);
                 this->close();    
                 db.close();        //关闭数据库连接
                 Control *control = new Control(this);
                 control->setWindowTitle(tr("航班管理"));
                 control->setModal(true);
                 control->show();
                 break;
             }
         }
         if(crl == 0)
         {
            QErrorMessage *message = new QErrorMessage(this);
            message->setWindowTitle(tr("提示"));
            message->showMessage(tr("用户名或密码错误!"));
         }
     }

     db.close();        //关闭数据库连接
}


void Dialog::on_pushButton_2_clicked()
{
  //由于登录窗口需要向主窗口传数据，在主窗口定义为全局变量，只绘制一次，所以每次登录信息要清空
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->radioButton->setChecked(false);
    ui->radioButton_2->setChecked(false);
    this->close();
}



