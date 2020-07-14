#include "monaco_dvh3.h"
#include "ui_monaco_dvh3.h"
#include <direct.h>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;


Monaco_dvh3::Monaco_dvh3(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Monaco_dvh3)
{
    ui->setupUi(this);
}

Monaco_dvh3::~Monaco_dvh3()
{
    delete ui;
}

void Monaco_dvh3::on_pushButton_2_clicked()
{
     dvhfile = QFileDialog::getOpenFileNames(this, tr("Open DVH files"),"C:\\" , "All files (*.*);;Text files (.**)");
}

void Monaco_dvh3::on_pushButton_clicked()
{
        input = QFileDialog::getOpenFileName(this, tr("Open INPUT file"),"C:\\" , "All files (*.*);;Text files (.**)");
}

void Monaco_dvh3::on_commandLinkButton_clicked()
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



        inputdvh = dvhfile[y].toStdString();

     myReadFile.open(inputdvh);
     path = inputdvh.substr(0, inputdvh.find_last_of("/")+1);
     std::string filename = inputdvh.substr(inputdvh.find_last_of("/")+1, inputdvh.length());
      outpath = path +"out_"+filename;



      MyOutfile.open(outpath);
     if (myReadFile.is_open())
     {
         std::getline(myReadFile, line);
         replace(line.begin(), line.end(), '|', '\n');
         MyOutfile << line<<std::endl<<std::endl<<std::endl;
         std::getline(myReadFile, line);
         std::getline(myReadFile, line);



    dirpath = path + "DVHData";
    _mkdir(dirpath.c_str());

     while (!getline(myReadFile, line).eof())
     {
         if (line == "") continue;
         line2 = line;
         line2.erase(line2.find("  "), line2.size());
         if (roiname.empty() || roiname.back() != line2)
         {
             roiname.push_back(line2);
             roipath=dirpath+ R"(\)" + line2;
             if (roifile.is_open())
                 roifile.close();
             roifile.open(roipath);
         }
         line.erase(0, line2.size());
         int i;
         for (i = 0; i < line.size(); i++) {
             if (line[i] == ' ')
                 continue;
             else
                 break;
         }
         line.erase(0, i);
         roifile << line << std::endl;
     }
      ui->progressBar->setValue(50);
      myReadFile.close();
      inputdvh.clear();

    }
     inppath = input.toStdString();
     std::ifstream MyInputs(inppath);

     getline(MyInputs, line);
     QMessageBox::information(this, QString::fromStdString(line), QString::fromStdString(line));
     c = stoi(line);
     while(c!=0)
     {
              getline(MyInputs, line);
                  QMessageBox::information(this, QString::fromStdString(line), QString::fromStdString(line));
              roipath = dirpath+ R"(\)" +line;

              MyOutfile<<std::endl<<line<<std::endl;
              std::ifstream file(roipath);
              roivec.clear();
              while (getline(file, line)){
                  roivec.push_back(line);
              }
              file.close();
               getline(MyInputs, line);
  //                  QMessageBox::information(this, QString::fromStdString(line), "Error");
               if(line.at(0)=='1')
               {
                   MyOutfile << "D(0,1cm^3): " << getAbsDoseonRelVol(roivec, 100) << " cGy" << endl;
               }
               if(line.at(1)=='1')
               {
                   MyOutfile << "D(99,9%): " << getAbsDoseonRelVol(roivec, 99.9) << " cGy" << endl;
               }
               if(line.at(2)=='1')
               {
                   MyOutfile << "Homogenity index: " << HI(roivec) << endl;
               }

               getline(MyInputs, line);
      //              QMessageBox::information(this, QString::fromStdString(line), "Error");
               s = stoi(line);

               while (s != 0){																//adatok keresése amíg a user kívánja
                   double result;
                    double dose;

                   switch (s){
                   case(1) :{
                      // double dose;
                       getline(MyInputs, line);
                       dose = stod(line);
                       result = GetrelVolonAbsDose(roivec, dose);

                       if (result >= 0){
                           if (exc!=1){
                           MyOutfile<<"V " << dose << " cGy = " << result << " %" << endl;}
                           else{
                               MyOutfile<<result << "\t";
                           }
                       }
                       else{
                            if (exc!=1){
                           MyOutfile << "Value (" << dose << ")not found" << endl;}
                            else{
                                MyOutfile << "Value (" << dose << ")not found" << "\t";
                            }
                       }
                       break;
   }
                   case(2) :{
                       double relvol;
                       getline(MyInputs, line);
                       relvol = stod(line);

                       result = getAbsDoseonRelVol(roivec, relvol);
                       if (result >= 0){
                           if (exc!=1){
                            MyOutfile<<"D " << relvol << " % = " << result << " cGy" << endl;}
                           else{
                               MyOutfile<< result << " \t";
                           }
                       }
                       else{
                            if (exc!=1){
                           MyOutfile << "Value (" << relvol << ")not found" << endl;}
                            else{
                                MyOutfile << "Value (" << dose << ")not found" << "\t";
                            }
                       }
                       break;}

                   default:
                       break;
                   }
                   getline(MyInputs, line);
        //                QMessageBox::information(this, QString::fromStdString(line), "Error");
                   s = stoi(line);
               }

               getline(MyInputs, line);
          //          QMessageBox::information(this, QString::fromStdString(line), "Error");
               c = stoi(line);
     }


          MyOutfile << std::endl << "END";
               //       fs::remove_all(dirpath);
          for (unsigned i = 0; i < roiname.size(); i++){
                  std::string location = dirpath + R"(\)" +  roiname.at(i);
                  QMessageBox::information(this, QString::fromStdString(location), QString::fromStdString(location));

                  remove(location.c_str());
              }
              QDir().rmdir(dirpath.c_str());

     MyInputs.close();
     MyOutfile.close();
     ui->progressBar->setValue(100);
}
    double nMilliseconds = myTimer.elapsed();
    QString status = QString("END of Process\n Elapsed time: %1min").arg(nMilliseconds/60000);
    QMessageBox::information(this, tr("FILE"), status );this->close();
}

