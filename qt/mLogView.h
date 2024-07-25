#ifndef MLOGVIEW_H
#define MLOGVIEW_H

#include <QWidget>

#include "mChartsWidget.h"
#include "mChartsWidgetControlWidget.h"

#include "mTablesWidget.h"
#include "mTablesWidgetControlWidget.h"

#include "m_base.h"

class mLogView : public QWidget
{
   Q_OBJECT

   public:
      explicit mLogView(QWidget *parent = nullptr);

   private slots:
      void saveSplitterSizes(int, int);
      void readSplitterSizes();

   private:
      QSplitter * splitter;
};

#endif // MLOGVIEW_H
