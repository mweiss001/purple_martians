#ifndef MTABLEROWSELECTWIDGET_H
#define MTABLEROWSELECTWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QCheckBox>
#include <QBoxLayout>
#include <QPushButton>
#include "m_base.h"

class mTableRowSelectWidget : public QWidget
{
   Q_OBJECT
   public:
      explicit mTableRowSelectWidget(QWidget *parent = nullptr);
      void set_sql_where_types(void);
      void set_array_from_cb(void);
      int VSIZE = 200;
      int HSIZE = 440;
      QSize minimumSizeHint() const  {  return QSize(VSIZE, HSIZE);   }
      QSize minimumSize () const     {  return QSize(VSIZE, HSIZE);   }

   signals:

   private slots:
      void cbClicked();
      void all_clicked();
      void none_clicked();

   private:
      QCheckBox * mlts_cb[100];
      void saveRowSelection();
      void readRowSelection();
};

#endif // MTABLEROWSELECTWIDGET_H

