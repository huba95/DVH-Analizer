#ifndef PLANFORM_H
#define PLANFORM_H

#include <QDialog>
#include <vector>
#include <string>
#include "dvh.h"
#include "monaco_main.h"

namespace Ui {
class planForm;
}

class planForm : public QDialog
{
    Q_OBJECT

public:
    explicit planForm(QWidget *parent = 0);
    ~planForm();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::planForm *ui;
    DVH *anal;
    Monaco_main *anal2;
};

#endif // PLANFORM_H
