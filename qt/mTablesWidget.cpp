#include "mTablesWidget.h"

mTablesWidget::mTablesWidget(QWidget *parent) : QWidget{parent}
{
   connect(&mbase, SIGNAL(mTablesWidgetUpdateSignal()),         this, SLOT(updateTables()));
   connect(&mbase, SIGNAL(mTablesWidgetUpdateColumnsSignal()),  this, SLOT(updateTablesHiddenColumns()));
   connect(&mbase, SIGNAL(mTablesWidgetFontChangeSignal()),     this, SLOT(doFontSizeChange()));
   connect(&mbase, SIGNAL(updateGlobalPositionSignal()),        this, SLOT(updateGlobalPosition()));

   mTablesWidgetSqlQueryModel = new QSqlQueryModel();

   mTablesWidgetTableView1 = new mTableView();
   mTablesWidgetTableView1->setModel(mTablesWidgetSqlQueryModel);
   mTablesWidgetTableView2 = new mTableView();
   mTablesWidgetTableView2->setModel(mTablesWidgetSqlQueryModel);

   // horizontal box layout
   QHBoxLayout *hbox = new QHBoxLayout;
   this->setLayout(hbox);
   hbox->addWidget(mTablesWidgetTableView1);
   hbox->addWidget(mTablesWidgetTableView2);
}


void mTablesWidget::updateGlobalPosition()
{
   if (!updateSelectedRow()) updateTables();    // if globalPosition is not in model
}

void mTablesWidget::updateTables()
{
   // qDebug() << "mTablesWidget::updateTables()";

   QString sql = "SELECT id, msg_type, sub_type, created, agt, frame_num AS frame, player AS p, client AS c, message";

   sql += " FROM logs WHERE created BETWEEN '";
   sql += mbase.globalPosition.toString("yyyy-MM-dd HH:mm:ss.z");
   sql += "' AND '";
   sql += mbase.sessionsDtEnd.toString("yyyy-MM-dd HH:mm:ss.z");
   sql += "' ";

   sql += " AND ";

   sql += mbase.mTablesWidgetControlWidgetSqlWhereTypeClause;

   sql += " ORDER BY id LIMIT ";

   sql += QString::number(mbase.mTablesWidgetSqlModelLimit);

   // qDebug() << "sql:" << sql;

   mTablesWidgetSqlQueryModel->setQuery(sql);
   int rowCount = mTablesWidgetSqlQueryModel->rowCount();

   if (rowCount == 0)
   {
      qDebug() << "Error in mTablesWidget::updateTables(): no rows for query\n" << sql;
      return;
   }

   doFontSizeChange();
   updateTablesHiddenColumns();
   showHideRowsBasedOnPlayer();
   updateSelectedRow();

   // get x axis ranges of sql model
   QModelIndex qmi = mTablesWidgetSqlQueryModel->index(0, 3);
   mbase.mTablesWidgetModelXAxisDateTimeStart = qmi.model()->data(qmi, Qt::DisplayRole).toDateTime();

   qmi = mTablesWidgetSqlQueryModel->index(rowCount-1, 3);
   mbase.mTablesWidgetModelXAxisDateTimeEnd = qmi.model()->data(qmi, Qt::DisplayRole).toDateTime();

   mbase.mTablesWidgetControlWidgetUpdateFunction();
}


void mTablesWidget::showHideRowsBasedOnPlayer()
{
   // show/hide rows in each TableView based on player
   for(int i=0; i<mTablesWidgetSqlQueryModel->rowCount(); i++)
   {
      // get player number from column 6
      QModelIndex qmi = mTablesWidgetSqlQueryModel->index(i, 6);
      QVariant v = qmi.model()->data(qmi, Qt::DisplayRole);
      int p = v.toInt();
      //qDebug() << p;
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

      if (p > 1)
      {
         mTablesWidgetTableView1->hideRow(i);
         mTablesWidgetTableView2->hideRow(i);
      }



   }
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
}


int mTablesWidget::updateSelectedRow()
{
   //qDebug() << "mTablesWidget::updateSelectedRow()";

   int foundt1 = -1;
   int foundt2 = -1;

   // cycle all rows in model
   for(int i=0; i<mTablesWidgetSqlQueryModel->rowCount(); i++)
   {
      if ((foundt1 == -1) || (foundt2 == -1))
      {
         // get player number from column 6
         QModelIndex qmi = mTablesWidgetSqlQueryModel->index(i, 6);
         QVariant v = qmi.model()->data(qmi, Qt::DisplayRole);
         int p = v.toInt();

         // get created from column 3
         qmi = mTablesWidgetSqlQueryModel->index(i, 3);
         v = qmi.model()->data(qmi, Qt::DisplayRole);
         QDateTime rdt = v.toDateTime();

         if (rdt >= mbase.globalPosition)
         {
            if ((p==0) && (foundt1 == -1) && (!mTablesWidgetTableView1->isRowHidden(i)))
            {
               mTablesWidgetTableView1->selectRow(i);
               foundt1 = i;
               // qDebug() << "found1_at:" << i;
            }

            if ((p==1) && (foundt2 == -1) && (!mTablesWidgetTableView2->isRowHidden(i)))
            {
               mTablesWidgetTableView2->selectRow(i);
               foundt2 = i;
               // qDebug() << "found2_at:" << i;
            }
         }
      }
   }

   if ((foundt1 < 10) || (foundt2 < 10))
   {
      // qDebug() << "mTablesWidget::updateSelectedRow()  --  not found in model -- reloading ";
      return 0;
   }

   mTablesWidgetTableView1->update();
   mTablesWidgetTableView2->update();

   return 1;
}


void mTablesWidget::doFontSizeChange(void)
{
   // qDebug() << "void mTablesWidget::doFontSizeChange(void)";

   // get new row height
   QFontMetrics fm(mbase.mTablesWidgetFont);
   int height = fm.height() - 2;

   mTablesWidgetTableView1->setFont(mbase.mTablesWidgetFont);
   mTablesWidgetTableView1->verticalHeader()->setMinimumSectionSize( height );
   mTablesWidgetTableView1->verticalHeader()->setMaximumSectionSize( height );
   mTablesWidgetTableView1->verticalHeader()->setDefaultSectionSize( height );
   mTablesWidgetTableView1->viewport()->update();

   mTablesWidgetTableView2->setFont(mbase.mTablesWidgetFont);
   mTablesWidgetTableView2->verticalHeader()->setMinimumSectionSize( height );
   mTablesWidgetTableView2->verticalHeader()->setMaximumSectionSize( height );
   mTablesWidgetTableView2->verticalHeader()->setDefaultSectionSize( height );
   mTablesWidgetTableView2->viewport()->update();
}















