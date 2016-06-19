#ifndef CONTROL_H
#define CONTROL_H

#include <QDialog>
#include <QString>

namespace Ui {
class Control;
}

class Control : public QDialog
{
    Q_OBJECT

public:
    explicit Control(QWidget *parent = 0);
    ~Control();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    bool eventFilter(QObject * watched,QEvent *event);       //事件过滤器

    void on_pushButton_6_clicked();

    void artilemove();      //文字滚动


private:
    Ui::Control *ui;
    int i;
    int position;
    QString hangbanNun;     //记录航班号，便于更新
};

#endif // CONTROL_H
