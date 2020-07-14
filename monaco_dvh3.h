#ifndef MONACO_DVH3_H
#define MONACO_DVH3_H

#include <QDialog>
#include<iostream>
#include<fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>


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

namespace Ui {
class Monaco_dvh3;
}

class Monaco_dvh3 : public QDialog
{
    Q_OBJECT

public:
    explicit Monaco_dvh3(QWidget *parent = nullptr);
    ~Monaco_dvh3();
    QString input;
    QStringList dvhfile;
    string line;														//ebbe olvassuk a sorokat
    string location;
    string outfile;														//ebben tároljuk a kimeneti fájl nevét
    string search;                                                  //ebbe olvassuk be a 8 jegyű számot a pipákhoz
    std::string inputs;
    std::string inputdvh;
    std::string dirpath;
    std::string path;
    std::ofstream MyOutfile;
    std::ifstream myReadFile{};
    std::ofstream roifile{};
    std::string outpath;
    std::string inppath;
    std::string roipath;
    std::vector<std::string> roiname;
    std::string line2;
    std::vector<std::string> roivec;
    int c, s;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_commandLinkButton_clicked();

private:
    Ui::Monaco_dvh3 *ui;
};

#endif // MONACO_DVH3_H
