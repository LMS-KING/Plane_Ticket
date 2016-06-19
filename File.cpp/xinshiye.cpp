#include "xinshiye.h"
#include "ui_xinshiye.h"
#include <QTimer>
#include <QPixmap>
#include <QFont>

extern QString idwidow;

xinshiye::xinshiye(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::xinshiye)
{
    ui->setupUi(this);
    page = 0;

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(picturechange()));//关联定时器计满信号和相应的槽函数
    timer->start(2000);//定时器开始计时，其中2000表示2000ms

    ui->label->setPixmap(QPixmap(":/picture/F:/下载/网络下载/飞机2_副本.jpg"));      //快速安全的航班
    ui->label_3->setPixmap(QPixmap(":/picture/F:/下载/网络下载/贴心_副本.jpg"));    //贴心的服务
    ui->label_4->setPixmap(QPixmap(":/picture/F:/下载/网络下载/舒适.jpg"));    //舒适的环境
    ui->label_5->setPixmap(QPixmap(":/picture/F:/下载/网络下载/团队.jpg"));    //我们的团队
    ui->label_2->setPixmap(QPixmap(":/picture/F:/下载/网络下载/信仰_副本.jpg"));    //我们的信仰


    QFont font  = ui->textEdit->font(); //设置字体大小
    font.setPointSize(16);
    ui->textEdit->setFont(font);

     if(idwidow.trimmed() == "xt")
     {
         ui->textEdit->setText(tr("新视野航空网络订票系统可以满足不同旅客的不同需求，可以自行订票，退票及改签！"));
     }
     else if(idwidow.trimmed() == "lkxz")
     {
          ui->textEdit->setText(tr("旅客朋友们请注意，每张机票只可改签一次！在航班起飞前，所有记录不可删除。谢谢合作！祝您旅途愉快！"));
     }

}

xinshiye::~xinshiye()
{
    delete ui;
}

void xinshiye::picturechange()      //图片切换
{
    ui->stackedWidget->setCurrentIndex(page);
    page++;
    if(page == 5)
        page = 0;
}

void xinshiye::on_pushButton_clicked()      //关闭
{
    this->close();
}
