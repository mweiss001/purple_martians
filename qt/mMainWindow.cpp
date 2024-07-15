#include "mMainWindow.h"

mMainWindow::mMainWindow(QWidget *parent) : QMainWindow(parent)
{
   centralWidget = new QWidget;
   setCentralWidget(centralWidget);

   // setup database
   mbase.setup_db();

   readSettings();

   // menu
   auto miscMenu = menuBar()->addMenu(tr("&Misc"));

   QAction* act1 = new QAction("Update Charts Theme");
   miscMenu->addAction(act1);
   connect(act1, SIGNAL(triggered()), this, SLOT(menuUpdateChartTheme()));

   QAction* act2 = new QAction("Reset Charts Size");
   miscMenu->addAction(act2);
   connect(act2, SIGNAL(triggered()), this, SLOT(menuResetChartSize()));

//   auto submenu = fileMenu->addMenu("Submenu");
//   submenu->addAction(new QAction("action1"));
//   submenu->addAction(new QAction("action2"));

   // logview
   mLogViewInstance = new mLogView(this);
   QHBoxLayout *hbox = new QHBoxLayout;
   centralWidget->setLayout(hbox);
   hbox->addWidget(mLogViewInstance);
}


void mMainWindow::writeSettings()
{
   mbase.settings->beginGroup("MainWindow");
   mbase.settings->setValue("geometry", saveGeometry());
   mbase.settings->endGroup();

   mbase.settings->beginWriteArray("charts");
   for (int i=0; i<NUM_CHARTS; i++)
   {
      mbase.settings->setArrayIndex(i);
      mbase.settings->setValue("visible", mbase.statChartGraphTypeArray[i].visible);
      // qDebug() << "w:" << i << " - " <<mbase.statChartGraphTypeArray[i].visible;
   }
   mbase.settings->endArray();
}

void mMainWindow::readSettings()
{
   mbase.settings->beginGroup("MainWindow");
   const auto geometry = mbase.settings->value("geometry", QByteArray()).toByteArray();
   if (geometry.isEmpty()) setGeometry(200, 200, 400, 400);
   else restoreGeometry(geometry);
   mbase.settings->endGroup();

   int size = mbase.settings->beginReadArray("charts");
   for (int i = 0; i < size; ++i)
   {
      mbase.settings->setArrayIndex(i);
      mbase.statChartGraphTypeArray[i].visible = mbase.settings->value("visible", 1).toInt();
      // qDebug() << "r:" << i << " - " << mbase.statChartGraphTypeArray[i].visible;
   }
   mbase.settings->endArray();

   mbase.mChartsWidgetChartTheme = mbase.settings->value("mChartsWidgetChartTheme", 0).toInt();



}

void mMainWindow::closeEvent(QCloseEvent *event)
{
   writeSettings();
   event->accept();
}
