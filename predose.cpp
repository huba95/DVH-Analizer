#include "predose.h"
#include "ui_predose.h"

Predose::Predose(QWidget *parent, double* maxdose) :
    QDialog(parent),
    ui(new Ui::Predose),
    predse(maxdose)
{
    ui->setupUi(this);
}

Predose::~Predose()
{
    delete ui;
}

void Predose::on_pushButton_clicked()
{
    *predse = ui->doubleSpinBox->value();
    this->close();
}
