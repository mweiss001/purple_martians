#include "mLogView.h"

mLogView::mLogView(QWidget *parent) : QWidget{parent}
{

   // horizontal box layout
   QHBoxLayout *hbox = new QHBoxLayout;
   this->setLayout(hbox);

   // make a splitter and add it to hbox
   splitter = new QSplitter(this);
   splitter->setChildrenCollapsible(false);
   splitter->setOrientation(Qt::Horizontal);
   hbox->addWidget(splitter);


   // ---------------------------------------------------------------------
   // mTablesWidget
   // ---------------------------------------------------------------------
   mTablesWidget * mTablesWidgetInstance = new mTablesWidget(this);
   hbox->addWidget(mTablesWidgetInstance);

   // add to splitter
   splitter->addWidget(mTablesWidgetInstance);

   // qDebug() << splitter->handleWidth();
   splitter->setHandleWidth(8); // 5 is default

   // make a frame middle panel

   QFrame * frame = new QFrame();
   frame->setLineWidth(0);
   frame->setMidLineWidth(0);
   frame->setFrameStyle(QFrame::Panel);
   frame->setMaximumWidth(640);

   // add frame to splitter
   splitter->addWidget(frame);

   // make vbox for frame
   QVBoxLayout *vbox = new QVBoxLayout;

   // add vbox layout to frame
   frame->setLayout(vbox);

   // ---------------------------------------------------------------------
   // mTablesWidgetControlWidget
   // ---------------------------------------------------------------------
   mTablesWidgetControlWidget * mTablesWidgetControlWidgetInstance = new mTablesWidgetControlWidget(this);
   vbox->addWidget(mTablesWidgetControlWidgetInstance);

   // ---------------------------------------------------
   // sessions widget
   // ---------------------------------------------------
   mSessionsWidgetInstance = new mSessionsWidget(this);
   vbox->addWidget(mSessionsWidgetInstance);

   // ---------------------------------------------------
   // charts controls widget
   // ---------------------------------------------------
   mChartsWidgetControlWidgetInstance = new mChartsWidgetControlWidget(this);

   // put it in its own hbox so I can push it over to the right
   QHBoxLayout *hbox3 = new QHBoxLayout;
   vbox->addLayout(hbox3);
   hbox3->addStretch();
   hbox3->addWidget(mChartsWidgetControlWidgetInstance);



   // ---------------------------------------------------
   // charts widget
   // ---------------------------------------------------
   mChartsWidgetInstance = new mChartsWidget(this);

   // add to splitter
   splitter->addWidget(mChartsWidgetInstance);

   readSplitterSizes();
   connect(splitter, SIGNAL(splitterMoved(int, int)), this, SLOT(saveSplitterSizes(int, int)));
}

void mLogView::saveSplitterSizes(int pos, int index)
{
   //qDebug() << "void mChartsWidget::saveSplitterSizes()";

   // get a list of all splitter sizes
   QList <int> sizes = splitter->sizes();

   mbase.settings->beginWriteArray("logview_splitter");
   for (int i=0; i<splitter->count(); i++)
   {
      mbase.settings->setArrayIndex(i);
      mbase.settings->setValue("size", sizes[i]);
      //qDebug() << "w:" << i << " - " << sizes[i];
   }
   mbase.settings->endArray();
}

void mLogView::readSplitterSizes()
{
   //qDebug() << "void mChartsWidget::readSplitterSizes()";
   // get a list of all splitter sizes
   QList <int> sizes = splitter->sizes();

   int size = mbase.settings->beginReadArray("logview_splitter");
   for (int i = 0; i < size; ++i)
   {
      mbase.settings->setArrayIndex(i);
      sizes[i] = mbase.settings->value("size", 0).toInt();
      //qDebug() << "r:" << i << " - " << sizes[i];
   }
   mbase.settings->endArray();
   splitter->setSizes(sizes);
}
















































































/*


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

*/

















































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



















