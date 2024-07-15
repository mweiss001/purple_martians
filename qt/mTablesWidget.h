#ifndef MTABLESWIDGET_H
#define MTABLESWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QBoxLayout>
#include "m_base.h"
#include "mdelegate.h"

class mTablesWidget : public QWidget
{
   Q_OBJECT
   public:
      explicit mTablesWidget(QWidget *parent = nullptr);

   signals:

   private slots:
      void on_tb1_clicked(QModelIndex indx);
      void on_tb2_clicked(QModelIndex indx);
      void updateTables();
      void updateTablesHiddenColumns();
      void doFontSizeChange();

   private:
      QSqlQueryModel * mTablesWidgetSqlQueryModel;
      QTableView * mTablesWidgetTableView1;
      QTableView * mTablesWidgetTableView2;
};

#endif // MTABLESWIDGET_H
