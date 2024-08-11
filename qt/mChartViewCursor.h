#ifndef MCHARTVIEWCURSOR_H
#define MCHARTVIEWCURSOR_H

#include <QChart>
#include "m_base.h"

class mChartViewCursor : public QGraphicsItem
{
   public:
      mChartViewCursor(QChart *chart) : QGraphicsItem(chart) , m_chart(chart) { }
      QRectF boundingRect() const override { return m_chart->geometry(); }
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
      {
         // draw a vertical line to indicate the global cursor position

         // x val of global cursor position
         QPointF val(mbase.globalPosition.toMSecsSinceEpoch(), 0);

         // get the first series chart from a list or chart series
         QList qls = m_chart->series();

         // translate to screen pos
         QPointF pos = m_chart->mapToPosition(val, qls.at(0));

         QPen pen(mbase.globalPositionColor);
         pen.setWidthF(1.8);
         painter->setPen(pen);

         QRectF pa = m_chart->plotArea();
         painter->drawLine(pos.x(), pa.top(), pos.x(), pa.bottom());
      }

   private:
      QChart *m_chart = nullptr;
};

#endif // MCHARTVIEWCURSOR_H
