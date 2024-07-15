#include "msqlwidget.h"

MSQLWidget::MSQLWidget(QWidget *parent)
    : QWidget{parent}
{

   QVBoxLayout *vbox = new QVBoxLayout;

   msqw_gb = new QGroupBox("SQL Widget", this);
   msqw_gb->setGeometry(10, 10, VSIZE-20, HSIZE-20);
   msqw_gb->setLayout(vbox);

   msqw1_gb = new QGroupBox("Main", this);
   msqw2_gb = new QGroupBox("Where Date Range", this);
   msqw3_gb = new QGroupBox("Where Message Type", this);

   msqw1_tb = new QTextBrowser(this);
   msqw2_tb = new QTextBrowser(this);
   msqw3_tb = new QTextBrowser(this);

   QVBoxLayout *vbox1 = new QVBoxLayout;
   msqw1_gb->setLayout(vbox1);
   vbox1->addWidget(msqw1_tb);

   QVBoxLayout *vbox2 = new QVBoxLayout;
   msqw2_gb->setLayout(vbox2);
   vbox2->addWidget(msqw2_tb);

   QVBoxLayout *vbox3 = new QVBoxLayout;
   msqw3_gb->setLayout(vbox3);
   vbox3->addWidget(msqw3_tb);

   vbox->addWidget(msqw1_gb);
   vbox->addWidget(msqw2_gb);
   vbox->addWidget(msqw3_gb);
}


void MSQLWidget::update(void)
{
//   msqw1_tb->setText(mbase.pml_sql_all);
//   msqw2_tb->setText(mbase.pml_sql_where_date);
//   msqw3_tb->setText(mbase.mTablesWidgetControlWidgetSqlWhereType);
}
