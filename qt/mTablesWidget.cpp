#include "mTablesWidget.h"

mTablesWidget::mTablesWidget(QWidget *parent) : QWidget{parent}
{
   connect(&mbase, SIGNAL(mTablesWidgetUpdateSignal()),         this, SLOT(updateTables()));
   connect(&mbase, SIGNAL(mTablesWidgetUpdateColumnsSignal()),  this, SLOT(updateTablesHiddenColumns()));
   connect(&mbase, SIGNAL(mTablesWidgetFontChangeSignal()),     this, SLOT(doFontSizeChange()));




   mTablesWidgetSqlQueryModel = new QSqlQueryModel();

   mTablesWidgetTableView1 = new QTableView(this);
   mTablesWidgetTableView1->setItemDelegate(new mDelegate);
   mTablesWidgetTableView1->setModel(mTablesWidgetSqlQueryModel);
   mTablesWidgetTableView1->horizontalHeader()->setStretchLastSection(true);
   mTablesWidgetTableView1->verticalHeader()->setVisible(false);
   mTablesWidgetTableView1->verticalHeader()->setSectionResizeMode( QHeaderView::Fixed );
   mTablesWidgetTableView1->setSelectionBehavior(QAbstractItemView::SelectRows);

   mTablesWidgetTableView2 = new QTableView(this);
   mTablesWidgetTableView2->setItemDelegate(new mDelegate);
   mTablesWidgetTableView2->setModel(mTablesWidgetSqlQueryModel);
   mTablesWidgetTableView2->horizontalHeader()->setStretchLastSection(true);
   mTablesWidgetTableView2->verticalHeader()->setVisible(false);
   mTablesWidgetTableView2->verticalHeader()->setSectionResizeMode( QHeaderView::Fixed );
   mTablesWidgetTableView2->setSelectionBehavior(QAbstractItemView::SelectRows);

   setStyleSheet("QTableView {selection-background-color: plum;}");

   connect(mTablesWidgetTableView1, SIGNAL(clicked(QModelIndex)), this, SLOT (on_tb1_clicked(QModelIndex)));
   connect(mTablesWidgetTableView2, SIGNAL(clicked(QModelIndex)), this, SLOT (on_tb2_clicked(QModelIndex)));

   // horizontal box layout
   QHBoxLayout *hbox = new QHBoxLayout;
   this->setLayout(hbox);
   hbox->addWidget(mTablesWidgetTableView1);
   hbox->addWidget(mTablesWidgetTableView2);
}


void mTablesWidget::updateTables()
{
   //qDebug() << "mTablesWidget::updateTables()";

   QString sql =  "SELECT id, msg_type, sub_type, created, agt, frame_num, player, client, message";
   sql += " FROM logs WHERE created " + mbase.sessionsSqlWhereDateClause;
   sql += " AND " + mbase.mTablesWidgetControlWidgetSqlWhereTypeClause;
   sql += " ORDER BY id LIMIT ";
   sql += QString::number(mbase.mTablesWidgetSqlModelLimit);
   //qDebug() << "sql: " << sql;

   mTablesWidgetSqlQueryModel->setQuery(sql);

   doFontSizeChange();
   updateTablesHiddenColumns();
}



void mTablesWidget::updateTablesHiddenColumns()
{
   for (int i=0; i<10; i++)
      if (mbase.col_types[i].valid)
      {
         if (mbase.col_types[i].shown == 0)
         {
            mTablesWidgetTableView1->hideColumn(i);
            mTablesWidgetTableView2->hideColumn(i);
         }
         else
         {
            mTablesWidgetTableView1->showColumn(i);
            mTablesWidgetTableView2->showColumn(i);
         }
      }

   // and rows
   for(int i=0; i<mTablesWidgetSqlQueryModel->rowCount(); i++)
   {
      // get player number from column 6
      QModelIndex qmi = mTablesWidgetSqlQueryModel->index(i, 6);
      QVariant v = qmi.model()->data(qmi, Qt::DisplayRole);
      int p = v.toInt();

      //      qDebug() << p;

      if (p == 0)
      {
         mTablesWidgetTableView1->showRow(i);
         mTablesWidgetTableView2->hideRow(i);
      }
      if (p == 1)
      {
         mTablesWidgetTableView1->hideRow(i);
         mTablesWidgetTableView2->showRow(i);
      }
   }
   mTablesWidgetTableView1->resizeColumnsToContents();
   mTablesWidgetTableView2->resizeColumnsToContents();
}


void mTablesWidget::on_tb1_clicked(QModelIndex indx)
{
   QModelIndexList selection = mTablesWidgetTableView1->selectionModel()->selectedRows();  // list of selected items
   QModelIndex index = selection.at(0);                                                    // only use first row in selection

   int row = index.row(); // starting row
   int wd = 100;          // max num of rows to look through
   while ((mTablesWidgetTableView2->isRowHidden(++row)) && (--wd > 0));

   if (!(mTablesWidgetTableView2->isRowHidden(row))) // did we find a non hidden row?
   {
      //   qDebug() << "first unhidden row in table2" << row;
      mTablesWidgetTableView2->selectRow(row);
      mTablesWidgetTableView2->scrollTo(mTablesWidgetSqlQueryModel->index(row, 0));
   }
   //  else qDebug() << "no unhidden rows in table2";
}

void mTablesWidget::on_tb2_clicked(QModelIndex indx)
{
   QModelIndexList selection = mTablesWidgetTableView2->selectionModel()->selectedRows();  // list of selected items
   QModelIndex index = selection.at(0);                                                    // only use first row in selection

   int row = index.row(); // starting row
   int wd = 100;          // max num of rows to look through
   while ((mTablesWidgetTableView1->isRowHidden(++row)) && (--wd > 0));

   if (!(mTablesWidgetTableView1->isRowHidden(row))) // did we find a non hidden row?
   {
      //   qDebug() << "first unhidden row in table1" << row;
      mTablesWidgetTableView1->selectRow(row);
      mTablesWidgetTableView1->scrollTo(mTablesWidgetSqlQueryModel->index(row, 0));
   }
   //  else qDebug() << "no unhidden rows in table1";
}

void mTablesWidget::doFontSizeChange(void)
{
   mTablesWidgetTableView1->setFont(mbase.mTablesWidgetFont);
   mTablesWidgetTableView2->setFont(mbase.mTablesWidgetFont);

   // get new row height
   QFontMetrics fm(mbase.mTablesWidgetFont);
   int height = fm.height() - 2;

   mTablesWidgetTableView1->verticalHeader()->setMinimumSectionSize( height );
   mTablesWidgetTableView1->verticalHeader()->setMaximumSectionSize( height );
   mTablesWidgetTableView1->verticalHeader()->setDefaultSectionSize( height );
   mTablesWidgetTableView1->resizeColumnsToContents();
   mTablesWidgetTableView1->viewport()->update();

   mTablesWidgetTableView2->verticalHeader()->setMinimumSectionSize( height );
   mTablesWidgetTableView2->verticalHeader()->setMaximumSectionSize( height );
   mTablesWidgetTableView2->verticalHeader()->setDefaultSectionSize( height );
   mTablesWidgetTableView2->resizeColumnsToContents();
   mTablesWidgetTableView2->viewport()->update();
}

