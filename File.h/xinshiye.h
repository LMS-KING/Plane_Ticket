#ifndef XINSHIYE_H
#define XINSHIYE_H

#include <QDialog>

namespace Ui {
class xinshiye;
}

class xinshiye : public QDialog
{
    Q_OBJECT

public:
    explicit xinshiye(QWidget *parent = 0);
    ~xinshiye();
private slots:
    void picturechange();

    void on_pushButton_clicked();

private:
    Ui::xinshiye *ui;
    int page;
};

#endif // XINSHIYE_H
