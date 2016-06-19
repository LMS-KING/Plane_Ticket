#include "peoplemessage.h"
#include "ui_peoplemessage.h"
#include <QPainter>
#include <QDebug>
#include <QErrorMessage>


extern QString mess;

Peoplemessage::Peoplemessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Peoplemessage)
{
    ui->setupUi(this);
    j = 0;
    i = 0;

    //给控件安装事件过滤器
    ui->zhanghao->installEventFilter(this);
    ui->telnum->installEventFilter(this);
    ui->email->installEventFilter(this);

    //改变控件可读性
    ui->zhanghao->setReadOnly(true);
    ui->name->setReadOnly(true);
    ui->comboBox->setEnabled(false);
    ui->idnum->setReadOnly(true);
    ui->telnum->setReadOnly(true);
    ui->email->setReadOnly(true);
    ui->address->setReadOnly(true);

    //透明窗体
    this->setWindowFlags(Qt::FramelessWindowHint  | Qt::WindowSystemMenuHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    QString sex = "男,女";
    QStringList sexs = sex.split(",",QString::SkipEmptyParts);
    ui->comboBox->addItems(sexs);


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

    query.exec("select UserPaw, UserTrueName,UserSex,UserIdNum,UserTel,UserEmail,UserAddress from Air_Users where UserName = '" + mess + "'");

    while(query.next())
    {
        passw = query.value(0).toString();
        ui->zhanghao->setText(mess);
        ui->name->setText(query.value(1).toString());
        if(query.value(2).toString() == "男")
            ui->comboBox->setCurrentText(tr("男"));
        else
            ui->comboBox->setCurrentText(tr("女"));
        ui->idnum->setText(query.value(3).toString());
        ui->telnum->setText(query.value(4).toString());
        ui->email->setText(query.value(5).toString());
        ui->address->setText(query.value(6).toString());
    }

}

Peoplemessage::~Peoplemessage()
{
    delete ui;
}

void Peoplemessage::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(),QColor(255,255,255,100));
}

void Peoplemessage::on_pushButton_clicked()     //编辑个人信息
{
    if(ui->pushButton->text() == "编辑")
    {
        ui->pushButton->setText(tr("保存"));
        //改变控件可读性
        ui->zhanghao->setReadOnly(false);
        ui->telnum->setReadOnly(false);
        ui->email->setReadOnly(false);
        ui->address->setReadOnly(false);
    }
    else
    {
        if(j == -1)
        {
            QErrorMessage *error = new QErrorMessage(this);
            error->setModal(true);
            error->setWindowTitle(tr("提示"));
            error->showMessage(tr("含有不合法信息!"));
            return;
        }
        else
        {
            //改变控件可读性
            ui->zhanghao->setReadOnly(true);
            ui->telnum->setReadOnly(true);
            ui->email->setReadOnly(true);
            ui->address->setReadOnly(true);

            ui->pushButton->setText(tr("编辑"));

            QSqlQuery query(db);
            //先删除，后插入
            query.exec("delete from Air_Users where UserName = '" + mess + "'");
            query.prepare("insert into Air_Users(UserName,UserPaw,UserTrueName,UserSex,UserIdNum,UserTel,UserEmail,UserAddress) values(?,?,?,?,?,?,?,?)");

            query.addBindValue(tr("%1").arg(ui->zhanghao->text()));
            query.addBindValue(passw);
            query.addBindValue(tr("%1").arg(ui->name->text()));
            query.addBindValue(tr("%1").arg(ui->comboBox->currentText()));
            query.addBindValue(tr("%1").arg(ui->idnum->text()));
            query.addBindValue(tr("%1").arg(ui->telnum->text()));
            query.addBindValue(tr("%1").arg(ui->email->text()));
            query.addBindValue(tr("%1").arg(ui->address->text()));
            query.exec();

            //成功提示
            QErrorMessage *error = new QErrorMessage(this);
            error->setModal(true);
            error->setWindowTitle(tr("提示"));
            error->showMessage(tr("修改成功!"));
        }
    }
}

bool Peoplemessage::eventFilter(QObject *watched, QEvent *event)       //事件过滤器
{
    if(watched == ui->zhanghao)         //修改用户账号
    {
        if(!ui->zhanghao->isReadOnly())     //可写状态时
        {
            if(event->type() == QEvent::FocusOut)     //控件失去焦点事件
            {
                if(ui->zhanghao->text().isEmpty())
                {
                    j = -1;
                    ui->label_10->setText(tr("不合法"));
                    return false;
                }
                QSqlQuery query(db);
                query.exec("select UserName from Air_Users");
                while(query.next())
                {
                    if(query.value(0).toString() == ui->zhanghao->text())
                    {
                        j = -1;
                        ui->label_9->setText(tr("不合法"));
                        return false;
                    }
                    else
                    {
                        j = 1;
                        ui->label_9->setText(tr("通过"));
                        return true;
                    }
                }
            }
            else
                return false;
        }
        else
            return false;
    }
    else if(watched == ui->telnum)      //修改电话
    {
        if(!ui->telnum->isReadOnly())
        {
            if(event->type() == QEvent::FocusOut)
            {
                if(ui->telnum->text().isEmpty())
                {
                    j = -1;
                    ui->label_10->setText(tr("不合法"));
                    return false;
                }

                std::string str = ui->telnum->text().toStdString();
                const char* ch = str.c_str();
                int k = 0;
                for(int i = 0;i <= ui->telnum->text().length();i++)
                {
                     if(ch[i]-'0' >= 0 && ch[i]-'0' <= 9)
                         k++;
                }
                if(k == 11)
                {
                    j = 1;
                    ui->label_10->setText(tr("通过"));
                    return true;
                }
                else
                {
                    j = -1;
                    ui->label_10->setText(tr("不合法"));
                    return false;
                }
            }
            else
                return false;
        }
        else
            return false;
    }
    else if(watched == ui->email)       //修改邮箱
    {
            if(event->type() == QEvent::FocusOut)
            {
                if(ui->email->text().isEmpty())
                {
                    j = -1;
                    ui->label_11->setText(tr("不合法"));
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
                    j = 1;
                    ui->label_11->setText(tr("通过"));
                    return true;
                }
                else
                {
                    j = -1;
                    ui->label_11->setText(tr("不合法"));
                    return false;
                }
            }
            else
                return false;
        }
        else
            return false;

    return Peoplemessage::eventFilter(watched, event);
}

void Peoplemessage::on_pushButton_2_clicked()       //取消
{
    this->close();
}
