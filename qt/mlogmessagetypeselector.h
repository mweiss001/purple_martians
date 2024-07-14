#ifndef MLOGMESSAGETYPESELECTOR_H
#define MLOGMESSAGETYPESELECTOR_H

#include <QWidget>
#include <QGroupBox>
#include <QCheckBox>
#include <QBoxLayout>
#include <QPushButton>
#include "m_base.h"

class MLogMessageTypeSelector : public QWidget
{
   Q_OBJECT
public:
   explicit MLogMessageTypeSelector(QWidget *parent = nullptr);
   void set_sql_where_types(void);
   void set_array_from_cb(void);
   int VSIZE = 200;
   int HSIZE = 440;
   QSize minimumSizeHint() const  {  return QSize(VSIZE, HSIZE);   }
   QSize minimumSize () const     {  return QSize(VSIZE, HSIZE);   }

signals:

private slots:
   void on_cb_tog(Qt::CheckState);
   void on_mlts_pb_all_clicked();
   void on_mlts_pb_none_clicked();

private:
   QPushButton * mlts_pb_all;
   QPushButton * mlts_pb_none;
   QGroupBox * mlts_gb;
   QCheckBox * mlts_cb[100];
};

#endif // MLOGMESSAGETYPESELECTOR_H
