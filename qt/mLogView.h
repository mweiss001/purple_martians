#ifndef MLOGVIEW_H
#define MLOGVIEW_H

#include <QWidget>

#include <QTableView>
#include <QSqlQueryModel>
#include <QHeaderView>

#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QDateTime>

#include "mTableRowSelectWidget.h"
#include "mTableColumnSelectWidget.h"
#include "mSessionsWidget.h"
#include "mChartsWidget.h"
#include "mChartsWidgetControlWidget.h"


#include "mTablesWidget.h"
#include "mTablesWidgetControlWidget.h"


#include "m_base.h"

#include "mSessionsTimelineWidget.h"

#include "mCurrentSessionTimelineWidget.h"




class mLogView : public QWidget
{
   Q_OBJECT

   public:
      explicit mLogView(QWidget *parent = nullptr);

   public slots:

   signals:

   private slots:

      void saveSplitterSizes(int, int);
      void readSplitterSizes();

   private:
      mTableRowSelectWidget * mTableRowSelectWidgetInstance;
      mTableColumnSelectWidget * mTableColumnSelectWidgetInstance;

      mSessionsWidget * mSessionsWidgetInstance;
      mChartsWidget * mChartsWidgetInstance;
      mChartsWidgetControlWidget * mChartsWidgetControlWidgetInstance;

      QImage rectangle(qreal imageSize, const QColor &color);
      QImage circle(qreal imageSize, const QColor &color);

      QSplitter * splitter;


};

#endif // MLOGVIEW_H
