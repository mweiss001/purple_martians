#include "mChartViewCursor.h"


QRectF mChartViewCursor::boundingRect() const
{
   return m_chart->geometry();
}


void mChartViewCursor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   // draw a vertical line to indicate the global cursor position


   // x val of global cursor position
   QPointF val(mbase.globalPosition.toMSecsSinceEpoch(), 0);

   // get the first series chart from a list or chart series
   QList qls = m_chart->series();

   // translate to screen pos
   QPointF pos = m_chart->mapToPosition(val, qls.at(0));

   QPen pen(QColor(255, 255, 255));

   pen.setWidthF(1.8);

   painter->setPen(pen);

   QRectF pa = m_chart->plotArea();
   painter->drawLine(pos.x(), pa.top(), pos.x(), pa.bottom());

}


