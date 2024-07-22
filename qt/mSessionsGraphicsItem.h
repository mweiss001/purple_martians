#ifndef MSESSIONSGRAPHICSITEM_H
#define MSESSIONSGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>


class mSessionsGraphicsItem : public QGraphicsItem
{
   public:
      mSessionsGraphicsItem(int p_index, QRectF p_rect, QString p_txt, QColor p_col)
      {
         index = p_index;
         rect = p_rect;
         txt = p_txt;
         col = p_col;
         this->setFlag(QGraphicsItem::ItemIsSelectable, true);
      }

      QRectF boundingRect() const override  {  return rect; }

      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
      {
         // frame
         QPen framePen(col);
         framePen.setWidthF(1.8);
         if (this->isSelected()) framePen.setWidthF(3.8);
         painter->setPen(framePen);
         painter->drawRect(rect);

         // text
         framePen.setColor(Qt::black);
         painter->setPen(framePen);
         painter->drawText(rect.adjusted(2, 0, 0, 0), txt);
      }

/*
      void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override
      {
         if (mouseEvent->button() == Qt::LeftButton)
         {
            qDebug() << "left mouse button pressed on:" << index << txt;
         }
      }
*/


      int index;
      QRectF rect;
      QString txt;
      QColor col;

};
#endif // MSESSIONSGRAPHICSITEM_H
