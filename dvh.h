#ifndef DVH_H
#define DVH_H

#include <QMainWindow>
#include "dvh1.h"
#include "dvh2.h"
#include "dvh3.h"
#include"Func.h"




namespace Ui {
class DVH;
}

class DVH : public QMainWindow
{
    Q_OBJECT

public:
    explicit DVH(QWidget *parent = 0);
    ~DVH();

private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();



private:
    Ui::DVH *ui;
    DVH1 *anal1;
    DVH2 *anal2;
    DVH3 *anal3;

};

#endif // DVH_H
