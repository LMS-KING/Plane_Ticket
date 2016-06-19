#ifndef USERREGIST_H
#define USERREGIST_H

#include <QDialog>

namespace Ui {
class userregist;
}

class userregist : public QDialog
{
    Q_OBJECT

public:
    explicit userregist(QWidget *parent = 0);
    ~userregist();

protected:
    void paintEvent(QPaintEvent *);

signals:
    void sendmessage();

private slots:
    void on_pushButton_2_clicked();

    void timemove();

    bool eventFilter(QObject * watched,QEvent *event);       //事件过滤器
    void on_pushButton_clicked();

private:
    Ui::userregist *ui;
    int i;
};

#endif // USERREGIST_H
