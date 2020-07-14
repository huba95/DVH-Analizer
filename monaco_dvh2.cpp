#include "monaco_dvh2.h"
#include "ui_monaco_dvh2.h"


Monaco_dvh2::Monaco_dvh2(QWidget *parent, int ifinput) :
    QDialog(parent),
    ui(new Ui::Monaco_dvh2),
    ifinput(ifinput)
{
    ui->setupUi(this);
}

Monaco_dvh2::~Monaco_dvh2()
{
    delete ui;
}

void Monaco_dvh2::on_pushButton_4_clicked()
{
    QString dvhfile;
       dvhfile = QFileDialog::getOpenFileName(this, tr("Open DVH file"),"C:\\" , "All files (*.*);;Text files (*.*)");
       QMessageBox::information(this, tr("FILE"), dvhfile);
       inputdvh = dvhfile.toStdString();

    myReadFile.open(inputdvh);
    path = inputdvh.substr(0, inputdvh.find_last_of("/")+1);
    std::string filename = inputdvh.substr(inputdvh.find_last_of("/")+1, inputdvh.length());
    //outpath = path +"Results.txt";
     outpath = path +"out_"+filename;
     inppath = path +"in_"+filename;
        //   QMessageBox::information(this, tr("FILE"), QString::fromStdString(outpath));

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
    //std::cout << path << std::endl;

    while (!getline(myReadFile, line).eof())
    {
        if (line == "") continue;
        line2 = line;
        line2.erase(line2.find("  "), line2.size());
        if (roiname.empty() || roiname.back() != line2)
        {
            roiname.push_back(line2);
            roipath=dirpath+ R"(\)" + line2;
            ui->listWidget_2->addItem(QString::fromStdString(line2));
             //QMessageBox::information(this, tr("FILE"), QString::fromStdString(line2));
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
    ui->progressBar_2->setValue(100);
}
else{
    QMessageBox::information(this, tr("FILE"), "Error");this->close();}
myReadFile.close();
roifile.close();
MyOutfile.close();
}

void Monaco_dvh2::on_pushButton_5_clicked()
{

    QString roi = ui->listWidget_2->currentItem()->text();
    std::string roiname2 = roi.toStdString();
    roipath = dirpath + R"(\)" + roiname2;

    std::ofstream input(inppath,  std::ios::app);
    input<<"1"<<std::endl<<roiname2<<std::endl;


    MyOutfile.open(outpath, std::ios::app);
    if(MyOutfile.is_open())
    {

        MyOutfile<<std::endl<<roiname2<<std::endl;
        std::ifstream file(roipath);

        std::vector<std::string> dvhfile;
        dvhfile.clear();
        while (getline(file, line)){
            dvhfile.push_back(line);
        }
        file.close();

        if(ui->checkBox01_2->isChecked()){
            input<<"1";
                MyOutfile << "D(0,1cm^3): " << getAbsDoseonRelVol(dvhfile, 100) << " cGy" << endl;

        ui->checkBox01_2->setChecked(false);
         }
        else{
            input<<"0";}

        if(ui->checkBox99_2->isChecked()){
            input<<"1";
        MyOutfile << "D(99,9%): " << getAbsDoseonRelVol(dvhfile, 99.9) << " cGy" << endl;
        ui->checkBox99_2->setChecked(false);
         }
        else{
            input<<"0";}

        if (ui->checkBox_HI_2->isChecked()){
            input<<"1";
            MyOutfile << "Homogenity index: " << HI(dvhfile) << endl;
           // MyOutfile << endl;
            ui->checkBox_HI_2->setChecked(false);
        }
        else{
            input<<"0";}


        getinputs=new Monaco_Getinputs(roi, dvhfile, outpath, inppath, this);
        getinputs->show();




    }
    else {
        QMessageBox::information(this, tr("FILE"), "Error. Couldn`t open outfile.");
    }


   // input<<"0"<<std::endl;
MyOutfile<<std::endl;
MyOutfile.close();
    input.close();
}

void Monaco_dvh2::on_pushButton_6_clicked()
{    std::ofstream myOutFile(outpath, std::ios::app);
     myOutFile << std::endl << "END";
     myOutFile.close();
     std::ofstream MyInputs(inppath, std::ios::app);
     MyInputs<<"0"<<std::endl;
     MyInputs.close();
     if(ifinput!=1)
     {
      remove(inppath.c_str());
     }
     for (unsigned i = 0; i < roiname.size(); i++){
             std::string location = dirpath + R"(\)" +  roiname.at(i);
             remove(location.c_str());
         }
         QDir().rmdir(dirpath.c_str());

         if(ui->checkBox->isChecked())
         {
             QString out=QString::fromStdString(outpath);
             out.replace("\\","/");
             QDesktopServices::openUrl(QUrl(out, QUrl::TolerantMode));
         }

         this->close();
}
