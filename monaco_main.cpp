#include "monaco_main.h"
#include "ui_monaco_main.h"


Monaco_main::Monaco_main(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Monaco_main)
{
    ui->setupUi(this);
}

Monaco_main::~Monaco_main()
{
    delete ui;
}

void Monaco_main::on_pushButton_2_clicked()
{
    if (ui->checkBox->isChecked())
        ifinp = 1;
    mon2=new Monaco_dvh2(this, ifinp);
    mon2->show();

}

void Monaco_main::on_pushButton_3_clicked()
{
    mon3=new Monaco_dvh3(this);
    mon3->show();

}

void Monaco_main::on_pushButton_4_clicked()
{
    exit(1);
}
