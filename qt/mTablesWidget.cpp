#include "mTablesWidget.h"

mTablesWidget::mTablesWidget(QWidget *parent) : QWidget{parent}
{
   connect(&mbase, SIGNAL(mTablesWidgetReloadModelSignal()),         this, SLOT(reloadModel()));
   connect(&mbase, SIGNAL(updateGlobalPositionSignal()),             this, SLOT(updateGlobalPosition()));
   connect(&mbase, SIGNAL(mTablesWidgetUpdateUISignal()),            this, SLOT(updateUI()));

   mTablesWidgetSqlQueryModel = new QSqlQueryModel();

   // create topLayout and apply to this->
   QHBoxLayout *topLayout = new QHBoxLayout;
   this->setLayout(topLayout);
   topLayout->setContentsMargins(0, 0, 0, 0);

   // create groupBox and add to topLayout
   QGroupBox * topGroupBox = new QGroupBox("Tables", this);
   topLayout->addWidget(topGroupBox);
   topGroupBox->setObjectName("t1");
   QString style = "QGroupBox#t1 { border: 1px solid " + mbase.mTablesWidgetColor.name();
   style += "; margin: 0px; margin-top:4px; background-color:" + mbase.mTablesWidgetColor.lighter(195).name();
   style += "; } QGroupBox#t1::title { left: 10px; top: -8px; }";
   topGroupBox->setStyleSheet(style);

   // create layout for topGroupBox
   QHBoxLayout *topGroupBoxLayout = new QHBoxLayout;
   topGroupBox->setLayout(topGroupBoxLayout);
   topGroupBoxLayout->setContentsMargins(4, 8, 4, 4);


   mbase.loadFilters();

   for (int i=0; i<NUM_TABLES; i++)
   {
      mTablesWidgetTables[i] = new mTablesWidgetTable(this, i, mTablesWidgetSqlQueryModel);
      topGroupBoxLayout->addWidget(mTablesWidgetTables[i]);
   }
}


void mTablesWidget::updateGlobalPosition()
{
   // is new global position in model range
   if ((mbase.globalPosition >= mbase.mTablesWidgetModelXAxisDateTimeStart) && (mbase.globalPosition <= mbase.mTablesWidgetModelXAxisDateTimeEnd))
   {
      updateSelectedRow();
   }
   else reloadModel();
}

void mTablesWidget::reloadModel()
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
      // qDebug() << "Error in mTablesWidget::updateTables(): no rows for query\n" << sql;
      return;
   }


   updateUI();

   updateSelectedRow();

   // get x axis ranges of sql model
   QModelIndex qmi = mTablesWidgetSqlQueryModel->index(0, 3);
   mbase.mTablesWidgetModelXAxisDateTimeStart = qmi.model()->data(qmi, Qt::DisplayRole).toDateTime();

   qmi = mTablesWidgetSqlQueryModel->index(rowCount-1, 3);
   mbase.mTablesWidgetModelXAxisDateTimeEnd = qmi.model()->data(qmi, Qt::DisplayRole).toDateTime();

   mbase.mTablesWidgetControlWidgetUpdateFunction();
}




void mTablesWidget::updateSelectedRow()
{
   //qDebug() << "mTablesWidget::updateSelectedRow()";
   bool found[4] = { false };
   // cycle all rows in model
   for (int r=0; r<mTablesWidgetSqlQueryModel->rowCount(); r++)
   {
      // get created from column 3
      QModelIndex qmi = mTablesWidgetSqlQueryModel->index(r, 3);
      QDateTime rdt = qmi.model()->data(qmi, Qt::DisplayRole).toDateTime();

      if (rdt >= mbase.globalPosition)
         for (int t=0; t<4; t++)
            if ((!found[t]) && (!mTablesWidgetTables[t]->tableView->isRowHidden(r)))
            {
               mTablesWidgetTables[t]->tableView->selectRow(r);
               found[t] = true;
            }
   }
   for (int t=0; t<4; t++)
      mTablesWidgetTables[t]->tableView->update();
}












void mTablesWidget::updateUI()
{
   // cycle tables and update filters
   for (int t=0; t<NUM_TABLES; t++)
      mTablesWidgetTables[t]->tableFilter->updateCheckBoxesFromArray();


   // change row height based on font height
   int height = QFontMetrics(mbase.mTablesWidgetFont).height() - 2;
   for (int t=0; t<NUM_TABLES; t++)
      if (mTablesWidgetTables[t]->isVisible())
      {
         mTablesWidgetTables[t]->tableView->setFont(mbase.mTablesWidgetFont);
         mTablesWidgetTables[t]->tableView->verticalHeader()->setMinimumSectionSize( height );
         mTablesWidgetTables[t]->tableView->verticalHeader()->setMaximumSectionSize( height );
         mTablesWidgetTables[t]->tableView->verticalHeader()->setDefaultSectionSize( height );
         mTablesWidgetTables[t]->tableView->viewport()->update();
      }


   // show/hide columns
   for (int c=0; c<10; c++)
      if (mbase.col_types[c].valid)
         for (int t=0; t<NUM_TABLES; t++)
            if (mTablesWidgetTables[t]->isVisible())
            {
               if (mbase.col_types[c].shown == 0) mTablesWidgetTables[t]->tableView->hideColumn(c);
               else                               mTablesWidgetTables[t]->tableView->showColumn(c);
            }

   // update number of visible tables and show/hide filter controls
   for (int i=0; i<NUM_TABLES; i++)
   {
      if (i < mbase.numVisibleTables) mTablesWidgetTables[i]->frame->show();
      else mTablesWidgetTables[i]->frame->hide();

      // also do show/hide of filter controls
      if (mbase.showFilterControls)
      {
         mTablesWidgetTables[i]->tableFilter->pGb->show();
         mTablesWidgetTables[i]->tableFilter->cGb->show();
      }
      else
      {
         mTablesWidgetTables[i]->tableFilter->pGb->hide();
         mTablesWidgetTables[i]->tableFilter->cGb->hide();
      }
   }

   // show/hide rows in each TableView based on player and client filters
   for (int r=0; r<mTablesWidgetSqlQueryModel->rowCount(); r++)
   {
      // get player number from column 6
      QModelIndex qmi = mTablesWidgetSqlQueryModel->index(r, 6);
      int p = qmi.model()->data(qmi, Qt::DisplayRole).toInt();

      // get client number from column 7
      qmi = mTablesWidgetSqlQueryModel->index(r, 7);
      int c = qmi.model()->data(qmi, Qt::DisplayRole).toInt();
      //qDebug() << "p:" << p << " - c:" << c;

      // cycle tables
      for (int t=0; t<NUM_TABLES; t++)
         if (mTablesWidgetTables[t]->isVisible())
         {
            if ((mbase.mTablesWidgetTableFilter[t][0][p]) && (mbase.mTablesWidgetTableFilter[t][1][c]))
               mTablesWidgetTables[t]->tableView->showRow(r);
            else
               mTablesWidgetTables[t]->tableView->hideRow(r);
         }
   }
}

