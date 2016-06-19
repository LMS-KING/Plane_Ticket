#include "password.h"
#include "ui_password.h"
#include <QPainter>
#include <QPixmap>
#include <QErrorMessage>
#include <QDebug>
#include <QTimer>

extern QString mess;

password::password(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::password)
{
    ui->setupUi(this);
    ui->lineEdit->setEchoMode(QLineEdit::Password);       //密码输入框为暗码
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);       //密码输入框为暗码
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(showpassw())); //
    timer->start(100);

    i = 0;
    ui->label_5->setPixmap(QPixmap(":/picture/F:/下载/网络下载/密码.png"));

    //透明窗体
    this->setWindowFlags(Qt::FramelessWindowHint  | Qt::WindowSystemMenuHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    //安装事件过滤器
    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);

}

password::~password()
{
    delete ui;
}

void password::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(),QColor(255,255,255,100));
}

bool password::eventFilter(QObject *watched, QEvent *event)     //事件过滤器
{
    if(watched == ui->lineEdit)
    {
        if(event->type() == QEvent::FocusOut)
        {
            //数据库连接
            db = QSqlDatabase::addDatabase("QODBC");
            dsn = QString::fromLocal8Bit("DRIVER={SQL SERVER};SERVER=LMS-KING;DATABASE=Air_Plane");
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
            query.exec("select UserPaw from Air_Users where UserName = '"+mess+"'");
            qDebug()<<mess;
            while(query.next())
            {
                qDebug()<<query.value(0).toString();

                if(query.value(0).toString() != ui->lineEdit->text())
                {
                    i = -1;
                    ui->label_3->setText(tr("密码错误"));
                    return false;

                }
                else
                {
                    i = 1;
                    ui->label_3->setText(tr("密码正确"));
                    return false;
                }
            }
         }
        else
            return false;
      }
        else if(watched == ui->lineEdit_2)
        {
            if(event->type() == QEvent::FocusOut)
            {
                if(ui->lineEdit_2->text().isEmpty())
                {
                    i = -1;
                    ui->label_4->setText(tr("密码不能为空"));
                    return false;
                }
                else if(ui->lineEdit_2->text().length() < 6)
                {
                    i = 1;
                    ui->label_4->setText(tr("密码安全性低"));
                    return false;
                }
                else if(ui->lineEdit_2->text().length() > 20)
                {
                    i = -1;
                    ui->label_4->setText(tr("密码不合法"));
                    return false;
                }
                else
                {
                    i = 1;
                    ui->label_4->setText(tr("密码通过"));
                    return false;
                }
            }
            else
                return false;
        }

    return password::eventFilter(watched, event);
}

void password::on_pushButton_clicked()      //修改
{
    if(i == 1)
    {
        db.open();
        QSqlQuery query(db);
        query.exec("update Air_Users set UserPaw = '"+ui->lineEdit_2->text()+"' where UserName = '"+mess+"'");

            QErrorMessage *error = new QErrorMessage(this);
            error->setWindowTitle(tr("提示"));
            error->showMessage(tr("密码修改成功!"));
            this->close();
            return;
    }
}

void password::on_pushButton_2_clicked()        //退出
{
    this->close();
}

void password::showpassw()      //实时判断是否显示密码
{
    if(ui->checkBox->isChecked())
    {
        ui->lineEdit->setEchoMode(QLineEdit::Normal);       //密码输入框为明码
        ui->lineEdit_2->setEchoMode(QLineEdit::Normal);       //密码输入框为明码
    }
    else
    {
        ui->lineEdit->setEchoMode(QLineEdit::Password);       //密码输入框为暗码
        ui->lineEdit_2->setEchoMode(QLineEdit::Password);       //密码输入框为暗码
    }
}
