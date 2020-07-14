#include "dvh.h"
#include "planform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    planForm w;
    w.show();

    return a.exec();
}
