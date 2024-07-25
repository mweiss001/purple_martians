#ifndef MTABLESWIDGET_H
#define MTABLESWIDGET_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QBoxLayout>


#include <QCheckBox>


#include "m_base.h"


#include "mTablesWidgetTableFilter.h"

#include "mTablesWidgetTable.h"


class mTablesWidget : public QWidget
{
   Q_OBJECT
   public:
      explicit mTablesWidget(QWidget *parent = nullptr);
      mTablesWidgetTable * mTablesWidgetTables[NUM_TABLES];

   private slots:
      void reloadModel();
      void updateGlobalPosition();
      void updateUI();


   private:
      QSqlQueryModel * mTablesWidgetSqlQueryModel;
      void updateSelectedRow();
};

#endif // MTABLESWIDGET_H
