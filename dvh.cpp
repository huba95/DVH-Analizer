#include "dvh.h"
#include "ui_dvh.h"
#include "dvh1.h"
#include "dvh2.h"
#include "dvh3.h"


DVH::DVH(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DVH)
{
    ui->setupUi(this);
}

DVH::~DVH()
{
    delete ui;
}


void DVH::on_pushButton_2_clicked()
{
    int inp=0;
    if(ui->checkBox->isChecked())
    {
        inp=1;
    }
    anal2=new DVH2(this, inp);
    anal2->show();

}

void DVH::on_pushButton_3_clicked()
{
    anal3=new DVH3(this);
    anal3->show();

}

void DVH::on_pushButton_4_clicked()
{
    exit(1);
}



