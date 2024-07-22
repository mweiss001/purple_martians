#include "mTableColumnSelectWidget.h"

mTableColumnSelectWidget::mTableColumnSelectWidget(QWidget *parent) : QWidget{parent}
{
   QVBoxLayout *vbox = new QVBoxLayout;

//   QHBoxLayout *hbox = new QHBoxLayout;

   QGroupBox * mpms_gb = new QGroupBox("Column Select", this);
   mpms_gb->setGeometry(0, 0, HSIZE, VSIZE);
   mpms_gb->setLayout(vbox);

   QPushButton * all  = new QPushButton("all", this);
   connect(all, SIGNAL (clicked()), this, SLOT (all_clicked()));
   QPushButton * none = new QPushButton("none", this);
   connect(none, SIGNAL (clicked()), this, SLOT (none_clicked()));

   vbox->addWidget(none);
   vbox->addWidget(all);

  // vbox->addLayout(hbox);





   readColumnSelection();

   for (int i=0; i<10; i++)
   {
      if (mbase.col_types[i].valid)
      {
         mpms_cb[i] = new QCheckBox(mbase.col_types[i].db_name, mpms_gb);
         mpms_cb[i]->setChecked(mbase.col_types[i].shown);
         vbox->addWidget(mpms_cb[i]);
         connect(mpms_cb[i], SIGNAL(clicked()), this, SLOT (cbClicked())) ;
      }
   }
}

void mTableColumnSelectWidget::cbClicked()
{
   // set array from checkboxes
   for (int i=0; i<10; i++)
      if (mbase.col_types[i].valid)
      {
         if (mpms_cb[i]->checkState()) mbase.col_types[i].shown = 1;
         else                          mbase.col_types[i].shown = 0;
      }

   mbase.mTablesWidgetUpdateColumnsFunction();
   saveColumnSelection();
}

void mTableColumnSelectWidget::all_clicked()
{
   for (int i=0; i<10; i++)
      if (mbase.col_types[i].valid)
         mpms_cb[i]->setChecked(true);
   cbClicked();
}

void mTableColumnSelectWidget::none_clicked()
{
   for (int i=0; i<10; i++)
      if (mbase.col_types[i].valid)
         mpms_cb[i]->setChecked(false);
   cbClicked();
}

void mTableColumnSelectWidget::saveColumnSelection()
{
   //qDebug() << "mTableColumnSelectWidget::saveColumnSelection()";
   mbase.settings->beginWriteArray("mTableColumnSelect");
   for (int i=0; i<10; i++)
   {
      mbase.settings->setArrayIndex(i);
      mbase.settings->setValue("sel", mbase.col_types[i].shown);
      //qDebug() << "w:" << i << " - " << mbase.col_types[i].shown;
   }
   mbase.settings->endArray();
}

void mTableColumnSelectWidget::readColumnSelection()
{
   //qDebug() << "mTableColumnSelectWidget::readColumnSelection()";
   mbase.settings->beginReadArray("mTableColumnSelect");
   for (int i=0; i<10; i++)
   {
      mbase.settings->setArrayIndex(i);
      mbase.col_types[i].shown = mbase.settings->value("sel", 1).toInt();
      //qDebug() << "r:" << i << " - " << mbase.col_types[i].shown;
   }
   mbase.settings->endArray();
}




















