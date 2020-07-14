
#ifndef DVH3_H
#define DVH3_H
#include <QDialog>
#include<iostream>
#include<fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <windows.h>

#include "predose.h"
using namespace std;

namespace Ui {
class DVH3;
}

class DVH3 : public QDialog
{
    Q_OBJECT

public:
    explicit DVH3(QWidget *parent = 0);
    ~DVH3();
    double maxdose;
    double* predse= &maxdose;
    QString input;
    QStringList dvhfile;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_commandLinkButton_clicked();

private:
    Ui::DVH3 *ui;

    Predose *preddose;
};


#endif // DVH3_H
