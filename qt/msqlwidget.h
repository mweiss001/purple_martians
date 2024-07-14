#ifndef MSQLWIDGET_H
#define MSQLWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QTextBrowser>

#include "m_base.h"

class MSQLWidget : public QWidget
{
   Q_OBJECT
public:
   explicit MSQLWidget(QWidget *parent = nullptr);

   int VSIZE = 600;
   int HSIZE = 500;
   QSize minimumSizeHint() const  {      return QSize(VSIZE, HSIZE);   }
   QSize minimumSize () const     {      return QSize(VSIZE, HSIZE);   }

   void update();

signals:

private:
   QGroupBox * msqw_gb;
   QGroupBox * msqw1_gb;
   QGroupBox * msqw2_gb;
   QGroupBox * msqw3_gb;
   QTextBrowser * msqw1_tb;
   QTextBrowser * msqw2_tb;
   QTextBrowser * msqw3_tb;
};

#endif // MSQLWIDGET_H
