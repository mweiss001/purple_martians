#ifndef MTABLESWIDGET_H
#define MTABLESWIDGET_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QBoxLayout>
#include "m_base.h"

#include "mTableView.h"

class mTablesWidget : public QWidget
{
   Q_OBJECT
   public:
      explicit mTablesWidget(QWidget *parent = nullptr);
      int updateSelectedRow();
      void showHideRowsBasedOnPlayer();

   private slots:
      void updateTables();
      void updateTablesHiddenColumns();
      void doFontSizeChange();
      void updateGlobalPosition();

   private:
      QSqlQueryModel * mTablesWidgetSqlQueryModel;
      mTableView * mTablesWidgetTableView1;
      mTableView * mTablesWidgetTableView2;
};

#endif // MTABLESWIDGET_H
