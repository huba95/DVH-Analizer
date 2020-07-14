#include "dvh2.h"
#include "ui_dvh2.h"
#include "Func.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QString>

#include<algorithm>
#include<iostream>
#include<fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>



DVH2::DVH2(QWidget *parent, int ifinput) :
    QDialog(parent),
    ui(new Ui::DVH2),
  ifinput(ifinput)
{
    ui->setupUi(this);
    ui->checkBox_HI->setCheckable(false);
    ui->checkBox_CN->setCheckable(false);
}

DVH2::~DVH2()
{
    delete ui;
}



void DVH2::on_pushButton_clicked()
{
    QString dvhfile;
       dvhfile = QFileDialog::getOpenFileName(this, tr("Open DVH file"),"C:\\" , "All files (*.*);;Text files (*.*)");
       QMessageBox::information(this, tr("FILE"), dvhfile);

       QDir().mkdir("DVHData");

       unsigned  n = 1;
       int status=5;
       string line;														//ebbe olvassuk a soroka
       string inputdvh;



       dvhfile.replace("/","\\");
       line=dvhfile.toStdString();
       string line2 = dvhfile.toStdString();
       line2.erase( line2.find_last_of("\\")+1, line2.size());
       line2+="Outfiles";
       QDir().mkdir(QString::fromStdString(line2));
       line.erase(0, line.find_last_of("\\")+1);


       outfile = line2+"\\out_" + line + ".txt";

       line.clear();

       inputdvh=dvhfile.toStdString();

       ofstream myOutFile(outfile);
       ifstream myReadFile(inputdvh);
       if (myReadFile.is_open()) {
           getline(myReadFile, line);										//kiírjuk az elsö két sort, ami a beteg nevét és azonosítóját tartalmazza
           myOutFile << line << endl;
           getline(myReadFile, line);
           myOutFile << line << endl << endl << "Structures:" << endl;
           status=10;
           ui->progressBar->setValue(status);
           status+=5;

           while (!myReadFile.eof()) {

               while (structsearch(line) == false){						//amíg nem talál struktúrát olvas
                   getline(myReadFile, line);

                   if (line.compare(0, 10, "Prescribed") == 0) {
                           line.erase(0, 23);
                           if (isdigit(line.c_str()[0])) {
                               maxdose = stold(line);
                           }
                           else {
                                maxdose=0;
                           }

                           }
               }

               if (structsearch(line) == true)								//ha struktúrát talál elindul
               {
                   ui->progressBar->setValue(status);
                   status+=5;
                   line.erase(0, 11);
                   structures.push_back(line);
                   if(line=="BODY"){body=1;}
                   ui->listWidget->addItem(QString::fromStdString(line));
                   myOutFile << line << endl;								//beírjuk a kimeneti fájlba is
                   location = "DVHdata\\";									//elkészítjük a részfájl elérési útját
                   location += line;
                   location += ".txt";
                   ofstream stuff(location);								//létrehozzuk/megnyitjuk a részfájlt
                   n++;
                   while (line.compare("") != 0){
                       getline(myReadFile, line);
                       if (line.compare(0, 6, "Volume") == 0){ stuff << getvolume(line) << endl; }
                       if (line.compare(0, 8, "Min Dose") == 0){ stuff << getminmaxdose(line) << endl; }
                       if (line.compare(0, 8, "Max Dose") == 0){ stuff << getminmaxdose(line) << endl; }
                       if (line.compare(0, 9, "Mean Dose") == 0){ stuff << getmeandose(line) << endl; }
                   }
                   getline(myReadFile, line);
                   while (getline(myReadFile, line)){								//kiírjuk a többi sort
                       if (structsearch(line) == false){
                           stuff << line << endl;
                       }
                       else{													//ha van új struktúra kilépünk
                           stuff.close();
                           break;
                       }
                   }
               }

           }
       }
       else{QMessageBox::information(this, tr("FILE"), "Error");this->close();}
       myReadFile.close();
       myOutFile.close();
       ui->progressBar->setValue(100);
       if(maxdose==0){
           preddose=new Predose(this, predse);
           preddose->show();

       }

}

void DVH2::on_pushButton_2_clicked()
{
    string bodyloc;
    QString roi = ui->listWidget->currentItem()->text();
    string search=roi.toStdString();
    string line2 = outfile;
    line2.erase(0 ,line2.find_last_of("\\")+1);
    line2.erase(0,4);
    line2.erase(line2.find_last_of("."), line2.size());
    line2="in_"+line2;
    inputs=outfile;
    inputs.erase( inputs.find_last_of("\\"),inputs.size());
    inputs.erase( inputs.find_last_of("\\")+1,inputs.size());
     inputs+=line2;
    ofstream MyInputs(inputs, ios::app);

    ofstream myOutFile(outfile, ios::app);
            string line;
            double vol;
            myOutFile << endl;
            MyInputs << "1" << endl;
            myOutFile << search << endl;
            MyInputs<<search<<endl;
            location = "DVHdata\\";
            bodyloc=location;
            bodyloc+="BODY.txt";
            location += search;
            location += ".txt";
            ifstream file(location);
            getline(file, line);
            if(ui->CheckBoxVol->isChecked()){
            myOutFile << line << endl;
            MyInputs<<"1";
            ui->CheckBoxVol->setChecked(false);
             }
            else{
                MyInputs<<"0";}
            line.erase(0, line.find_first_of(" "));
            line.erase(line.find_last_of(" "), 4);
            vol = stold(line);
            getline(file, line);
            if(ui->checkBoxMin->isChecked()){
                myOutFile << line << endl;
                MyInputs<<"1";
                ui->checkBoxMin->setChecked(false);
                 }
            else{
                MyInputs<<"0";}
            getline(file, line);
            if(ui->checkBoxMax->isChecked()){
            myOutFile << line << endl;
            MyInputs<<"1";
            ui->checkBoxMax->setChecked(false);
             }
            else{
                MyInputs<<"0";}
            line.erase(0, line.find_first_of(" ") + 1);
            line.erase(0, line.find_first_of(" ") + 1);
            line.erase(line.find_last_of(" "), 4);
            getline(file, line);
            if(ui->checkBoxMean->isChecked()){
            myOutFile << line << endl;
            MyInputs<<"1";
            ui->checkBoxMean->setChecked(false);
             }
            else{
                MyInputs<<"0";}

            vector<string> dvhfile;
            dvhfile.clear();
            while (getline(file, line)){
                dvhfile.push_back(line);
            }
            file.close();
            if(ui->checkBox01->isChecked()){
                MyInputs<<"1";
                double vol1cm = 0.1 / vol * 100;
                if (vol1cm < 100){
                    myOutFile << "D(0,1cm^3): " << getAbsDoseonRelVol(dvhfile, vol1cm) << " cGy" << endl;
                }
                else{
                    myOutFile << "D(0,1cm^3): " << getAbsDoseonRelVol(dvhfile, 100) << " cGy" << endl;
                }
            ui->checkBox01->setChecked(false);
             }
            else{
                MyInputs<<"0";}

            if(ui->checkBox99->isChecked()){
                MyInputs<<"1";
            myOutFile << "D(99,9%): " << getAbsDoseonRelVol(dvhfile, 99.9) << " cGy" << endl << endl;
            ui->checkBox99->setChecked(false);
             }
            else{
                MyInputs<<"0";}

            if (ui->checkBox_HI->isChecked()){
                MyInputs<<"1";
                myOutFile << "Homogenity index: " << HI(dvhfile) << endl;
                myOutFile << endl;
                ui->checkBox_HI->setChecked(false);
            }
            else{
                MyInputs<<"0";}
            if(ui->checkBox_CN->isChecked()){
                MyInputs<<"1"<<endl;
                double bodymaxdose, bodyvol;
               ifstream bodyfile(bodyloc);
                vector<string> bodystring;
                bodystring.clear();
                getline(bodyfile, line);
                line.erase(0, line.find_first_of(" "));
                line.erase(line.find_last_of(" "), 4);
                bodyvol=stold(line);
                getline(bodyfile, line);
                getline(bodyfile, line);
                bodymaxdose = maxdose;
                getline(bodyfile, line);

                while (getline(bodyfile, line)){
                    bodystring.push_back(line);
                }
                bodyfile.close();
                double ptv95;
                double body95;
                double dose = maxdose*95/100;
                dose = dose * 10;
                dose = round(dose) / 10;
                double result = GetrelVolonAbsDose(dvhfile, dose);
                ptv95=result*vol/100;
                dose = bodymaxdose*95/100;
                dose = dose * 10;
                dose = round(dose) / 10;
                result = GetrelVolonAbsDose(bodystring, dose);
                body95=result*bodyvol/100;
                double CN = (ptv95 * ptv95)/(vol* body95);
//myOutFile << ptv95<<"*"<< ptv95<<"/"<<vol<<"*"<<body95;
                myOutFile << "Conformity Number: " << CN << endl;
                myOutFile << endl;
                ui->checkBox_CN->setChecked(false);
            }
            else{
                MyInputs<<"0"<<endl;}




    getints=new GetInputs(roi, dvhfile, outfile, maxdose,vol,inputs , this);
    getints->show();
    myOutFile.close();
    MyInputs.close();

}

void DVH2::on_pushButton_3_clicked()
{
    ofstream myOutFile(outfile, ios::app);
    myOutFile << endl << "END";
    myOutFile.close();
    ofstream MyInputs(inputs, ios::app);
    MyInputs<<"0"<<endl;
    MyInputs.close();
    if(ifinput!=1)
    {
     remove(inputs.c_str());
    }
    for (unsigned i = 0; i < structures.size(); i++){
            location = "DVHdata\\";
            location += structures.at(i);
            location += ".txt";
            remove(location.c_str());
        }
        QDir().rmdir("DVHdata");

        if(ui->checkBox->isChecked())
        {
            QString out=QString::fromStdString(outfile);
            out.replace("\\","/");
            QDesktopServices::openUrl(QUrl(out, QUrl::TolerantMode));
        }

        this->close();
}

void DVH2::on_listWidget_clicked(const QModelIndex &index)
{
  QString roi = ui->listWidget->currentItem()->text();
  string item=roi.toStdString();
  if(item.find("PTV") != string::npos){
      ui->checkBox_HI->setCheckable(true);
  if(body==1){ui->checkBox_CN->setCheckable(true);}
  else {ui->checkBox_CN->setCheckable(false);}
  }
  else  {
      ui->checkBox_HI->setCheckable(false);
      ui->checkBox_CN->setCheckable(false);
  }
}
