#include <QApplication>
#include <QLayout>
#include <QResource>
#include "qtailermain.h"
#include "uctabcontaint.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load resources
    //QResource::registerResource("D:/RDK-Dev/Qt/Projects/QTailer/QTailer/Images.rcc");

    QTailerMain w;

    w.show();

    return a.exec();
}
