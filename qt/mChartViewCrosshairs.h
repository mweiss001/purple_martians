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

      QRectF boundingRect() const override;
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
      void setMousePos(QPointF pos);

   protected:

   private:

      void setXAxisText();
      void setYAxisText();

      QFont m_font;
      QChart *m_chart = nullptr;

      QPointF mousePos;

      double YAxisVal;
      double XAxisVal;

      QString XAxisText;
      QString YAxisText;


      QRectF XAxisTextRect;
      QRectF XAxisRect;
      QPointF XAxisTextPos;

      QRectF YAxisTextRect;
      QRectF YAxisRect;
      QPointF YAxisTextPos;
};

#endif // MCHARTVIEWCROSSHAIRS_H
