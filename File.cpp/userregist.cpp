#include "userregist.h"
#include "ui_userregist.h"
#include <QPainter>
#include <QErrorMessage>
#include <QPalette>
#include <Qtimer>
#include "dialog.h"
//数据库头文件
#include <QSqlDatabase>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QSqlQuery>
#include <Qtsql>


userregist::userregist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userregist)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);       //实时判断密码是否可见
    connect(timer,SIGNAL(timeout()),this,SLOT(timemove()));//关联定时器计满信号和相应的槽函数
    timer->start(10);//定时器开始计时，其中10表示10ms//新建定时器

    //安装事件过滤器
    ui->zhanghao->installEventFilter(this);
    ui->mima->installEventFilter(this);
    ui->name->installEventFilter(this);
    ui->Idnum->installEventFilter(this);
    ui->radioButton->installEventFilter(this);
    ui->radioButton_2->installEventFilter(this);
    ui->telnum->installEventFilter(this);
    ui->email->installEventFilter(this);
    ui->address->installEventFilter(this);

    //透明窗体
    this->setWindowFlags(Qt::FramelessWindowHint  | Qt::WindowSystemMenuHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

userregist::~userregist()
{
    delete ui;
}

void userregist::paintEvent(QPaintEvent *)      //注册窗体重绘
{
    QPainter painter(this);
    painter.fillRect(rect(),QColor(255,255,255,100));
}

bool userregist::eventFilter(QObject *watched, QEvent *event)       //处理各个编辑框的输入
{
    if(watched == ui->zhanghao)     //账号输入框
    {
        if(event->type() == QEvent::FocusOut)     //控件失去焦点事件
        {
            if(ui->zhanghao->text().isEmpty())
            {
                i = -1;
                ui->label_10->setText(tr("账号不能为空"));
                return false;
            }
             //数据库连接
            QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
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

            QSqlQuery query(db);
            query.exec("select *from Air_Users");
            while(query.next())
            {
               if(query.value(1).toString() == ui->zhanghao->text()) //用户名已存在
               {
                    ui->label_10->setText(tr("账号已存在!"));
                    i = -1;
               }
               else
               {
                    ui->label_10->setText("通过");
                    i = 1;
               }
            }
        }
    }
    else if(watched == ui->mima)         //密码输入框
    {

              if(event->type()==QEvent::FocusOut)    //控件失去焦点事件
              {
                  if(ui->mima->text().isEmpty())
                  {
                      i = -1;
                      ui->label_11->setText(tr("密码不能为空"));
                      return false;
                  }
                   else if(ui->mima->text().length() > 20 )
                   {
                       i = -1;
                       ui->label_11->setText(tr("密码不合法"));
                   }
                   else if(ui->mima->text().length() <= 6)
                   {
                       i = 1;
                       ui->label_11->setText(tr("安全性低"));
                   }
                   else
                   {
                       i = 1;
                       ui->label_11->setText(tr("通过"));
                   }
              }
      }
    else if (watched == ui->name)   //姓名输入框
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->name->text().isEmpty())
            {
                i = -1;
                ui->label_12->setText(tr("姓名不能为空"));
                return false;
            }
            if(ui->name->text().length() > 32)
            {
                i = -1;
                ui->label_12->setText(tr("姓名不合法"));
            }
            else
            {
                 i = 1;
                 ui->label_12->setText(tr("通过"));
            }

//            QRegExp Name("^[\u4E00-\u9FA5]+$");
//            int k = 0;
//            i = Name.indexIn(ui->name->text(),i);
//            if(k != -1)
//                ui->label_12->setText(tr("通过"));
//            else
//                ui->label_12->setText(tr("不通过"));
        }



    }
    else if (watched == ui->Idnum)   //身份证号输入框
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->Idnum->text().isEmpty())
            {
                i = -1;
                ui->label_13->setText(tr("身份证号不能为空"));
                return false;
            }


           std::string str = ui->Idnum->text().toStdString();
           const char* ch = str.c_str();
           int j = 0;
           for(int i = 0;i <= ui->Idnum->text().length();i++)
           {
                if(ch[i]-'0' >= 0 && ch[i]-'0' <= 9)
                    j++;
           }
            if((j == 17 && ui->Idnum->text().length() == 18) || j == 18)
            {
                i = 1;
                ui->label_13->setText(tr("通过"));
            }
            else
            {
                i = -1;
                ui->label_13->setText(tr("证件号不合法"));
            }
        }
    }
    else if (watched == ui->telnum)   //电话号输入框
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->telnum->text().isEmpty())
            {
                i = -1;
                ui->label_14->setText(tr("电话不能为空"));
                return false;
            }

            std::string str = ui->telnum->text().toStdString();
            const char* ch = str.c_str();
            int j = 0;
            for(int i = 0;i <= ui->telnum->text().length();i++)
            {
                 if(ch[i]-'0' >= 0 && ch[i]-'0' <= 9)
                     j++;
            }
            if(j == 11)
            {
                i = 1;
                ui->label_14->setText(tr("通过"));
            }
            else
            {
                i = -1;
                ui->label_14->setText(tr("号码不合法"));
            }
        }
    }
    else if (watched == ui->email)   //E-mail输入框
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->email->text().isEmpty())
            {
                i = -1;
                ui->label_15->setText(tr("邮箱不能为空"));
                return false;
            }

            //判断邮箱的正则表达式
            QRegExp Email("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");

//            bool tf = Email.isValid();        //判断正则表达式是否合法
//            qDebug()<<tf;

            int e = 0;
            e = Email.indexIn(ui->email->text(),e);
            if(e != -1)
            {
                i = 1;
                ui->label_15->setText(tr("通过"));
            }
            else
            {
                i = -1;
                ui->label_15->setText(tr("邮箱不合法"));
            }
        }
    }
    return QWidget::eventFilter(watched,event);     // 最后将事件交给上层对话框
}

void userregist::on_pushButton_2_clicked()      //取消
{
  //由于注册窗口需要向主窗口传数据，在主窗口定义为全局变量，只绘制一次，所以每次注册后信息要清空
          ui->zhanghao->clear();
          ui->mima->clear();
          ui->name->clear();
          ui->Idnum->clear();
          ui->radioButton->setChecked(false);
          ui->radioButton_2->setChecked(false);
          ui->checkBox->setChecked(false);
          ui->telnum->clear();
          ui->email->clear();
          ui->address->clear();
          ui->label_10->setText("*");
          ui->label_11->setText(tr("(6-20字符)"));
          ui->label_12->setText("*");
          ui->label_13->setText("*");
          ui->label_14->setText("*");
          ui->label_15->setText("*");

    this->close();
}

void userregist::timemove()     //实时判断密码是否可见
{
    if(ui->checkBox->isChecked())
    {
        ui->mima->setEchoMode(QLineEdit::Normal);     //密码输入框为明码
    }
    else
        ui->mima->setEchoMode(QLineEdit::Password);     //密码输入框为明码
}

void userregist::on_pushButton_clicked()        //注册
{
    if(i == 1)      //信息合法
    {
        //数据库连接
       QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
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
       query.prepare("insert into Air_Users(UserName,UserPaw,UserTrueName,UserSex,UserIdNum,UserTel,UserEmail,UserAddress) values(?,?,?,?,?,?,?,?)");

       query.addBindValue(tr("%1").arg(ui->zhanghao->text()));
       query.addBindValue(tr("%1").arg(ui->mima->text()));
       query.addBindValue(tr("%1").arg(ui->name->text()));
       if(ui->radioButton->isChecked())
       {
           query.addBindValue(tr("男"));
       }
       else
           query.addBindValue(tr("女"));
       query.addBindValue(tr("%1").arg(ui->Idnum->text()));
       query.addBindValue(tr("%1").arg(ui->telnum->text()));
       query.addBindValue(tr("%1").arg(ui->email->text()));
       query.addBindValue(tr("%1").arg(ui->address->text()));
       query.exec();

       QErrorMessage *error = new QErrorMessage(this);
       error->setModal(true);
       error->setWindowTitle(tr("提示"));
       error->showMessage(tr("注册成功!"));

 //由于注册窗口需要向主窗口传数据，在主窗口定义为全局变量，只绘制一次，所以每次注册后信息要清空
       ui->zhanghao->clear();
       ui->mima->clear();
       ui->name->clear();
       ui->Idnum->clear();
       ui->radioButton->setChecked(false);
       ui->radioButton_2->setChecked(false);
       ui->checkBox->setChecked(false);
       ui->telnum->clear();
       ui->email->clear();
       ui->address->clear();
       ui->label_10->setText("*");
       ui->label_11->setText("*");
       ui->label_12->setText("*");
       ui->label_13->setText("*");
       ui->label_14->setText("*");
       ui->label_15->setText("*");

       emit sendmessage();     //触发信号，显示登录窗口
       this->close();
    }
    else
    {
        QErrorMessage *error = new QErrorMessage(this);
        error->setWindowTitle(tr("提示"));
        error->showMessage(tr("含有不合法信息!"));
    }
}
