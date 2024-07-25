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
         QVBoxLayout *vbox = new QVBoxLayout;

         QGroupBox * mpms_gb = new QGroupBox("Column Select", this);
         mpms_gb->setLayout(vbox);

         QPushButton * all  = new QPushButton("all", this);
         connect(all, SIGNAL (clicked()), this, SLOT (allClicked()));
         QPushButton * none = new QPushButton("none", this);
         connect(none, SIGNAL (clicked()), this, SLOT (noneClicked()));

         vbox->addWidget(none);
         vbox->addWidget(all);

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

      QSize minimumSizeHint() const { return QSize(100, 330); }

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
