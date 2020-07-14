#ifndef DVH2_H
#define DVH2_H

#include <QDialog>
#include <vector>
#include <string>

#include "getinputs.h"
#include "predose.h"


using namespace std;

namespace Ui {
class DVH2;
}

class DVH2 : public QDialog
{
    Q_OBJECT

public:
    vector<string> structures;
    string inputs;
    static ofstream myOutFile();
    string location;
    string outfile;
    int body;
    double maxdose;
    int ifinput;
    double* predse= &maxdose;
    explicit DVH2(QWidget *parent = 0, int inp=0);
    ~DVH2();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


    void on_listWidget_clicked(const QModelIndex &index);

private:
    Ui::DVH2 *ui;

    GetInputs *getints;
    Predose *preddose;
};

#endif // DVH2_H
