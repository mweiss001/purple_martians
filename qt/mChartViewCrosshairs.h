#ifndef MCHARTVIEWCROSSHAIRS_H
#define MCHARTVIEWCROSSHAIRS_H

#include <QChart>
#include <QPainter>
#include <QFont>
#include <QGraphicsItem>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QLineSeries>

#include "m_base.h"

class mChartViewCrosshairs : public QGraphicsItem
{
   public:
      mChartViewCrosshairs(QChart *chart) : QGraphicsItem(chart), m_chart(chart) { }
      QRectF boundingRect() const override { return m_chart->geometry(); }

      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
      {
         // QColor p(0, 0, 0);
         // QColor b(255, 255, 255);

         QColor b(0, 0, 0);
         QColor p(255, 255, 255);



         painter->setPen(p);
         painter->setBrush(b);

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

      void setMousePos(QPointF pos)
      {
         mousePos = pos;
         // map mouse position to chart values
         QPointF val = m_chart->mapToValue(pos);

         XAxisVal = val.x();
         YAxisVal = val.y();

         setXAxisText();
         setYAxisText();
      }

   private:


      void setXAxisText()
      {
         XAxisText = QDateTime::fromMSecsSinceEpoch(XAxisVal).toString("mm:ss.zzz");

         QFontMetrics metrics(m_font);
         XAxisTextRect = metrics.boundingRect(QRect(0, 0, 150, 150), Qt::AlignLeft, XAxisText);

         // add 5, 4
         XAxisTextRect.translate(5, 2);

         // make a slighly bigger rect to hold it
         // this will be the size of the rounded rectangle
         XAxisRect = XAxisTextRect.adjusted(-5, -2, 5, 2);

         // center horizontally by subtracting width/2
         XAxisRect.translate(-XAxisRect.width()/2, 0);

         // align y position with bottom of geometry - height
         XAxisTextPos = QPointF(mousePos.x(), m_chart->geometry().bottom() - XAxisRect.height());

         // translate to the display position
         XAxisRect.translate(XAxisTextPos);

         // translate the text position back relative to outer frame
         XAxisTextRect = XAxisRect.adjusted(5, 2, -5, -2);

         prepareGeometryChange();
         m_chart->update();
      }


      void setYAxisText()
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

      QFont m_font;
      QChart *m_chart = nullptr;

      QPointF mousePos;

      double XAxisVal, YAxisVal;
      QString XAxisText, YAxisText;
      QRectF XAxisTextRect, YAxisTextRect, XAxisRect, YAxisRect;
      QPointF XAxisTextPos, YAxisTextPos;
};

#endif // MCHARTVIEWCROSSHAIRS_H
