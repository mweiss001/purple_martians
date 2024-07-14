#include "mLogView.h"

mLogView::mLogView(QWidget *parent)
    : QWidget{parent}
{

/*

   // sql widget
   msqw = new MSQLWidget(this);

   model = new QSqlQueryModel();



   // pm_log TableView
   tableView1 = new QTableView(this);
   tableView1->setItemDelegate(new mDelegate);
   tableView1->setModel(model);
   tableView1->horizontalHeader()->setStretchLastSection(true);
   tableView1->verticalHeader()->setVisible(false);
   tableView1->verticalHeader()->setSectionResizeMode( QHeaderView::Fixed );
   tableView1->setSelectionBehavior(QAbstractItemView::SelectRows);

   tableView2 = new QTableView(this);
   tableView2->setItemDelegate(new mDelegate);
   tableView2->setModel(model);
   tableView2->horizontalHeader()->setStretchLastSection(true);
   tableView2->verticalHeader()->setVisible(false);
   tableView2->verticalHeader()->setSectionResizeMode( QHeaderView::Fixed );
   tableView2->setSelectionBehavior(QAbstractItemView::SelectRows);


   setStyleSheet("QTableView {selection-background-color: plum;}");

   connect(tableView1, SIGNAL(clicked(QModelIndex)), this, SLOT (on_tb1_clicked(QModelIndex)));
   connect(tableView2, SIGNAL(clicked(QModelIndex)), this, SLOT (on_tb2_clicked(QModelIndex)));

   // message line type selector
   mlts = new MLogMessageTypeSelector(this);

   // column select widget
   mpms = new MPMSelect(this);

   // sessions widget
   mpss = new MPMSessions(this);

   // sql execute button
   pb_exec =  new QPushButton("Execute", this);
   connect(pb_exec, SIGNAL (clicked()), this, SLOT (pb_exec_clicked()));

   // auto exec check box
   cb_autoexec = new QCheckBox("Auto Execute", this);
   cb_autoexec->setChecked(true);

   // number of rows label
   lb_num_rows = new QLabel("num rows");

   connect(&mbase, SIGNAL (sql_changed()), this, SLOT (do_sql_changed() ));

   connect(mpms, SIGNAL (sel_changed()), this, SLOT (update_table_hidden_columns() ));

   // font size spin box
   sb_font_size =  new QSpinBox();
   sb_font_size->setRange(4, 20);
   sb_font_size->setValue(mbase.font_size);
   connect(sb_font_size, SIGNAL (valueChanged(int)), this, SLOT (font_size_changed(int)));

   // horizontal box layout
   QHBoxLayout *hbox = new QHBoxLayout;
   this->setLayout(hbox);

   // put table view on lhs
   hbox->addWidget(tableView1);
   hbox->addWidget(tableView2);


   // make v box for rhs
   QVBoxLayout *vbox = new QVBoxLayout;
   hbox->addLayout(vbox);

   // make hbox for mlts and mldr
   QHBoxLayout *hbox1 = new QHBoxLayout;
   vbox->addLayout(hbox1);

   hbox1->addWidget(mlts);
   hbox1->addWidget(mpms);


   // make hbox for pb_exec and cb_autoexec
   QHBoxLayout *hbox2 = new QHBoxLayout;
   vbox->addLayout(hbox2);

   hbox2->addWidget(pb_exec);
   hbox2->addWidget(cb_autoexec);

   vbox->addWidget(lb_num_rows);

   vbox->addWidget(sb_font_size);



   vbox->addWidget(mpss);


   // charts widget
   mchw = new mChartsWidget(this);
   vbox->addWidget(mchw);


*/




   // minimalistic for testing...

   QVBoxLayout *vbox = new QVBoxLayout;
   this->setLayout(vbox);






   // sessions widget
   mSessionsWidgetInstance = new mSessionsWidget(this);

   connect(&mbase, SIGNAL (sql_changed()), this, SLOT (do_sql_changed() ));




   mChartsWidgetControlWidgetInstance = new mChartsWidgetControlWidget(this);




   // charts widget
   mChartsWidgetInstance = new mChartsWidget(this);


   QHBoxLayout *hbox = new QHBoxLayout;

   hbox->addWidget(mSessionsWidgetInstance);

   hbox->addWidget(mChartsWidgetControlWidgetInstance);


   vbox->addLayout(hbox);

   vbox->addWidget(mChartsWidgetInstance);






//   vbox->addWidget(msqw);
//   pb_exec_clicked();
}

void mLogView::pb_exec_clicked()
{
   //qDebug() << "pb_exec_clicked()";

   mbase.update_sql();
   mChartsWidgetInstance->update();
   update_table();
}

void mLogView::do_sql_changed()
{
   //qDebug() << "do_sql_changed()";
//   msqw->update();
//   if (cb_autoexec->checkState()) update_table();
   update_table();
}

void mLogView::update_table()
{
   //qDebug() << "update_table()";

   mChartsWidgetInstance->update();

   /*
   model->setQuery(mbase.pml_sql_all);

   for (int i=0; i<10; i++)
   {
      if ((mbase.col_types[i].valid) && (mbase.col_types[i].shown == 0)) tableView1->hideColumn(i);
      if ((mbase.col_types[i].valid) && (mbase.col_types[i].shown == 0)) tableView2->hideColumn(i);
   }

   QString txt = QString("rows:%1").arg(QString::number(model->rowCount()));
   lb_num_rows->setText(txt);

   update_table_hidden_columns();

   font_size_changed(mbase.font_size);


   tableView1->resizeColumnsToContents();
   tableView2->resizeColumnsToContents();
*/

}

void mLogView::font_size_changed(int size)
{
   mbase.font_size = size;
   mbase.set_font();

   tableView1->setFont(mbase.font);
   tableView2->setFont(mbase.font);

   // get new row height
   QFontMetrics fm(mbase.font);
   int height = fm.height() - 2;

   tableView1->verticalHeader()->setMinimumSectionSize( height );
   tableView1->verticalHeader()->setMaximumSectionSize( height );
   tableView1->verticalHeader()->setDefaultSectionSize( height );
   tableView1->resizeColumnsToContents();
   tableView1->viewport()->update();

   tableView2->verticalHeader()->setMinimumSectionSize( height );
   tableView2->verticalHeader()->setMaximumSectionSize( height );
   tableView2->verticalHeader()->setDefaultSectionSize( height );
   tableView2->resizeColumnsToContents();
   tableView2->viewport()->update();
}





void mLogView::update_table_hidden_columns()
{
   for (int i=0; i<10; i++)
      if (mbase.col_types[i].valid)
      {
         if (mbase.col_types[i].shown == 0)
         {
            tableView1->hideColumn(i);
            tableView2->hideColumn(i);
         }
         else
         {
            tableView1->showColumn(i);
            tableView2->showColumn(i);
         }
      }

   // and rows
   for(int i=0; i<model->rowCount(); i++)
   {
      // get player number from column 6
      QModelIndex qmi = model->index(i, 6);
      QVariant v = qmi.model()->data(qmi, Qt::DisplayRole);
      int p = v.toInt();

//      qDebug() << p;

      if (p == 0)
      {
         tableView1->showRow(i);
         tableView2->hideRow(i);
      }
      if (p == 1)
      {
         tableView1->hideRow(i);
         tableView2->showRow(i);
      }
   }
}


void mLogView::on_tb1_clicked(QModelIndex indx)
{
   QModelIndexList selection = tableView1->selectionModel()->selectedRows();            // list of selected items
   QModelIndex index = selection.at(0);                                                 // only use first row in selection

   int row = index.row(); // starting row
   int wd = 100;          // max num of rows to look through
   while ((tableView2->isRowHidden(++row)) && (--wd > 0));

   if (!(tableView2->isRowHidden(row))) // did we find a non hidden row?
   {
      //   qDebug() << "first unhidden row in table2" << row;
      tableView2->selectRow(row);
      tableView2->scrollTo(model->index(row, 0));
   }
 //  else qDebug() << "no unhidden rows in table2";
}


void mLogView::on_tb2_clicked(QModelIndex indx)
{
   QModelIndexList selection = tableView2->selectionModel()->selectedRows();            // list of selected items
   QModelIndex index = selection.at(0);                                                 // only use first row in selection

   int row = index.row(); // starting row
   int wd = 100;          // max num of rows to look through
   while ((tableView1->isRowHidden(++row)) && (--wd > 0));

   if (!(tableView1->isRowHidden(row))) // did we find a non hidden row?
   {
      //   qDebug() << "first unhidden row in table1" << row;
      tableView1->selectRow(row);
      tableView1->scrollTo(model->index(row, 0));
   }
   //  else qDebug() << "no unhidden rows in table1";
}

/*

   QDateTime dt = model->data(index.siblingAtColumn(3), Qt::DisplayRole).toDateTime();  // column 3 'created' DateTime
   int frame = model->data(index.siblingAtColumn(5), Qt::DisplayRole).toInt();          // column 5 'frame' int


  */


//   qDebug() << frame << dt;



   // experiment trying to set selection in table2


   /*


 there could be a very elegant way of doing this...

find the same index that is visible in the other table...they share the model.....

*/

   // see if I can find the siblings of the item

//   int row = index.row();
//   qDebug() << row;

/*
   QModelIndex qmi2 = index.sibling(row+1, 3);
   QDateTime dt2 = model->data(qmi2, Qt::DisplayRole).toDateTime();  // column 3 'created' DateTime

   qDebug() << dt2;


   // is this index visible in table 2

   if (tableView2->isRowHidden(row+1)) qDebug() << "Hidden";


   // this all seems to work, so I gues what I want to do is find the next unhidden row in table 2
  */


/*

   // get row
   int row = index.row();
   qDebug() << "row" << row;

   for (row = index.row(); row<index.row()+100; row++)
   {
      if (!(tableView2->isRowHidden(row))) break;
   }

   if (!(tableView2->isRowHidden(row)))
   {
      qDebug() << "first unhidden row in table2" << row;
   }
   else qDebug() << "no unhidden rows in table2";

   tableView2->selectRow(row);




   int row = index.row(); // starting row
   int wd = 100;          // max num of rows to look through
   while ((tableView2->isRowHidden(++row)) && (--wd > 0));

   if (!(tableView2->isRowHidden(row))) // did we find a non hidden row?
   {
      qDebug() << "first unhidden row in table2" << row;
      tableView2->selectRow(row);
      tableView2->scrollTo(model->index(row, 0));
   }
   else qDebug() << "no unhidden rows in table2";





   QItemSelectionModel *selectionModel = tableView2->selectionModel();


   QModelIndex topLeft;
   QModelIndex bottomRight;

   topLeft = model->index(0, 2, QModelIndex());
   bottomRight = model->index(50, 3, QModelIndex());
   QItemSelection selection2(topLeft, bottomRight);

   selectionModel->select(selection2, QItemSelectionModel::Select);
*/


/*
   QItemSelectionModel *selectionModel = tableView2->selectionModel();


   QModelIndex topLeft;
   QModelIndex bottomRight;

   topLeft = model->index(0, 0, QModelIndex());
   bottomRight = model->index(2, 2, QModelIndex());

   selectionModel->select(selection, QItemSelectionModel::Select);

*/


   /*


   QItemSelectionModel *selectionModel = table->selectionModel();

   The table view's default selection model is retrieved for later use. We do not modify any items in the model, but instead select a few items that the view will display at the top-left of the table. To do this, we need to retrieve the model indexes corresponding to the top-left and bottom-right items in the region to be selected:

   QModelIndex topLeft;
   QModelIndex bottomRight;

   topLeft = model->index(0, 0, QModelIndex());
   bottomRight = model->index(5, 2, QModelIndex());

   To select these items in the model, and see the corresponding change in the table view, we need to construct a selection object then apply it to the selection model:

                                                                                                                                                                          QItemSelection selection(topLeft, bottomRight);
   selectionModel->select(selection, QItemSelectionModel::Select);



   private slots:
//      void on_tb1_changed(const QItemSelection & sel, const QItemSelection & dsel);
//       void on_tb2_changed(const QItemSelection & sel, const QItemSelection & dsel);


    connect(tableView1->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this,
            SLOT    (on_tb1_changed(const QItemSelection &, const QItemSelection &)));
   connect(tableView2->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this,
           SLOT    (on_tb2_changed(const QItemSelection &, const QItemSelection &)));






//void MPMLogView::on_tb2_changed(const QItemSelection & sel, const QItemSelection & dsel)
void MPMLogView::on_tb2_clicked(QModelIndex indx)
{
   QModelIndexList selection = tableView2->selectionModel()->selectedRows();            // list of selected items
   QModelIndex index = selection.at(0);                                                 // only use first row in selection

   int row = index.row(); // starting row
   int wd = 100;          // max num of rows to look through
   while ((tableView1->isRowHidden(++row)) && (--wd > 0));

   if (!(tableView1->isRowHidden(row))) // did we find a non hidden row?
   {
      //   qDebug() << "first unhidden row in table1" << row;

      // tableView1->blockSignals(true);
      // tableView1->selectionModel()->blockSignals(true);


      tableView1->selectRow(row);
      tableView1->scrollTo(model->index(row, 0));

      // tableView1->blockSignals(false);
      // tableView1->selectionModel()->blockSignals(false);


   }
   //  else qDebug() << "no unhidden rows in table1";
}


*/



















