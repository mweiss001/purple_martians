#include "mlogmessagetypeselector.h"

MLogMessageTypeSelector::MLogMessageTypeSelector(QWidget *parent)
    : QWidget{parent}
{

   QVBoxLayout *vbox = new QVBoxLayout;
   QHBoxLayout *hbox = new QHBoxLayout;

   mlts_gb = new QGroupBox("Log Message Types", this);
   mlts_gb->setGeometry(10, 10, VSIZE-20, HSIZE-20);
   mlts_gb->setLayout(vbox);

   mlts_pb_all  = new QPushButton("all", this);
   connect(mlts_pb_all, SIGNAL (clicked()), this, SLOT (on_mlts_pb_all_clicked()));

   mlts_pb_none = new QPushButton("none", this);
   connect(mlts_pb_none, SIGNAL (clicked()), this, SLOT (on_mlts_pb_none_clicked()));

   hbox->addWidget(mlts_pb_none);
   hbox->addWidget(mlts_pb_all);

   vbox->addLayout(hbox);

   for (int i=10; i<40; i++)
   {
      if (mbase.log_types[i].valid)
      {
         mlts_cb[i] = new QCheckBox(mbase.log_types[i].name, mlts_gb);
         mlts_cb[i]->setChecked(mbase.log_types[i].shown);

         QColor c = mbase.log_types[i].color;
         QString s = QString("background-color: rgba(%1,%2,%3,%4); border: 2px ;").arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha());
         mlts_cb[i]->setStyleSheet(s);

         vbox->addWidget(mlts_cb[i]);
         connect(mlts_cb[i], SIGNAL (checkStateChanged(Qt::CheckState)), this, SLOT (on_cb_tog(Qt::CheckState))) ;
      }
   }
   vbox->addStretch(1);
   set_sql_where_types();
}

void MLogMessageTypeSelector::on_mlts_pb_all_clicked()
{
   for (int i=10; i<40; i++)
      if (mbase.log_types[i].valid)
         mlts_cb[i]->setChecked(true);
}

void MLogMessageTypeSelector::on_mlts_pb_none_clicked()
{
   for (int i=10; i<40; i++)
      if (mbase.log_types[i].valid)
         mlts_cb[i]->setChecked(false);
}

void MLogMessageTypeSelector::on_cb_tog(Qt::CheckState)
{
   set_array_from_cb();
   set_sql_where_types();
}

void MLogMessageTypeSelector::set_array_from_cb(void)
{
   for (int i=10; i<40; i++)
      if (mbase.log_types[i].valid)
      {
         if (mlts_cb[i]->checkState()) mbase.log_types[i].shown = 1;
         else                          mbase.log_types[i].shown = 0;
      }
}

void MLogMessageTypeSelector::set_sql_where_types(void)
{
   QString sql = "(";
   for (int i=10; i<40; i++)
      if ((mbase.log_types[i].valid) && (mbase.log_types[i].shown))
      {
         sql += "msg_type=";
         sql += QString::number(i);
         sql += " OR ";
      }
   if (sql.length() > 4) sql.chop(4); // remove last OR
   sql += ")";

   //qDebug() << sql;

   mbase.pml_sql_where_type = sql;
   mbase.update_sql();
}















