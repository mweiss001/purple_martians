#include "mTableViewOverlay.h"



QRectF mTableViewOverlay::boundingRect() const
{

   QRectF pa(0, 0, 1000, 1000);
   return pa;

   //   return table->geometry();
}




void mTableViewOverlay::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

   painter->setBrush(QColor(255, 255, 255));



   QRectF pa(100, 100, 200, 200);
   QPainterPath path;

   path.addRoundedRect(pa, 5, 5);

   painter->drawPath(path);


/*
   path.moveTo(mousePos.x(), pa.top());
   path.lineTo(mousePos.x(), pa.bottom());
   path.moveTo(pa.left(), mousePos.y());
   path.lineTo(pa.right(), mousePos.y());
   path.addRoundedRect(XAxisRect, 5, 5);
   path.addRoundedRect(YAxisRect, 5, 5);
   painter->drawPath(path);

   painter->drawText(XAxisTextRect, XAxisText);
   painter->drawText(YAxisTextRect, YAxisText);

*/

}
