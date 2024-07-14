#ifndef MPMSELECT_H
#define MPMSELECT_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>

#include <QCheckBox>
#include <QBoxLayout>
#include <QPushButton>
#include "m_base.h"


class MPMSelect : public QWidget
{
   Q_OBJECT
public:
   explicit MPMSelect(QWidget *parent = nullptr);

   void set_array_from_cb(void);

   int VSIZE = 200;
   int HSIZE = 320;
   QSize minimumSizeHint() const {      return QSize(VSIZE, HSIZE);   }
   QSize minimumSize () const    {      return QSize(VSIZE, HSIZE);   }


signals:
   void sel_changed(void);

private slots:
   void cb_tog(Qt::CheckState);
   void on_mpms_pb_all_clicked();
   void on_mpms_pb_none_clicked();

private:
   QGroupBox * mpms_gb;
   QCheckBox * mpms_cb[10];

   QPushButton * mpms_pb_all;
   QPushButton * mpms_pb_none;
};

#endif // MPMSELECT_H
