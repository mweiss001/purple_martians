#ifndef MTABLECOLUMNSELECTWIDGET_H
#define MTABLECOLUMNSELECTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>

#include <QCheckBox>
#include <QBoxLayout>
#include <QPushButton>
#include "m_base.h"


class mTableColumnSelectWidget : public QWidget
{
   Q_OBJECT
   public:
      explicit mTableColumnSelectWidget(QWidget *parent = nullptr);

      int VSIZE = 200;
      int HSIZE = 320;
      QSize minimumSizeHint() const {      return QSize(VSIZE, HSIZE);   }
      QSize minimumSize () const    {      return QSize(VSIZE, HSIZE);   }

   signals:

   private slots:
      void cbClicked();
      void all_clicked();
      void none_clicked();

   private:
      QCheckBox * mpms_cb[10];
      void saveColumnSelection();
      void readColumnSelection();
};

#endif // MTABLECOLUMNSELECTWIDGET_H
