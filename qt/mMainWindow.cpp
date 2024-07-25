#include "mMainWindow.h"

mMainWindow::mMainWindow(QWidget *parent) : QMainWindow(parent)
{
   // setup database
   mbase.setup_db();

   readSettings();


   // menu
   auto miscMenu = menuBar()->addMenu(tr("&Misc"));
   auto viewMenu = menuBar()->addMenu(tr("&View"));

   QAction* act1 = new QAction("Update Charts Theme");
   miscMenu->addAction(act1);
   connect(act1, SIGNAL(triggered()), this, SLOT(menuUpdateChartTheme()));

   QAction* act2 = new QAction("Reset Charts Size");
   miscMenu->addAction(act2);
   connect(act2, SIGNAL(triggered()), this, SLOT(menuResetChartSize()));

   QAction* act3 = new QAction("Default Table Filters");
   miscMenu->addAction(act3);
   connect(act3, SIGNAL(triggered()), this, SLOT(menuDefaultFilters()));





   // logview
   mLogView * mLogViewInstance = new mLogView(this);
   setCentralWidget(mLogViewInstance);

   // sessions table
   QDockWidget * dock = new QDockWidget(tr("Sessions"), this);
   mSessionsWidget * mSessionsWidgetInstance = new mSessionsWidget(dock);
   dock->setWidget(mSessionsWidgetInstance);
   dock->setObjectName("sessions");
   addDockWidget(Qt::TopDockWidgetArea, dock);
   viewMenu->addAction(dock->toggleViewAction());

   // sessions timeline
   dock = new QDockWidget(tr("Sessions Timeline"), this);
   dock->setObjectName("sessionsTimeline");
   mCurrentSessionTimelineWidget * mCurrentSessionTimelineWidgetInstance = new mCurrentSessionTimelineWidget(dock);
   dock->setWidget(mCurrentSessionTimelineWidgetInstance);
   addDockWidget(Qt::TopDockWidgetArea, dock);
   viewMenu->addAction(dock->toggleViewAction());

   readSettings();


}


void mMainWindow::writeSettings()
{
   mbase.settings->beginGroup("MainWindow");
   mbase.settings->setValue("geometry", saveGeometry());
   mbase.settings->setValue("state", saveState());
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
   restoreGeometry(mbase.settings->value("MainWindow/geometry").toByteArray());
   restoreState(mbase.settings->value("MainWindow/state").toByteArray());

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
