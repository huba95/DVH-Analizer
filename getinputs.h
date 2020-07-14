#ifndef GETINPUTS_H
#define GETINPUTS_H

#include <QDialog>

#include <vector>
#include <string>
using namespace std;

namespace Ui {
class GetInputs;
}

class GetInputs : public QDialog
{
    Q_OBJECT

public:
    explicit GetInputs(QString name, vector<string> lines, string outfile,double maxdose, double volumen, string inputfile , QWidget *parent = 0);
    vector<string> dvhfile;
    string out;
    string in;
    double maxdose;
    double vol;
    ~GetInputs();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


private:
    Ui::GetInputs *ui;
};

#endif // GETINPUTS_H
