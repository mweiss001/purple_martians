#ifndef MTABLESWIDGETTABLE_H
#define MTABLESWIDGETTABLE_H

#include <QWidget>
#include <QBoxLayout>
#include <QGroupBox>

#include <QFrame>


#include "m_base.h"

#include "mTableView.h"
#include "mTablesWidgetTableFilter.h"

class mTablesWidgetTable : public QWidget
{
   Q_OBJECT
   public:
      mTablesWidgetTable(QWidget *parent = nullptr, int id = 0, QSqlQueryModel * qm = nullptr)
      {
         tableId = id;

         QVBoxLayout *vbox1 = new QVBoxLayout;
         this->setLayout(vbox1);

         vbox1->setContentsMargins(0,0,0,0);

         frame = new QFrame();
         vbox1->addWidget(frame);

         QVBoxLayout *vbox = new QVBoxLayout;
         frame->setLayout(vbox);

         vbox->setContentsMargins(0,0,0,0);
         vbox->setSpacing(0);

         tableFilter = new mTablesWidgetTableFilter(this, tableId);
         vbox->addWidget(tableFilter);

         tableView = new mTableView();
         tableView->setModel(qm);
         vbox->addWidget(tableView);

      }
   int tableId;
   QFrame * frame;

   mTableView * tableView = nullptr;
   mTablesWidgetTableFilter * tableFilter = nullptr;
};
#endif // MTABLESWIDGETTABLE_H
