#ifndef PREDOSE_H
#define PREDOSE_H

#include <QDialog>

namespace Ui {
class Predose;
}

class Predose : public QDialog
{
    Q_OBJECT

public:
    explicit Predose(QWidget *parent = 0, double* maxdose=NULL);
    ~Predose();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Predose *ui;
    double* predse;
};

#endif // PREDOSE_H
