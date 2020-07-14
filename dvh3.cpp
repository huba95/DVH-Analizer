#include "dvh3.h"
#include "ui_dvh3.h"
#include "Func.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QTime>

#include<algorithm>
#include<iostream>
#include<fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;


using namespace std;


DVH3::DVH3(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DVH3)
{
    ui->setupUi(this);
}

DVH3::~DVH3()
{
    delete ui;
}


void DVH3::on_pushButton_2_clicked()
{
    dvhfile = QFileDialog::getOpenFileNames(this, tr("Open DVH files"),"C:\\" , "All files (*.*);;Text files (.**)");

}

void DVH3::on_pushButton_clicked()
{

    input = QFileDialog::getOpenFileName(this, tr("Open INPUT file"),"C:\\" , "All files (*.*);;Text files (.**)");

}

void DVH3::on_commandLinkButton_clicked()
{

    int exc=0;                                          //ha be van pipálva az excel kompatibilis ebbe tároljuk
    if(ui->checkBox->isChecked()){exc=1;}
    QTime myTimer;
    myTimer.start();

    for(int y=0; y<dvhfile.size(); y++){                //y-ban tároljuk, hogy hanyadik fájlt elemezzük és végigmegyünk rajtuk
        QString label(QString::number(y+1));            //ebben a 4 sorban a labelt állítjuk elő hogy hány fájlból hanyadikat elemezzük
        label+="/";
        label+=QString::number(dvhfile.size());
        ui->label->setText(label);
        QDir().mkdir("DVHData");                        //létrehozzuk a DVHData mappát ahova összevágjuk a fájlokat
        vector<string> structures;											//ebben tároljuk a struktúrák elnevezéseit

        unsigned int i, n = 1;
        int status=0;

        string line;														//ebbe olvassuk a sorokat
        string location;
        string outfile;														//ebben tároljuk a kimeneti fájl nevét
        string search;                                                  //ebbe olvassuk be a 8 jegyű számot a pipákhoz
        std::string inputs;
        std::string inputdvh;

        dvhfile[y].replace("/","\\");                                   //Windowsnak érthető filepath létrehozása
        line=dvhfile.at(y).toStdString();                               //c++-os sztring létrhozása
        string line2 = dvhfile.at(y).toStdString();
        line2.erase( line2.find_last_of("\\")+1, line2.size());
        line2+="Outfiles";
        QDir().mkdir(QString::fromStdString(line2));                        //Outfiles Mappa lérehozása
        line.erase(0, line.find_last_of("\\")+1);
        outfile = line2+"\\out_" + line + ".txt";                           //kimeneti fájl létrehozása
        line.clear();

        input.replace("/","\\");

        inputs=input.toStdString();

        QString dvh=dvhfile.at(y);
        dvh.replace("/","\\");
        inputdvh=dvhfile.at(y).toStdString();

        if(exc==1){outfile="Outfile.txt";}

        ofstream myOutFile(outfile, ios::app);
        ifstream MyInputs(inputs);
        if (y==0 && exc==1)         //ha az első ciklusban vagyunk és excel kompatibilis a kimenet, itt gyártjuk le a fejlécet
        {
            int z=0;
            if (MyInputs.is_open()) {
                myOutFile << "Patient Name\tPatient ID\t";
                getline(MyInputs, line);
                while (!MyInputs.eof()) {
                    cout << '.' << endl;
                    if (isalpha(line.at(0))) {

                        if (z > 0) {
                            z = z - 2;
                            z = z / 2;
                            for (int k = 0; k <= z; k++) {
                                myOutFile << '\t';
                            }
                            z = 0;
                        }
                        myOutFile << line << '\t';
                        getline(MyInputs, line);
                        int i = 0;
                        while (i<8) {
                        if (line[i]=='1')
                            myOutFile << '\t';
                        i++;
                        }
                    }

                    getline(MyInputs, line);
                    z++;
                }
            }
            myOutFile << endl;
            myOutFile << "\t\t\t";
            MyInputs.clear();
            MyInputs.seekg(0, ios::beg);
            getline(MyInputs, line);
            while (!MyInputs.eof()) {
                getline(MyInputs, line);
                if (isalpha(line[0])) {
                    getline(MyInputs, line);
                    //cout << line;
                   // QMessageBox::information(this, tr("FILE"), "Error");
                    line[0] == '1' ? (myOutFile << "Volume\t") : (myOutFile << "");
                    line[1] == '1' ? (myOutFile << "DMax\t") : (myOutFile << "");
                    line[2] == '1' ? (myOutFile << "DMin\t") : (myOutFile << "");
                    line[3] == '1' ? (myOutFile << "DMean\t") : (myOutFile << "");
                    line[4] == '1' ? (myOutFile << "D(0,1cm^3)\t") : (myOutFile << "");
                    line[5] == '1' ? (myOutFile << "D(99,9%)\t") : (myOutFile << "");
                    line[6] == '1' ? (myOutFile << "HI\t") : (myOutFile << "");
                    line[7] == '1' ? (myOutFile << "CN\t") : (myOutFile << "");

                }
                if (isdigit(line[0])) {
                    int d = stoi(line);
                    double dvh;
                    switch (d) {
                        case(0):
                            getline(MyInputs, line);
                            myOutFile <<"\t\t";
                            break;
                        case(1):
                            getline(MyInputs, line);
                            dvh = stold(line);
                            myOutFile << "V(" << dvh << "cGy)%\t";
                            break;
                        case(2):
                            getline(MyInputs, line);
                            dvh = stold(line);
                            myOutFile << "D(" << dvh << "%)cGy\t";
                            break;
                        case(3) :
                            getline(MyInputs, line);
                            dvh = stold(line);
                            myOutFile << "V(" << dvh << "%)%\t";
                            break;
                        case(4) :
                            getline(MyInputs, line);
                            dvh = stold(line);
                            myOutFile << "V(" << dvh << "%)cm^3\t";
                            break;
                        case(5) :
                            getline(MyInputs, line);
                            dvh = stold(line);
                            myOutFile << "D(" << dvh << "cm^3)cGy\t";
                            break;
                    }
                }
        }
        }
        MyInputs.clear();
        MyInputs.seekg(0, ios::beg);
        ifstream myReadFile(inputdvh);
        if (myReadFile.is_open()) {
             ui->progressBar->setValue(5);
             status=6;
             if (exc!=1){
            getline(myReadFile, line);                                          //kiírjuk az elsö két sort, ami a beteg nevét és azonosítóját tartalmazza
            myOutFile << line << endl;
            getline(myReadFile, line);
            myOutFile << line << endl << endl << "Structures:" << endl;}        //Structures felirat kiírása
             else{
                   getline(myReadFile, line);
                   line.erase(0,23);
                    myOutFile<<endl << line << "\t";
                     getline(myReadFile, line);
                     line.erase(0,23);
                      myOutFile << line << "\t\t";
             }
            while (!myReadFile.eof()) {
                while (structsearch(line) == false){						//amíg nem talál struktúrát olvas
                    getline(myReadFile, line);
                    if (line.compare(0, 10, "Prescribed") == 0) {           //Prescribed dose keresése
                            line.erase(0, 23);
                            if (isdigit(line.c_str()[0])) {
                                maxdose = stold(line);
                            }
                            else {
                                preddose=new Predose(this, predse);         //ha nem talál rákérdez
                                preddose->show();
                            }

                            }
                }

                if (structsearch(line) == true)								//ha struktúrát talál elindul
                {
                    ui->progressBar->setValue(status);
                    status+=2;
                    line.erase(0, 11);										//csak a struktúra nevét hagyja meg
                    structures.push_back(line);									//kiírjuk a struktúrát
                    if (exc!=1){
                    myOutFile << line << endl;}								//beírjuk a kimeneti fájlba is, ha nem exceles a kimenet
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

        ui->progressBar->setValue(50);
        status=55;
if (exc!=1){
        myOutFile << endl;}

        getline(MyInputs, search);
        int s;
        int c = 5;
        int p;
        double vol;
         double reldose;

        while (c != 0){
            s = 7;
            p = -5;
            while (p < 0){
                search.erase();


                getline(MyInputs, search);
                for (unsigned q = 0; q < structures.size(); q++){
                    if (search.compare(0, search.size(), structures.at(q)) == 0){
                        p = q;
                        break;
                    }
                    else{ p = -5; }
                }
            }
            ui->progressBar->setValue(status);
            status+=5;

            string bodyloc;
            if (exc!=1){
            myOutFile << endl;
            myOutFile << structures.at(p) << endl;	}								//kiírjuk fájlba is
            location = "DVHdata\\";
            bodyloc=location;
            bodyloc+="BODY.txt";
            location += structures.at(p);
            location += ".txt";
            ifstream file(location);													//megnyitjuk és beolvassuk a fájlt majd bezárjuk
            getline(file, line);
            getline(MyInputs, search);
            if(search.at(0)=='1'){
                if (exc!=1){
            myOutFile << line << endl;}
                line.erase(0, line.find_first_of(" "));
                line.erase(line.find_last_of(" "), 4);
                vol = stold(line);
            if (exc==1){ myOutFile << vol<<"\t";}}
            getline(file, line);
            if(search.at(1)=='1'){
                 if (exc!=1){
            myOutFile << line << endl;}
             if (exc==1){
                 line.erase(0, line.find_first_of(" ") + 1);
                 line.erase(0, line.find_first_of(" ") + 1);
                 line.erase(line.find_last_of(" "), 4);
                  myOutFile << line<<"\t";
             }}
            getline(file, line);
            if(search.at(2)=='1'){
                 if (exc!=1){
            myOutFile << line << endl;}
             if (exc==1){
            line.erase(0, line.find_first_of(" ") + 1);
            line.erase(0, line.find_first_of(" ") + 1);
            line.erase(line.find_last_of(" "), 4);
            myOutFile << line<<"\t";
             }}
            getline(file, line);
            if(search.at(3)=='1'){
                if (exc!=1){
            myOutFile << line << endl;}
            if (exc==1){
           line.erase(0, line.find_first_of(" ") + 1);
           line.erase(0, line.find_first_of(" ") + 1);
           line.erase(line.find_last_of(" "), 4);
           myOutFile << line<<"\t";
            }}
            ui->progressBar->setValue(status);
            status+=5;


            vector<string> dvhfile;
            dvhfile.clear();
            while (getline(file, line)){
                dvhfile.push_back(line);
            }
            file.close();

            double vol1cm = 0.1 / vol * 100;
            if(search.at(4)=='1'){
            if (vol1cm < 100){
                if (exc!=1){
                myOutFile <<  "D(0.1cm^3):" << getAbsDoseonRelVol(dvhfile, vol1cm) << "cGy" << endl;}
                else{
                    myOutFile <<  getAbsDoseonRelVol(dvhfile, vol1cm)<< "\t";
                }

            }
            else{
                 if (exc!=1){
                myOutFile << "D(0.1cm^3):" << getAbsDoseonRelVol(dvhfile, 100) << "cGy" << endl;}
                 else{
                     myOutFile << getAbsDoseonRelVol(dvhfile, 100) << "\t";
                 }

            }
}
            if(search.at(5)=='1'){
                if (exc!=1){
            myOutFile << "D(99,9%):" << getAbsDoseonRelVol(dvhfile, 99.9) << "cGy" << endl << endl;}
            else{
                    myOutFile << getAbsDoseonRelVol(dvhfile, 99.9) << "\t";
                }
            }
            ui->progressBar->setValue(status);
            status+=5;

             if(search.at(6)=='1'){
                 if (exc!=1){
                 myOutFile << "Homogenity index: " << HI(dvhfile) << endl;
                 myOutFile << endl;}
                 else{
                     myOutFile<< HI(dvhfile) << "\t";
                 }
             }
             ui->progressBar->setValue(status);
             status+=5;
             if(search.at(7)=='1'){
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
                 //myOutFile <<"dose"<< dose<<endl;
                 dose = dose * 10;
                 dose = round(dose) / 10;
                 double result = GetrelVolonAbsDose(dvhfile, dose);
                 //myOutFile <<"result"<< result<<endl;
                 ptv95=result*vol/100;
                // myOutFile <<"ptv95 "<< ptv95<<endl;
                 dose = bodymaxdose*95/100;
                 dose = dose * 10;
                 dose = round(dose) / 10;
                 result = GetrelVolonAbsDose(bodystring, dose);
                 body95=result*bodyvol/100;
                 double CN = (ptv95 * ptv95)/(vol* body95);
              //   myOutFile << ptv95<<"*"<< ptv95<<"/"<<vol<<"*"<<body95<<" maxdose:"<<maxdose<<" "<<bodymaxdose;
  if (exc!=1){
                 myOutFile << "Conformity Number: " << CN << endl;
                 myOutFile << endl;}
  else{
  myOutFile << CN << "\t";
  }
             }
             ui->progressBar->setValue(status);
             status+=5;




            getline(MyInputs, line);
            s = stoi(line);

            while (s != 0){																//adatok keresése amíg a user kívánja
                double result;
                 double dose;

                switch (s){
                case(1) :{
                    double dose;
                    getline(MyInputs, line);
                    dose = stod(line);
                    result = GetrelVolonAbsDose(dvhfile, dose);

                    if (result >= 0){
                        if (exc!=1){
                        myOutFile<<"V " << dose << " cGy = " << result << " %" << endl;}
                        else{
                            myOutFile<<result << "\t";
                        }
                    }
                    else{
                         if (exc!=1){
                        myOutFile << "Value (" << dose << ")not found" << endl;}
                         else{
                             myOutFile << "Value (" << dose << ")not found" << "\t";
                         }
                    }
                    ui->progressBar->setValue(status);
                    status+=2;
                    break;
}
                case(2) :{
                    double relvol;
                    getline(MyInputs, line);
                    relvol = stod(line);

                    result = getAbsDoseonRelVol(dvhfile, relvol);
                    if (result >= 0){
                        if (exc!=1){
                         myOutFile<<"D " << relvol << " % = " << result << " cGy" << endl;}
                        else{
                            myOutFile<< result << " \t";
                        }
                    }
                    else{
                         if (exc!=1){
                        myOutFile << "Value (" << relvol << ")not found" << endl;}
                         else{
                             myOutFile << "Value (" << dose << ")not found" << "\t";
                         }
                    }
                    ui->progressBar->setValue(status);
                    status+=2;
                    break;}
                case(3):{

                    getline(MyInputs, line);
                    reldose = stod(line);
                    dose = reldose*maxdose / 100;
                    dose = dose * 10;
                    dose = round(dose) / 10;
                    result = GetrelVolonAbsDose(dvhfile, dose);
                        if (exc!=1){
                    myOutFile<<"V " << dose << " % = " << result << " %" << endl;}
                        else{
                            myOutFile<< result << " \t";
                        }
                    ui->progressBar->setValue(status);
                    status+=2;
                    break;}
                case(4):{
                    getline(MyInputs, line);
                    reldose = stod(line);
                    dose = reldose*maxdose / 100;
                    dose = dose * 10;
                    dose = round(dose) / 10;
                    result = GetrelVolonAbsDose(dvhfile, dose);
                    result=result*vol/100;
                    if (exc!=1){
                    myOutFile<<"V " << dose << " % = " << result << " cm^3" << endl;}
                    else{
                        myOutFile<< result << " \t";
                    }
                    ui->progressBar->setValue(status);
                    status+=2;
                    break;}

                case(5):{
                     getline(MyInputs, line);
                    double absvol = stold(line);
                    double relvol=absvol/vol*100;
                    result = getAbsDoseonRelVol(dvhfile, relvol);
                    if (exc!=1){
                    myOutFile<<"D " << absvol << " cm^3 = " << result << " cGy" << endl;}
                    else{
                        myOutFile<< result << " \t";
                    }
                    break;}

                default:

                    break;
                }
                getline(MyInputs, line);
                s = stoi(line);
            }
            if(exc==1){myOutFile<<"\t\t";}
            getline(MyInputs, line);
            c = stoi(line);
        }
          if (exc!=1){ myOutFile << "END";}



        myOutFile << endl;

        for (i = 0; i < structures.size(); i++){
            location = "DVHdata\\";														//elkészítjük az útvonalat
            fs::remove_all(location);
            location += structures.at(i);
            location += ".txt";
            remove(location.c_str());
        }
        QDir().rmdir("DVHdata");


        myOutFile.close();
        ui->progressBar->setValue(100);
}
    double nMilliseconds = myTimer.elapsed();
    QString status = QString("END of Process\n Elapsed time: %1min").arg(nMilliseconds/60000);
    QMessageBox::information(this, tr("FILE"), status );this->close();
}
