#ifndef MTABLESWIDGETCONTROLWIDGET_H
#define MTABLESWIDGETCONTROLWIDGET_H

#include <QWidget>
#include <QBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>


#include "mTableRowSelectWidget.h"
#include "mTableColumnSelectWidget.h"

class mTablesWidgetControlWidget : public QWidget
{
   Q_OBJECT
   public:
      explicit mTablesWidgetControlWidget(QWidget *parent = nullptr);

   private slots:
      void showHideFilterControlsToggled(bool);
      void fontSizeChanged(int);
      void sqlLimitChanged(int);
      void numTablesChanged(int);

      void updateUI();

  private:
      mTableRowSelectWidget * mTableRowSelectWidgetInstance;
      mTableColumnSelectWidget * mTableColumnSelectWidgetInstance;
      QLabel * xAxisModel;


      QSpinBox * numTablesSpinBox;
      QPushButton * showHideFilterControls;
      QSpinBox * fontSizeSpinBox;
      QSpinBox * sqlLimitSpinBox;

};

#endif // MTABLESWIDGETCONTROLWIDGET_H
