#ifndef MONACO_MAIN_H
#define MONACO_MAIN_H

#include <QDialog>
#include "monaco_dvh2.h"
#include "monaco_dvh3.h"

namespace Ui {
class Monaco_main;
}

class Monaco_main : public QDialog
{
    Q_OBJECT

public:
    explicit Monaco_main(QWidget *parent = nullptr);
    ~Monaco_main();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Monaco_main *ui;
    Monaco_dvh2 *mon2;
    Monaco_dvh3 *mon3;
    int ifinp;
};

#endif // MONACO_MAIN_H
