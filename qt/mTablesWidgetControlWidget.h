#ifndef MTABLESWIDGETCONTROLWIDGET_H
#define MTABLESWIDGETCONTROLWIDGET_H

#include <QWidget>
#include <QBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSpinBox>



#include "mTableRowSelectWidget.h"
#include "mTableColumnSelectWidget.h"

class mTablesWidgetControlWidget : public QWidget
{
   Q_OBJECT
   public:
      explicit mTablesWidgetControlWidget(QWidget *parent = nullptr);
      int HSIZE = 600;
      int VSIZE = 600;
      QSize minimumSizeHint() const {      return QSize(HSIZE, VSIZE);   }
      QSize minimumSize () const    {      return QSize(HSIZE, VSIZE);   }

   public slots:
   signals:
   private slots:
      void fontSizeChanged(int);
      void sqlLimitChanged(int);

   private:
      mTableRowSelectWidget * mTableRowSelectWidgetInstance;
      mTableColumnSelectWidget * mTableColumnSelectWidgetInstance;
};

#endif // MTABLESWIDGETCONTROLWIDGET_H
