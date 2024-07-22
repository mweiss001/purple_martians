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

      int HSIZE = 90;
      int VSIZE = 320;
      QSize minimumSizeHint() const { return QSize(HSIZE, VSIZE); }
      QSize minimumSize()     const { return QSize(HSIZE, VSIZE); }

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
