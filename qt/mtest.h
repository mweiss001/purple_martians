#ifndef MTEST_H
#define MTEST_H

#include <QWidget>

#include <QGroupBox>
#include <QBoxLayout>

#include <QGraphicsScene>
#include <QGraphicsView>

#include "mGraphicsScene.h"


class mTest : public QWidget
{
   Q_OBJECT
   public:

      mTest();


      int VSIZE = 200;
      int HSIZE = 440;
      QSize minimumSizeHint() const  {  return QSize(VSIZE, HSIZE);   }
      QSize minimumSize () const     {  return QSize(VSIZE, HSIZE);   }


   private:

      mGraphicsScene * scene;

      QGraphicsView * view;



};

#endif // MTEST_H
