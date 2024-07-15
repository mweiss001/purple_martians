#include "mTableRowSelectWidget.h"

mTableRowSelectWidget::mTableRowSelectWidget(QWidget *parent) : QWidget{parent}
{
   QVBoxLayout *vbox = new QVBoxLayout;
   QHBoxLayout *hbox = new QHBoxLayout;

   QGroupBox * mlts_gb = new QGroupBox("Log Message Types", this);
   mlts_gb->setGeometry(10, 10, VSIZE-20, HSIZE-20);
   mlts_gb->setLayout(vbox);


   QPushButton * all  = new QPushButton("all", this);
   connect(all, SIGNAL (clicked()), this, SLOT (all_clicked()));

   QPushButton * none = new QPushButton("none", this);
   connect(none, SIGNAL (clicked()), this, SLOT (none_clicked()));

   hbox->addWidget(none);
   hbox->addWidget(all);

   vbox->addLayout(hbox);

   readRowSelection();

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
         connect(mlts_cb[i], SIGNAL(clicked()), this, SLOT(cbClicked()));

      }
   }
   vbox->addStretch(1);
   set_sql_where_types();
}

void mTableRowSelectWidget::all_clicked()
{
   for (int i=10; i<40; i++)
      if (mbase.log_types[i].valid)
         mlts_cb[i]->setChecked(true);
   cbClicked();
}

void mTableRowSelectWidget::none_clicked()
{
   for (int i=10; i<40; i++)
      if (mbase.log_types[i].valid)
         mlts_cb[i]->setChecked(false);
   cbClicked();
}

void mTableRowSelectWidget::cbClicked()
{
   //qDebug() << "mTableRowSelectWidget::cbClicked()";
   set_array_from_cb();
   set_sql_where_types();
   saveRowSelection();
   mbase.mTablesWidgetUpdateFunction();
}

void mTableRowSelectWidget::set_array_from_cb(void)
{
   for (int i=10; i<40; i++)
      if (mbase.log_types[i].valid)
      {
         if (mlts_cb[i]->checkState()) mbase.log_types[i].shown = 1;
         else                          mbase.log_types[i].shown = 0;
      }
}


void mTableRowSelectWidget::set_sql_where_types(void)
{
   //qDebug() << "mTableRowSelectWidget::set_sql_where_types(void)";
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
   mbase.mTablesWidgetControlWidgetSqlWhereTypeClause = sql;
}



void mTableRowSelectWidget::saveRowSelection()
{
   //qDebug() << "mTableRowSelectWidget::saveRowSelection()";
   mbase.settings->beginWriteArray("mTableRowSelect");
   for (int i=10; i<40; i++)
   {
      mbase.settings->setArrayIndex(i);
      mbase.settings->setValue("sel", mbase.log_types[i].shown);
      //qDebug() << "w:" << i << " - " << mbase.log_types[i].shown;
   }
   mbase.settings->endArray();
}

void mTableRowSelectWidget::readRowSelection()
{
   //qDebug() << "mTableRowSelectWidget::readRowSelection()";
   mbase.settings->beginReadArray("mTableRowSelect");
   for (int i=10; i<40; i++)
   {
      mbase.settings->setArrayIndex(i);
      mbase.log_types[i].shown = mbase.settings->value("sel", 1).toInt();
      //qDebug() << "r:" << i << " - " << mbase.log_types[i].shown;
   }
   mbase.settings->endArray();
}






















