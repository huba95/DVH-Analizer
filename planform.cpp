#include "planform.h"

#include "ui_planform.h"

planForm::planForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::planForm)
{
    ui->setupUi(this);
}

planForm::~planForm()
{
    delete ui;
}

void planForm::on_pushButton_clicked()
{
    anal=new DVH(this);
    anal->show();

}

void planForm::on_pushButton_2_clicked()
{
    anal2=new Monaco_main(this);
    anal2->show();

}
