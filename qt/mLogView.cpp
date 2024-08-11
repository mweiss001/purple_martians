#include "mLogView.h"

mLogView::mLogView(QWidget *parent) : QWidget{parent}
{

// ---------------------------------------------------
// first create all the things I will show here
// ---------------------------------------------------
   mTablesWidget * mTablesWidgetInstance = new mTablesWidget();
   mChartsWidget * mChartsWidgetInstance = new mChartsWidget();
   mTablesWidgetControlWidget * mTablesWidgetControlWidgetInstance = new mTablesWidgetControlWidget();
   mChartsWidgetControlWidget * mChartsWidgetControlWidgetInstance = new mChartsWidgetControlWidget();

   // ---------------------------------------------------------
   // middle panel frame
   // ---------------------------------------------------------
   QFrame * middlePanelFrame = new QFrame();
   middlePanelFrame->setLineWidth(0);
   middlePanelFrame->setMidLineWidth(0);
   middlePanelFrame->setFrameStyle(QFrame::Panel);
   middlePanelFrame->setMaximumWidth(360);

   // create and apply layout for middle panel frame
   QVBoxLayout *middlePanelFrameLayout = new QVBoxLayout;
   middlePanelFrameLayout->setContentsMargins(0, 0, 0, 0);
   middlePanelFrame->setLayout(middlePanelFrameLayout);

   // make hbox layout for mTablesWidgetControlWidget so I can left align it
   QHBoxLayout *mTablesWidgetControlWidgetLayout = new QHBoxLayout;
   middlePanelFrameLayout->addLayout(mTablesWidgetControlWidgetLayout);
   mTablesWidgetControlWidgetLayout->addWidget(mTablesWidgetControlWidgetInstance);
   mTablesWidgetControlWidgetLayout->addStretch();

   middlePanelFrameLayout->addStretch();

   // make hbox layout for mChartsWidgetControlWidget so I can right align it
   QHBoxLayout *mChartsWidgetControlWidgetLayout = new QHBoxLayout;
   middlePanelFrameLayout->addLayout(mChartsWidgetControlWidgetLayout);
   mChartsWidgetControlWidgetLayout->addStretch();
   mChartsWidgetControlWidgetLayout->addWidget(mChartsWidgetControlWidgetInstance);


// --------------------------------------------------------------------------
// layout
// --------------------------------------------------------------------------

   // create layout and apply to this
   QHBoxLayout *hbox = new QHBoxLayout;
   hbox->setContentsMargins(0, 0, 0, 0);
   this->setLayout(hbox);

   // instantiate splitter and add to layout
   splitter = new QSplitter();
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




