#ifndef MTABLESWIDGETCONTROLWIDGET_H
#define MTABLESWIDGETCONTROLWIDGET_H

#include <QWidget>
#include <QBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>



#include "mTableRowSelectWidget.h"
#include "mTableColumnSelectWidget.h"

class mTablesWidgetControlWidget : public QWidget
{
   Q_OBJECT
   public:
      explicit mTablesWidgetControlWidget(QWidget *parent = nullptr);
      int HSIZE = 336;
      int VSIZE = 464;
      QSize minimumSizeHint() const {      return QSize(HSIZE, VSIZE);   }
      QSize minimumSize () const    {      return QSize(HSIZE, VSIZE);   }


   private slots:
      void fontSizeChanged(int);
      void sqlLimitChanged(int);

      void update();



   private:
      mTableRowSelectWidget * mTableRowSelectWidgetInstance;
      mTableColumnSelectWidget * mTableColumnSelectWidgetInstance;
      QLabel * xAxisModel;

};

#endif // MTABLESWIDGETCONTROLWIDGET_H
