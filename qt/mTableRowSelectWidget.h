#ifndef MTABLEROWSELECTWIDGET_H
#define MTABLEROWSELECTWIDGET_H

#include <QWidget>
#include <QBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include "m_base.h"

class mTableRowSelectWidget : public QWidget
{
   Q_OBJECT
   public:
      explicit mTableRowSelectWidget(QWidget *parent = nullptr)
      {
         // create top laout and add to this->
         QVBoxLayout *topLayout = new QVBoxLayout;
         topLayout->setContentsMargins(0, 0, 0, 0);
         this->setLayout(topLayout);

         // create top level groupbox add to top layout
         QGroupBox * topLayoutGroupBox = new QGroupBox("Message Types", this);

         topLayoutGroupBox->setMinimumSize(108, 300);
         topLayoutGroupBox->setMaximumSize(108, 300);
         topLayout->addWidget(topLayoutGroupBox);

         // create vertical layout for inside groupbox
         QVBoxLayout *topLayoutGroupBoxLayout = new QVBoxLayout;
         topLayoutGroupBox->setLayout(topLayoutGroupBoxLayout);


         // create and add buttons to layout
         QPushButton * none = new QPushButton("none");
         connect(none, SIGNAL (clicked()), this, SLOT (noneClicked()));
         topLayoutGroupBoxLayout->addWidget(none);

         QPushButton * all  = new QPushButton("all");
         connect(all, SIGNAL (clicked()), this, SLOT (allClicked()));
         topLayoutGroupBoxLayout->addWidget(all);


         // create and add checkboxes to layout
         readRowSelection();
         for (int i=10; i<40; i++)
            if (mbase.log_types[i].valid)
            {
               mlts_cb[i] = new QCheckBox(mbase.log_types[i].name);
               mlts_cb[i]->setChecked(mbase.log_types[i].shown);
               mlts_cb[i]->setStyleSheet("background-color: " + mbase.log_types[i].color.name(QColor::HexArgb) + " ; border: 2px ;");
               topLayoutGroupBoxLayout->addWidget(mlts_cb[i]);
               connect(mlts_cb[i], SIGNAL(clicked()), this, SLOT(cbClicked()));
            }
//         topLayoutGroupBoxLayout->addStretch(1);
         set_sql_where_types();
      }

   private slots:
      void allClicked()
      {
         for (int i=10; i<40; i++)
            if (mbase.log_types[i].valid)  mlts_cb[i]->setChecked(true);
         cbClicked();
      }

      void noneClicked()
      {
         for (int i=10; i<40; i++)
            if (mbase.log_types[i].valid) mlts_cb[i]->setChecked(false);
         cbClicked();
      }

      void cbClicked()
      {
         //qDebug() << "mTableRowSelectWidget::cbClicked()";
         set_array_from_cb();
         set_sql_where_types();
         saveRowSelection();
         mbase.mTablesWidgetReloadModelFunction();
      }

  private:
      QCheckBox * mlts_cb[100];

      void set_array_from_cb(void)
      {
         for (int i=10; i<40; i++)
            if (mbase.log_types[i].valid) mbase.log_types[i].shown = mlts_cb[i]->checkState();
      }

      void set_sql_where_types(void)
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

      void saveRowSelection()
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

      void readRowSelection()
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
};
#endif // MTABLEROWSELECTWIDGET_H




