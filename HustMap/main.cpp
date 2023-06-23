#include "HustMap.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    HustMap w;
    w.show();
    return a.exec();
}
