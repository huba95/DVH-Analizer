#include "monaco_getinputs.h"
#include "ui_monaco_getinputs.h"

#include "Func.h"


#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDir>
#include <QString>

#include<algorithm>
#include<iostream>
#include<fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

Monaco_Getinputs::Monaco_Getinputs(QString name, std::vector<std::string> lines, std::string outfile, std::string inputfile, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Monaco_Getinputs),
    dvhfile(lines),
    out(outfile),
    in(inputfile)
{
    ui->setupUi(this);
    this->setWindowTitle(name);
    ui->label->setText(windowTitle());
    ui->radioButtonGetrelvolonAbsdose->setChecked(true);
}

Monaco_Getinputs::~Monaco_Getinputs()
{
    delete ui;
}

void Monaco_Getinputs::on_pushButton_clicked()
{


    std::ofstream myOutFile(out, std::ios::app);

    std::ofstream MyInputs(in, std::ios::app);

    if(ui->radioButtonGetrelvolonAbsdose->isChecked())
    {
        double dose = ui->doubleSpinBox_3->value();
        double result = GetrelVolonAbsDose(dvhfile, dose);
        ui->LabelResult->setText("%");
        if (result >= 0){
             ui->lcdNumber_3->display(result);
            myOutFile<<"V " << dose << " cGy = " << result << " %" << endl;
            MyInputs<<std::endl<<1<<endl<<dose<<endl;
        }
        else{
            myOutFile << "Value (" << dose << ")not found" << endl;
            QString status = QString("Value (%1) not found").arg(dose);
            QMessageBox::information(this, tr("Bad Value"), status);

        }

    }

    if(ui->radioButtongetAbsDoseonRelvol->isChecked())
    {
        double relvol = ui->doubleSpinBox_3->value();
        double result = getAbsDoseonRelVol(dvhfile, relvol);
        ui->LabelResult->setText("cGy");
        if (result >= 0){
             ui->lcdNumber_3->display(result);
            myOutFile<<"D " << relvol << " % = " << result << " cGy" << endl;
            MyInputs<<2<<endl<<relvol<<endl;
        }
        else{
            myOutFile << "Value (" << relvol << ")not found" << endl;
            QString status = QString("Value (%1) not found").arg(relvol);
            QMessageBox::information(this, tr("Bad Value"), status);

        }


    }
    myOutFile.close();
    MyInputs.close();
}

void Monaco_Getinputs::on_pushButton_2_clicked()
{
    ofstream MyInputs(in, std::ios::app);
    MyInputs<<"0"<<endl;
    MyInputs.close();
     this->close();
}
