#ifndef ROIANALYZER_H
#define ROIANALYZER_H

#include <QDialog>
#include <vector>
#include <string>

using namespace std;

namespace Ui {
class ROIanalyzer;
}

class ROIanalyzer : public QDialog
{
    Q_OBJECT

public:
    explicit ROIanalyzer(QString name, vector<string> lines, string outfile,double maxdose, double volumen , QWidget *parent = 0);
    vector<string> dvhfile;
    string out;
    double maxdose;
    double vol;
    ~ROIanalyzer();



private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ROIanalyzer *ui;
};

#endif // ROIANALYZER_H
