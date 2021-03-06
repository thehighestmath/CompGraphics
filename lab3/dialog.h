#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    double resultWeight();
    void setWeight(double);

private slots:
//    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
