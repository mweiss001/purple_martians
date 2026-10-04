#ifndef MSESSIONSWIDGET_H
#define MSESSIONSWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QBoxLayout>
#include <QTableView>
#include <QSqlQueryModel>
#include <QHeaderView>

#include "mSessionsWidgetTableViewDelegate.h"

class mSessionsWidget : public QWidget
{
   Q_OBJECT
   public:
      explicit mSessionsWidget(QWidget *parent = nullptr)
      {
         // create topLayout and apply to this->
         QHBoxLayout *topLayout = new QHBoxLayout;
         this->setLayout(topLayout);

         // create groupBox and add to topLayout
         groupBox = new QGroupBox("Sessions", this);
         groupBox->setMinimumWidth(600);
         groupBox->setMaximumWidth(600);
         topLayout->addWidget(groupBox);


         groupBox->setObjectName("t1");

         QString style = "QGroupBox#t1 { border: 1px solid " + mbase.sessionsColor.name();
         style += "; margin: 0px; margin-top:4px; background-color:" + mbase.sessionsColor.lighter(195).name();
         style += "; } QGroupBox#t1::title { left: 10px; top: -8px; }";

         groupBox->setStyleSheet(style);

         // create layout for groupBox
         QVBoxLayout *groupBoxLayout = new QVBoxLayout;
         groupBox->setLayout(groupBoxLayout);

         tableView = new QTableView(this);
         tableView->setModel(mbase.sessionsModel);
         tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
         tableView->setItemDelegate(new SessionsWidgetTableViewDelegate);
         tableView->horizontalHeader()->setStretchLastSection(true);
         tableView->resizeColumnsToContents();
         tableView->viewport()->update();

         connect(tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
         groupBoxLayout->addWidget(tableView);
      }

   private slots:
      void selectionChanged(const QItemSelection & sel, const QItemSelection & dsel)
      {
         // cycle the selected rows and set mbase.sessionsDtStart and mbase.sessionsDtEnd
         mbase.sessionsDtStart = QDateTime::fromString("2100", "yyyy"); // set start to max
         mbase.sessionsDtEnd   = QDateTime::fromString("1900", "yyyy"); // set end to min

         QModelIndexList selection = tableView->selectionModel()->selectedRows();
         for (int i=0; i<selection.count(); i++)
         {
            // get datetime start
            QModelIndex index = selection.at(i);
            QDateTime dts = mbase.sessionsModel->data(index, Qt::DisplayRole).toDateTime();

            // get datetime end
            QModelIndex qmi = index.siblingAtColumn(1);
            QDateTime dte = mbase.sessionsModel->data(qmi, Qt::DisplayRole).toDateTime();

            if (dts < mbase.sessionsDtStart) mbase.sessionsDtStart = dts;
            if (dte > mbase.sessionsDtEnd)   mbase.sessionsDtEnd = dte;
         }








         mbase.sessionSelectionChanged();
      }
   private:
      QGroupBox * groupBox;
      QTableView * tableView;
};
#endif // MSESSIONSWIDGET_H
