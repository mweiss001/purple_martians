#include "mMainWindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   QFile file("qdarkstyle/dark/darkstyle.qss");
   file.open(QFile::ReadOnly);
   QString styleSheet { QLatin1String(file.readAll()) };
   a.setStyleSheet(styleSheet);

   mMainWindow w;
   w.setWindowTitle("pm logs");
   w.show();
   return a.exec();
}
