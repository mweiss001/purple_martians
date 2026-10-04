#ifndef MTABLECOLUMNSELECTWIDGET_H
#define MTABLECOLUMNSELECTWIDGET_H

#include <QWidget>
#include <QBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include "m_base.h"


class mTableColumnSelectWidget : public QWidget
{
   Q_OBJECT
   public:
      explicit mTableColumnSelectWidget(QWidget *parent = nullptr)
      {
         //qDebug() << "mTableColumnSelectWidget::constructor()";

         // create top laout and add to this->
         QVBoxLayout *topLayout = new QVBoxLayout;
         topLayout->setContentsMargins(0, 0, 0, 0);

         this->setLayout(topLayout);

         // create top level groupbox add to top layout
         QGroupBox * topLayoutGroupBox = new QGroupBox("Column Select", this);
         topLayoutGroupBox->setMinimumSize(100, 300);
         topLayoutGroupBox->setMaximumSize(100, 300);
         topLayout->addWidget(topLayoutGroupBox);

         // create vertical layout for inside groupbox
         QVBoxLayout *topLayoutGroupBoxLayout = new QVBoxLayout;
         topLayoutGroupBox->setLayout(topLayoutGroupBoxLayout);

         // create and add buttons to layout
         QPushButton * none = new QPushButton("none", this);
         connect(none, SIGNAL (clicked()), this, SLOT (noneClicked()));
         topLayoutGroupBoxLayout->addWidget(none);

         QPushButton * all  = new QPushButton("all", this);
         connect(all, SIGNAL (clicked()), this, SLOT (allClicked()));
         topLayoutGroupBoxLayout->addWidget(all);

         readColumnSelection();

         for (int i=0; i<10; i++)
         {
            if (mbase.col_types[i].valid)
            {
               mpms_cb[i] = new QCheckBox(mbase.col_types[i].db_name);
               mpms_cb[i]->setChecked(mbase.col_types[i].shown);
               mpms_cb[i]->setStyleSheet("border: 2px ;");

               topLayoutGroupBoxLayout->addWidget(mpms_cb[i]);
               connect(mpms_cb[i], SIGNAL(clicked()), this, SLOT (cbClicked())) ;
            }
         }
      }

   private slots:
      void cbClicked()
      {
         // set array from checkboxes
         for (int i=0; i<10; i++)
            if (mbase.col_types[i].valid) mbase.col_types[i].shown = mpms_cb[i]->checkState();

         mbase.mTablesWidgetUpdateUIFunction();
         saveColumnSelection();
      }
      void allClicked()
      {
         for (int i=0; i<10; i++)
            if (mbase.col_types[i].valid) mpms_cb[i]->setChecked(true);
         cbClicked();
      }
      void noneClicked()
      {
         for (int i=0; i<10; i++)
            if (mbase.col_types[i].valid) mpms_cb[i]->setChecked(false);
         cbClicked();
      }

   private:
      QCheckBox * mpms_cb[10];

      void saveColumnSelection()
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

      void readColumnSelection()
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
};
#endif // MTABLECOLUMNSELECTWIDGET_H
