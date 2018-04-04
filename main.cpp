#include "mainwindow.h"
#include <QApplication>
#include "crchash.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    CRCHash<uint8_t> crchash(0x8CU, 0xFFU, 0xFFU);


    return a.exec();
}
