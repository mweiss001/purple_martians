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
      int HSIZE = 200;
      int VSIZE = 300;
      QSize minimumSizeHint() const  {  return QSize(HSIZE, VSIZE);   }
      QSize minimumSize () const     {  return QSize(HSIZE, VSIZE);   }

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

