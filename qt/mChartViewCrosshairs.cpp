#include "mChartViewCrosshairs.h"

mChartViewCrosshairs::mChartViewCrosshairs(QChart *chart) : QGraphicsItem(chart) , m_chart(chart) { }

QRectF mChartViewCrosshairs::boundingRect() const
{
   return m_chart->geometry();
}

void mChartViewCrosshairs::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//   painter->setBrush(QColor(255, 255, 255));

   painter->setBrush(QColor(100, 100, 100));


   QRectF pa = m_chart->plotArea();
   QPainterPath path;

   path.moveTo(mousePos.x(), pa.top());
   path.lineTo(mousePos.x(), pa.bottom());
   path.moveTo(pa.left(), mousePos.y());
   path.lineTo(pa.right(), mousePos.y());
   path.addRoundedRect(XAxisRect, 5, 5);
   path.addRoundedRect(YAxisRect, 5, 5);
   painter->drawPath(path);

   painter->drawText(XAxisTextRect, XAxisText);
   painter->drawText(YAxisTextRect, YAxisText);
}

void mChartViewCrosshairs::setXAxisText()
{
   if (mbase.mChartsWidgetXAxisFrame) XAxisText.setNum((int)XAxisVal);
   else XAxisText = QDateTime::fromMSecsSinceEpoch(XAxisVal).toString("mm:ss.zzz");

   XAxisTextPos = QPointF(mousePos.x(), m_chart->plotArea().bottom()+1);

   QFontMetrics metrics(m_font);
   XAxisTextRect = metrics.boundingRect(QRect(0, 0, 150, 150), Qt::AlignLeft, XAxisText);

   // add 5, 4
   XAxisTextRect.translate(5, 2);

   // make a slighly bigger rect to hold it
   // this will be the size of the rounded rectangle
   XAxisRect = XAxisTextRect.adjusted(-5, -2, 5, 2);

   // center horizontally by subtracting width/2
   XAxisRect.translate(-XAxisRect.width()/2, 0);

   // translate to the display position
   XAxisRect.translate(XAxisTextPos);

   // translate the text position back relative to outer frame
   XAxisTextRect = XAxisRect.adjusted(5, 2, -5, -2);

   prepareGeometryChange();
   m_chart->update();
}


void mChartViewCrosshairs::setYAxisText()
{
   YAxisText.setNum(YAxisVal, 'g');
   YAxisTextPos = QPointF(m_chart->plotArea().left(), mousePos.y());

   // get the text bounding rect
   QFontMetrics metrics(m_font);
   YAxisTextRect = metrics.boundingRect(QRect(0, 0, 150, 150), Qt::AlignLeft, YAxisText);

   // add 5,2
   YAxisTextRect.translate(5, 2);

   // make a slighly bigger rect to hold it
   // this will be the size of the rounded rectangle
   YAxisRect = YAxisTextRect.adjusted(-5, -2, 5, 2);

   // center vertically by subtracting height/2
   YAxisRect.translate(0, -YAxisRect.height()/2);

   // translate to the display position
   YAxisRect.translate(YAxisTextPos);

   // align to right edge by subtracting width
   YAxisRect.translate(-YAxisRect.width(), 0);

   // translate the text position back relative to outer frame
   YAxisTextRect = YAxisRect.adjusted(5, 2, -5, -2);

   prepareGeometryChange();
   m_chart->update();
}

void mChartViewCrosshairs::setMousePos(QPointF pos)
{
   mousePos = pos;
   // map mouse position to chart values
   QPointF val = m_chart->mapToValue(pos);




   XAxisVal = val.x();
   YAxisVal = val.y();

   setXAxisText();
   setYAxisText();
}
