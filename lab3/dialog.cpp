#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

//void Dialog::on_buttonBox_clicked(QAbstractButton *button)
//{

//}


double Dialog::resultWeight() {
    return this->ui->doubleSpinBox->value();
}

void Dialog::setWeight(double w) {
    this->ui->doubleSpinBox->setValue(w);
}
