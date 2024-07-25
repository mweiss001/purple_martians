#include "mLogView.h"

mLogView::mLogView(QWidget *parent) : QWidget{parent}
{

// ---------------------------------------------------
// first create all the things I will show here
// ---------------------------------------------------
   mTablesWidget * mTablesWidgetInstance = new mTablesWidget(this);
   mChartsWidget * mChartsWidgetInstance = new mChartsWidget(this);

   // ---------------------------------------------------------
   // middle panel
   // ---------------------------------------------------------
   QFrame * middlePanelFrame = new QFrame();
   middlePanelFrame->setLineWidth(0);
   middlePanelFrame->setMidLineWidth(0);
   middlePanelFrame->setFrameStyle(QFrame::Panel);
   middlePanelFrame->setMaximumWidth(360);

   // make vbox for frame
   QVBoxLayout *MPFvbox = new QVBoxLayout;

   // add vbox layout to frame
   middlePanelFrame->setLayout(MPFvbox);

   // ---------------------------------------------------------------------
   // mTablesWidgetControlWidget
   // ---------------------------------------------------------------------
   mTablesWidgetControlWidget * mTablesWidgetControlWidgetInstance = new mTablesWidgetControlWidget(this);
   MPFvbox->addWidget(mTablesWidgetControlWidgetInstance);

   // ---------------------------------------------------
   // mChartsWidgetControlWidgetInstance
   // ---------------------------------------------------
   mChartsWidgetControlWidget * mChartsWidgetControlWidgetInstance = new mChartsWidgetControlWidget(this);

   // put it in its own hbox so I can push it over to the right
   QHBoxLayout *MPFhbox = new QHBoxLayout;
   MPFvbox->addLayout(MPFhbox);

   MPFhbox->addStretch();
   MPFhbox->addWidget(mChartsWidgetControlWidgetInstance);


// --------------------------------------------------------------------------
// layout
// --------------------------------------------------------------------------

   // create layout and apply to this
   QHBoxLayout *hbox = new QHBoxLayout;
   this->setLayout(hbox);

   // instantiate splitter and add to layout
   splitter = new QSplitter(this);
   splitter->setChildrenCollapsible(false);
   splitter->setOrientation(Qt::Horizontal);
   splitter->setHandleWidth(8); // 5 is default
   hbox->addWidget(splitter);

   // add widgets to splitter
   splitter->addWidget(mTablesWidgetInstance);
   splitter->addWidget(middlePanelFrame);
   splitter->addWidget(mChartsWidgetInstance);

   readSplitterSizes();
   connect(splitter, SIGNAL(splitterMoved(int, int)), this, SLOT(saveSplitterSizes(int, int)));
}


void mLogView::saveSplitterSizes(int pos, int index)
{
   //qDebug() << "void mChartsWidget::saveSplitterSizes()";

   // get a list of all splitter sizes
   QList <int> sizes = splitter->sizes();

   mbase.settings->beginWriteArray("logview_splitter");
   for (int i=0; i<splitter->count(); i++)
   {
      mbase.settings->setArrayIndex(i);
      mbase.settings->setValue("size", sizes[i]);
      //qDebug() << "w:" << i << " - " << sizes[i];
   }
   mbase.settings->endArray();
}

void mLogView::readSplitterSizes()
{
   //qDebug() << "void mChartsWidget::readSplitterSizes()";
   // get a list of all splitter sizes
   QList <int> sizes = splitter->sizes();

   int size = mbase.settings->beginReadArray("logview_splitter");
   for (int i = 0; i < size; ++i)
   {
      mbase.settings->setArrayIndex(i);
      sizes[i] = mbase.settings->value("size", 0).toInt();
      //qDebug() << "r:" << i << " - " << sizes[i];
   }
   mbase.settings->endArray();
   splitter->setSizes(sizes);
}




