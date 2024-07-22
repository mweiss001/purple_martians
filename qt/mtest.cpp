#include "mtest.h"

mTest::mTest()
{
   QVBoxLayout *vbox = new QVBoxLayout;

   QGroupBox * gb = new QGroupBox("Test", this);

   gb->setGeometry(10, 10, VSIZE-20, HSIZE-20);
   gb->setLayout(vbox);


   scene = new mGraphicsScene();

   scene->addText("Hello, world!");



   QPen pen(QColor(255, 0, 0));
   pen.setWidthF(1.8);


   QRectF rf(20, 20, 80, 60);


   scene->addRect(rf, pen);


   view = new QGraphicsView(scene);

   view->show();

   vbox->addWidget(view);

}
