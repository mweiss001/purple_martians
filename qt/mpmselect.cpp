#include "mpmselect.h"

MPMSelect::MPMSelect(QWidget *parent)
    : QWidget{parent}
{

   QVBoxLayout *vbox = new QVBoxLayout;
   QHBoxLayout *hbox = new QHBoxLayout;

   mpms_gb = new QGroupBox("Column Select", this);
   mpms_gb->setGeometry(10, 10, VSIZE-20, HSIZE-20);
   mpms_gb->setLayout(vbox);

   mpms_pb_all  = new QPushButton("all", this);
   connect(mpms_pb_all, SIGNAL (clicked()), this, SLOT (on_mpms_pb_all_clicked()));
   mpms_pb_none = new QPushButton("none", this);
   connect(mpms_pb_none, SIGNAL (clicked()), this, SLOT (on_mpms_pb_none_clicked()));

   hbox->addWidget(mpms_pb_none);
   hbox->addWidget(mpms_pb_all);

   vbox->addLayout(hbox);

   for (int i=0; i<10; i++)
   {
      if (mbase.col_types[i].valid)
      {
         mpms_cb[i] = new QCheckBox(mbase.col_types[i].db_name, mpms_gb);
         mpms_cb[i]->setChecked(mbase.col_types[i].shown);
         vbox->addWidget(mpms_cb[i]);
         connect(mpms_cb[i], SIGNAL (checkStateChanged(Qt::CheckState)), this, SLOT (cb_tog(Qt::CheckState))) ;
      }
   }
}

void MPMSelect::cb_tog(Qt::CheckState)
{
   set_array_from_cb();
   emit sel_changed();
}

void MPMSelect::set_array_from_cb(void)
{
   for (int i=0; i<10; i++)
      if (mbase.col_types[i].valid)
      {
         if (mpms_cb[i]->checkState()) mbase.col_types[i].shown = 1;
         else                          mbase.col_types[i].shown = 0;
      }
}

void MPMSelect::on_mpms_pb_all_clicked()
{
   for (int i=0; i<10; i++)
      if (mbase.col_types[i].valid)
         mpms_cb[i]->setChecked(true);
}

void MPMSelect::on_mpms_pb_none_clicked()
{
   for (int i=0; i<10; i++)
      if (mbase.col_types[i].valid)
         mpms_cb[i]->setChecked(false);
}





















