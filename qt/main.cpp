#include "mMainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mMainWindow w;
    w.setWindowTitle("pm logs");
    w.show();
    return a.exec();
}
