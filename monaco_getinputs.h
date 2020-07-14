#ifndef MONACO_GETINPUTS_H
#define MONACO_GETINPUTS_H

#include <QDialog>

namespace Ui {
class Monaco_Getinputs;
}

class Monaco_Getinputs : public QDialog
{
    Q_OBJECT

public:
    explicit Monaco_Getinputs(QString name, std::vector<std::string> lines, std::string outfile, std::string inputfile, QWidget *parent = nullptr);
    ~Monaco_Getinputs();
    std::vector<std::string> dvhfile;
    std::string out;
    std::string in;
    double maxdose;
    double vol;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Monaco_Getinputs *ui;
};

#endif // MONACO_GETINPUTS_H
