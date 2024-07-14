#include "mMainWindow.h"



mMainWindow::mMainWindow(QWidget *parent)
    : QMainWindow(parent)
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

mMainWindow::~mMainWindow()
{

}

void mMainWindow::writeSettings()
{
   QSettings settings(QSettings::IniFormat, QSettings::UserScope, "mwSoft", "pml");
   settings.beginGroup("MainWindow");
   settings.setValue("geometry", saveGeometry());
   settings.endGroup();
}

void mMainWindow::readSettings()
{
   QSettings settings(QSettings::IniFormat, QSettings::UserScope, "mwSoft", "pml");
   settings.beginGroup("MainWindow");
   const auto geometry = settings.value("geometry", QByteArray()).toByteArray();
   if (geometry.isEmpty()) setGeometry(200, 200, 400, 400);
   else restoreGeometry(geometry);
   settings.endGroup();
}

void mMainWindow::closeEvent(QCloseEvent *event)
{
   writeSettings();
   event->accept();
}

