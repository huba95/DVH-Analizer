#include "dvh1.h"
#include "ui_dvh1.h"
#include "Func.h"
#include "roianalyzer.h"
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



using namespace std;


DVH1::DVH1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DVH1)
{
    ui->setupUi(this);
    ui->checkBox_HI->setCheckable(false);
    ui->checkBox_CN->setCheckable(false);
}

DVH1::~DVH1()
{
    delete ui;
}



void DVH1::on_pushButton_clicked()
{
    QString dvhfile;
       dvhfile = QFileDialog::getOpenFileName(this, tr("Open DVH file"),"C:\\" , "All files (*.*);;Text files (*.*)");
       QMessageBox::information(this, tr("FILE"), dvhfile);

       QDir().mkdir("DVHData");

       unsigned  n = 1;
       int status=5;

       string line;														//ebbe olvassuk a sorokat


       std::string inputdvh;


       dvhfile.replace("/","\\");
       line=dvhfile.toStdString();
       string line2 = dvhfile.toStdString();
       line2.erase( line2.find_last_of("\\")+1, line2.size());
       line2+="Outfiles";
       QDir().mkdir(QString::fromStdString(line2));
       line.erase(0, line.find_last_of("\\")+1);


       string outfile = line2+"\\out_" + line + ".txt";

       line.clear();
       inputdvh=dvhfile.toStdString();

       ofstream myOutFile(outfile.c_str());
       ifstream myReadFile(inputdvh.c_str());
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
               }

               if (structsearch(line) == true)								//ha struktúrát talál elindul
               {
                   ui->progressBar->setValue(status);
                   status+=5;
                   line.erase(0, 11);
                   if(line=="BODY"){body=1;}
                   structures.push_back(line);
                   ui->listWidget->addItem(QString::fromStdString(line));
                   myOutFile << line << endl;								//beírjuk a kimeneti fájlba is
                   location = "DVHdata\\";									//elkészítjük a részfájl elérési útját
                   location += line;
                   location += ".txt";
                   ofstream stuff(location.c_str());								//létrehozzuk/megnyitjuk a részfájlt
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

}

void DVH1::on_pushButton_2_clicked()
{
        string bodyloc;
    QString roi = ui->listWidget->currentItem()->text();
    string search=roi.toStdString();


    ofstream myOutFile(outfile.c_str(), ios::app);
            string line;
            double vol, maxdose;

            myOutFile << endl;
            myOutFile << search << endl;
            location = "DVHdata\\";
            bodyloc=location;
            bodyloc+="BODY.txt";
            location += search;
            location += ".txt";
            ifstream file(location.c_str());
            getline(file, line);
            if(ui->CheckBoxVol->isChecked()){
            myOutFile << line << endl;
            ui->CheckBoxVol->setChecked(false);
             }
            line.erase(0, line.find_first_of(" "));
            line.erase(line.find_last_of(" "), 4);
            vol = stold(line);
            getline(file, line);
            if(ui->checkBoxMin->isChecked()){
                myOutFile << line << endl;
                ui->checkBoxMin->setChecked(false);
                 }
            getline(file, line);
            if(ui->checkBoxMax->isChecked()){
            myOutFile << line << endl;
            ui->checkBoxMax->setChecked(false);
             }
            line.erase(0, line.find_first_of(" ") + 1);
            line.erase(0, line.find_first_of(" ") + 1);
            line.erase(line.find_last_of(" "), 4);
            maxdose = stold(line);
            getline(file, line);
            if(ui->checkBoxMean->isChecked()){
            myOutFile << line << endl;
            ui->checkBoxMean->setChecked(false);
             }

            vector<string> dvhfile;
            dvhfile.clear();
            while (getline(file, line)){
                dvhfile.push_back(line);
            }
            file.close();
            if(ui->checkBox01->isChecked()){
                double vol1cm = 0.1 / vol * 100;
                if (vol1cm < 100){
                    myOutFile << "D(0,1cm^3): " << getAbsDoseonRelVol(dvhfile, vol1cm) << " cGy" << endl;
                }
                else{
                    myOutFile << "D(0,1cm^3): " << getAbsDoseonRelVol(dvhfile, 100) << " cGy" << endl;
                }
            ui->checkBox01->setChecked(false);
             }

            if(ui->checkBox99->isChecked()){
            myOutFile << "D(99,9%): " << getAbsDoseonRelVol(dvhfile, 99.9) << " cGy" << endl << endl;
            ui->checkBox99->setChecked(false);
             }

            if (ui->checkBox_HI->isChecked()){
                myOutFile << "Homogenity index: " << HI(dvhfile) << endl;
                myOutFile << endl;
                ui->checkBox_HI->setChecked(false);
            }

            if(ui->checkBox_CN->isChecked()){
                double bodymaxdose, bodyvol;
               ifstream bodyfile(bodyloc.c_str());
                vector<string> bodystring;
                bodystring.clear();
                getline(bodyfile, line);
                line.erase(0, line.find_first_of(" "));
                line.erase(line.find_last_of(" "), 4);
                bodyvol=stold(line);
                getline(bodyfile, line);
                getline(bodyfile, line);
                line.erase(0, line.find_first_of(" ") + 1);
                line.erase(0, line.find_first_of(" ") + 1);
                line.erase(line.find_last_of(" "), 4);
                bodymaxdose = stold(line);
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

                myOutFile << "Conformity Number: " << CN << endl;
                myOutFile << endl;
                ui->checkBox_CN->setChecked(false);
            }



myOutFile.close();
    analroi=new ROIanalyzer(roi, dvhfile, outfile, maxdose, vol, this);
    analroi->show();


}

void DVH1::on_pushButton_3_clicked()
{
    ofstream myOutFile(outfile.c_str(), ios::app);
    myOutFile << endl << "END";
    myOutFile.close();
    for (unsigned i = 0; i < structures.size(); i++){
            location = "DVHdata\\";														//elkészítjük az útvonalat
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

void DVH1::on_listWidget_clicked(const QModelIndex &index)
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
