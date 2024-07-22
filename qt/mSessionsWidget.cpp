#include "mSessionsWidget.h"

mSessionsWidget::mSessionsWidget(QWidget *parent)
    : QWidget{parent}
{

   QVBoxLayout *vbox = new QVBoxLayout;

   mpss_gb = new QGroupBox("Sessions", this);
   mpss_gb->setGeometry(10, 10, VSIZE-20, HSIZE-20);
   mpss_gb->setLayout(vbox);

//   mbase.sessionsModel = new QSqlQueryModel();
//   mbase.sessionsModel->setQuery("SELECT dt_start, dt_end, duration AS dur, hostname AS host, endreason FROM sessions ORDER by dt_start");

   sessionsTableView = new QTableView(this);
   sessionsTableView->setModel(mbase.sessionsModel);
   sessionsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
   sessionsTableView->setItemDelegate(new mDelegate2);
   sessionsTableView->horizontalHeader()->setStretchLastSection(true);

   connect(sessionsTableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this,
                                                SLOT    (on_stv_changed(const QItemSelection &, const QItemSelection &)));

   vbox->addWidget(sessionsTableView);

   sessionsTableView->resizeColumnsToContents();
   sessionsTableView->viewport()->update();


   mpss_lb_start = new QLabel;
   mpss_lb_end = new QLabel;
   mpss_lb_range = new QLabel;

   mpss_gb_start = new QGroupBox("Start");
   mpss_gb_end = new QGroupBox("End");
   mpss_gb_range = new QGroupBox("Range");


   QHBoxLayout *hbox = new QHBoxLayout;
   vbox->addLayout(hbox);

   hbox->addWidget(mpss_gb_start);
   hbox->addWidget(mpss_gb_end);
   hbox->addWidget(mpss_gb_range);

   QVBoxLayout *vbox1 = new QVBoxLayout;
   mpss_gb_start->setLayout(vbox1);
   vbox1->addWidget(mpss_lb_start);

   QVBoxLayout *vbox2 = new QVBoxLayout;
   mpss_gb_end->setLayout(vbox2);
   vbox2->addWidget(mpss_lb_end);

   QVBoxLayout *vbox3 = new QVBoxLayout;
   mpss_gb_range->setLayout(vbox3);
   vbox3->addWidget(mpss_lb_range);

}


void mSessionsWidget::on_stv_changed(const QItemSelection & sel, const QItemSelection & dsel)
{
   // cycle the selected rows and set mbase.sessionsDtStart and mbase.sessionsDtEnd

   mbase.sessionsDtStart = QDateTime::fromString("2100", "yyyy"); // set start to max
   mbase.sessionsDtEnd   = QDateTime::fromString("1900", "yyyy"); // set end to min

   QModelIndexList selection = sessionsTableView->selectionModel()->selectedRows();
   for(int i=0; i< selection.count(); i++)
   {
      QModelIndex index = selection.at(i);

      // get datetime start
      QString ds = mbase.sessionsModel->data(index, Qt::DisplayRole).toString();
      QDateTime dts = QDateTime::fromString(ds, "yyyy-MM-ddTHH:mm:ss.zzz");

      // get datetime end
      QModelIndex qmi = index.siblingAtColumn(1);
      QString de = mbase.sessionsModel->data(qmi, Qt::DisplayRole).toString();
      QDateTime dte = QDateTime::fromString(de, "yyyy-MM-ddTHH:mm:ss.zzz");

      if (dts < mbase.sessionsDtStart) mbase.sessionsDtStart = dts;
      if (dte > mbase.sessionsDtEnd)   mbase.sessionsDtEnd = dte;
   }

   QString st = mbase.sessionsDtStart.toString("yyyy-MM-dd HH:mm:ss");
   QString nd = mbase.sessionsDtEnd.toString("yyyy-MM-dd HH:mm:ss");
   mpss_lb_start->setText(st);
   mpss_lb_end->setText(nd);

   int ts = mbase.sessionsDtStart.secsTo(mbase.sessionsDtEnd);
   mbase.sessionsRange = QTime::fromString("0", "s");
   mbase.sessionsRange = mbase.sessionsRange.addSecs(ts);
   if (mbase.sessionsRange.isValid()) mpss_lb_range->setText(mbase.sessionsRange.toString("hh:mm:ss"));
   else                               mpss_lb_range->setText("not valid");

   mbase.sessionSelectionChanged();
}













































































  /*


//   mbase.sessionsDtStart = QDateTime::fromString("1.30.1", "M.d.s");


   QDateTime dateTime = QDateTime::fromString("1.30.1", "M.d.s");
   // dateTime is January 30 in 1900 at 00:00:01.
   dateTime = QDateTime::fromString("12", "yy");
   // dateTime is January 1 in 1912 at 00:00:00.


//   QString tt = mbase.sessionsDtStart.toString("yyyy-MM-ddTHH:mm:ss.zzz");

//   qDebug() << mbase.sessionsDtStart;


//   qDebug() << tt;


//   qDebug() << mbase.sessionsDtStart.toString("yyyy-MM-ddTHH:mm:ss.zzz");

//   qDebug() << mbase.sessionsDtStart.toString("yyyy-MM-ddTHH:mm:ss.zzz");


   qDebug() << "Selection changed";
   // show which rows are selected
   QModelIndexList selection = sessionsTableView->selectionModel()->selectedRows();
   for(int i=0; i< selection.count(); i++)
   {
      QModelIndex index = selection.at(i);
      // qDebug() << index.row();

      // get datetime start
      QString ds = sessionsModel->data(index, Qt::DisplayRole).toString();

      QDateTime dts = QDateTime::fromString(ds, "yyyy-MM-ddTHH:mm:ss.zzz");
      qDebug() << "start:" << dts.toString("yyyy-MM-ddTHH:mm:ss.zzz");


      // get datetime end
      QModelIndex qmi = index.siblingAtColumn(1);
      QString de = sessionsModel->data(qmi, Qt::DisplayRole).toString();

      QDateTime dte = QDateTime::fromString(de, "yyyy-MM-ddTHH:mm:ss.zzz");
      qDebug() << "end  :" << dte.toString("yyyy-MM-ddTHH:mm:ss.zzz");

      qDebug() << ds << " - " << de;


      if (dts < mbase.sessionsDtStart) mbase.sessionsDtStart = dts;
      if (dte > mbase.sessionsDtEnd) mbase.sessionsDtEnd = dte;


   }


   qDebug() << "Start:" << mbase.sessionsDtStart.toString("yyyy-MM-ddTHH:mm:ss.zzz");
   qDebug() << "End  :" << mbase.sessionsDtEnd.toString("yyyy-MM-ddTHH:mm:ss.zzz");



}




      // find message_type sibling in column 1
      QModelIndex qmi = index.siblingAtColumn(1);

      name = sessionsModel->data(qmi, Qt::DisplayRole);
      dt = name.toString();
      qDebug() << dt;

*/







/*

      QModelIndex index = selection.at(i);
      qDebug() << index.row();

      QVariant name = sessionsModel->data(index, Qt::DisplayRole);
      QString dt = name.toString();
      qDebug() << dt;



      // find message_type sibling in column 1
      QModelIndex qmi = index.siblingAtColumn(1);
      name = sessionsModel->data(qmi, Qt::DisplayRole);
      dt = name.toString();
      qDebug() << dt;

  */











      // get time ...

      /*

      QModelIndex i2 = sessionsModel->index(i, 1); // row from above col 1
      QVariant name =  sessionsModel->data(i2, Qt::DisplayRole);
      QString dt = name.toString();
      qDebug() << dt;

*/



/*


what is the model



      QModelIndex index = mbase.status_model1->index(r, 0);
      QVariant name = mbase.status_model1->data(index, Qt::DisplayRole);
      QString dt = name.toString();

      index = mbase.status_model1->index(r, 1);
      name = mbase.status_model1->data(index);
      float cpu = name.toFloat();

      qDebug() << dt << cpu;

      QDateTime momentInTime = QDateTime::fromString(dt, "yyyy-MM-ddTHH:mm:ss.zzz");

      // put into line series
      mbase.series1->append(momentInTime.toMSecsSinceEpoch(), cpu);

  */







// find the earliest date and set sessionsDtStart;

// set to max date possible then test if lower

//   mbase












/*

   // this is the time range set by selecting one or more sessions
   QDateTime sessionsDtStart;
   QDateTime sessionsDtEnd;

*/





















































