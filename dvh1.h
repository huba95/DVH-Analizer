
#ifndef DVH1_H
#define DVH1_H

#include <QDialog>
#include <vector>
#include <string>

#include "roianalyzer.h"

using namespace std;


namespace Ui {
class DVH1;
}

class DVH1 : public QDialog
{
    Q_OBJECT

public:

    vector<string> structures;
    static ofstream myOutFile();
    string location;
    string outfile;
    int body;
    explicit DVH1(QWidget *parent = 0);
    ~DVH1();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_listWidget_clicked(const QModelIndex &index);

private:
    Ui::DVH1 *ui;
      ROIanalyzer *analroi;
};



#endif // DVH1_H
