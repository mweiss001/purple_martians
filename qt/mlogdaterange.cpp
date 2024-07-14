#include "mlogdaterange.h"

MLogDateRange::MLogDateRange(QWidget *parent)
    : QWidget{parent}
{
   QVBoxLayout *vbox = new QVBoxLayout;

   mldr_gb = new QGroupBox("Date Range Select", this);
   mldr_gb->setGeometry(10, 10, VSIZE-20, HSIZE-20);
   mldr_gb->setLayout(vbox);

   QVBoxLayout *vboxb = new QVBoxLayout;
   mldrb_gb = new QGroupBox("Beginning", this);
   mldrb_gb->setLayout(vboxb);

   QVBoxLayout *vboxe = new QVBoxLayout;
   mldre_gb = new QGroupBox("End", this);
   mldre_gb->setLayout(vboxe);

   QVBoxLayout *vboxr = new QVBoxLayout;
   mldrr_gb = new QGroupBox("Range", this);
   mldrr_gb->setLayout(vboxr);

   dte_beg = new QDateTimeEdit(QDateTime::currentDateTime(), this);
   dte_beg->setDisplayFormat("yyyy-MM-dd HH:mm:ss.z");

   dte_end = new QDateTimeEdit(QDateTime::currentDateTime(), this);
   dte_end->setDisplayFormat("yyyy-MM-dd HH:mm:ss.z");

   connect(dte_beg, SIGNAL (dateTimeChanged(QDateTime) ), this, SLOT (on_dte_changed(QDateTime)));
   connect(dte_end, SIGNAL (dateTimeChanged(QDateTime) ), this, SLOT (on_dte_changed(QDateTime)));

   pb_beg_earliest = new QPushButton("Earliest", this);
   connect(pb_beg_earliest, SIGNAL (clicked()), this, SLOT (on_pb_beg_earliest_clicked()));

   pb_end_latest = new QPushButton("Latest", this);
   connect(pb_end_latest, SIGNAL (clicked()), this, SLOT (on_pb_end_latest_clicked()));


   vboxb->addWidget(dte_beg);
   vboxb->addWidget(pb_beg_earliest);
   vboxe->addWidget(dte_end);
   vboxe->addWidget(pb_end_latest);

   mldr_lb_range = new QLabel();
   vboxr->addWidget(mldr_lb_range);

   vbox->addWidget(mldrb_gb);
   vbox->addWidget(mldre_gb);
   vbox->addWidget(mldrr_gb);

   on_pb_beg_earliest_clicked();
   on_pb_end_latest_clicked();

   set_sql_where_date_range();
}


void MLogDateRange::on_dte_changed(const QDateTime &dateTime)
{
   set_sql_where_date_range();

   int ts = dte_beg->dateTime().secsTo(dte_end->dateTime());
   QTime qt(0, 0, 0);
   qt = qt.addSecs(ts);

   // if (qt.isValid()) mldr_lb_range->setText(qt.toString("hh:mm:ss"));
   // else              mldr_lb_range->setText(qt.toString("not valid"));

   mldr_lb_range->setText(qt.toString("hh:mm:ss"));

   // this does not work....dates are 1969
   // int ts = dte_beg->dateTime().secsTo(dte_end->dateTime());
   // QDateTime qdt;
   // qdt.setMSecsSinceEpoch(ts*1000);
   // mldr_lb_range->setText(qdt.toString("yyyy-MM-dd hh:mm:ss"));
}

void MLogDateRange::set_sql_where_date_range(void)
{
   QString sql = "created BETWEEN '";
   sql += dte_beg->dateTime().toString("yyyy-MM-dd HH:mm:ss.z");
   sql += "' AND '";
   sql += dte_end->dateTime().toString("yyyy-MM-dd HH:mm:ss.z");
   sql += "'";
   // qDebug() << sql;

//   mbase.pml_sql_where_date = sql;
//   mbase.update_sql();

}


void MLogDateRange::on_pb_beg_earliest_clicked()
{
   if (mbase.db.isOpen())
   {
      QSqlQuery query;
      if (query.exec("SELECT MIN(created) FROM logs"))
      {
         query.next();
         dte_beg->setDateTime(QDateTime::fromString(query.value(0).toString(), "yyyy-MM-ddTHH:mm:ss.zzz"));
      }
      else qDebug() << "query error:" << query.lastError().text();
   }
}

void MLogDateRange::on_pb_end_latest_clicked()
{
   if (mbase.db.isOpen())
   {
      QSqlQuery query;
      if (query.exec("SELECT MAX(created) FROM logs"))
      {
         query.next();
         dte_end->setDateTime(QDateTime::fromString(query.value(0).toString(), "yyyy-MM-ddTHH:mm:ss.zzz"));
      }
      else qDebug() << "query error:" << query.lastError().text();
   }
}
