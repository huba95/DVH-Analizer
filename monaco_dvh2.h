#ifndef MONACO_DVH2_H
#define MONACO_DVH2_H

#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QString>
#include<iostream>
#include<fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <direct.h>
#include "monaco_getinputs.h"
#include "Func.h"

namespace Ui {
class Monaco_dvh2;
}

class Monaco_dvh2 : public QDialog
{
    Q_OBJECT

public:
    explicit Monaco_dvh2(QWidget *parent = nullptr, int ifinput=0);
    ~Monaco_dvh2();

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::Monaco_dvh2 *ui;
    std::string inputdvh;
    std::string line{};
    std::vector<std::string> roiname;
    std::ifstream myReadFile{};
    std::ofstream roifile{};
    std::string roipath;
    std::string path;
    std::string outpath;
    std::string inppath;
    std::string dirpath;
    std::string line2;
    std::ofstream MyOutfile;
    Monaco_Getinputs *getinputs;
    int ifinput;
};

#endif // MONACO_DVH2_H
