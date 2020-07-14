#include "getinputs.h"
#include "ui_getinputs.h"

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

GetInputs::GetInputs(QString name, vector<string> lines, string outfile, double maxidose, double volume ,string inputfile , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetInputs),
    dvhfile(lines),
    out(outfile),
    maxdose(maxidose),
    vol(volume),
    in(inputfile)
{
    ui->setupUi(this);
    this->setWindowTitle(name);
    ui->label->setText(windowTitle());
    ui->radioButtonGetrelvolonAbsdose->setChecked(true);
}

GetInputs::~GetInputs()
{
    delete ui;
}


void GetInputs::on_pushButton_clicked()
{

    ofstream myOutFile(out, ios::app);
    ofstream MyInputs(in, ios::app);

    if(ui->radioButtonGetrelvolonAbsdose->isChecked())
    {

        double dose = ui->doubleSpinBox_3->value();
        double result = GetrelVolonAbsDose(dvhfile, dose);
        ui->LabelResult->setText("%");
        if (result >= 0){
             ui->lcdNumber_3->display(result);
            myOutFile<<"V " << dose << " cGy = " << result << " %" << endl;
                        MyInputs<<1<<endl<<dose<<endl;
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

    if(ui->radioButtonGetRelvolonReldose->isChecked())
    {

        double reldose = ui->doubleSpinBox_3->value();
        double dose = maxdose*reldose/100;
        dose = dose * 10;
        dose = round(dose) / 10;
        double result = GetrelVolonAbsDose(dvhfile, dose);
        ui->LabelResult->setText("%");
        if (result >= 0){
             ui->lcdNumber_3->display(result);
            myOutFile<<"V " << dose << " % = " << result << " %" << endl;
                        MyInputs<<3<<endl<<reldose<<endl;
        }
        else{
            myOutFile << "Value (" << dose << ")not found" << endl;
            QString status = QString("Value (%1) not found").arg(dose);
            QMessageBox::information(this, tr("Bad Value"), status);

    }
    }

    if(ui->radioButton_AbsVoltoReldose->isChecked())
    {
        double reldose = ui->doubleSpinBox_3->value();
        double dose = maxdose*reldose/100;
        dose = dose * 10;
        dose = round(dose) / 10;
        double result = GetrelVolonAbsDose(dvhfile, dose);
        result=result*vol/100;
        ui->LabelResult->setText("cm^3");
        if (result >= 0){
             ui->lcdNumber_3->display(result);
            myOutFile<<"V " << reldose << " % = " << result << " cm^3" << endl;
               MyInputs<<4<<endl<<reldose<<endl;
        }
        else{
            myOutFile << "Value (" << dose << ")not found" << endl;
            QString status = QString("Value (%1) not found").arg(reldose);
            QMessageBox::information(this, tr("Bad Value"), status);

    }
    }
	
	    if (ui->radioButton_GetabsdoseonAbsvol->isChecked())
    {
        double absvol = ui->doubleSpinBox_3->value();
        double relvol=absvol/vol*100;
        double result = getAbsDoseonRelVol(dvhfile, relvol);
        ui->LabelResult->setText("cGy");
        if (result >= 0){
                     ui->lcdNumber_3->display(result);
                    myOutFile<<"D " << absvol << " cm^3 = " << result << " cGy" << endl;
                    MyInputs<<5<<endl<<absvol<<endl;
                }
                else{
                    myOutFile << "Value (" << absvol << ")not found" << endl;
                    QString status = QString("Value (%1) not found").arg(absvol);
                    QMessageBox::information(this, tr("Bad Value"), status);


    }}

myOutFile.close();
MyInputs.close();

}
void GetInputs::on_pushButton_2_clicked()
{

    ofstream MyInputs(in, ios::app);
    MyInputs<<"0"<<endl;
    MyInputs.close();
    this->close();
}
