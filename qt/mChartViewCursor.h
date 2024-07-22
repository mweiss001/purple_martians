#ifndef MCHARTVIEWCURSOR_H
#define MCHARTVIEWCURSOR_H

#include <QChart>
#include "m_base.h"


class mChartViewCursor : public QGraphicsItem
{
   public:
      mChartViewCursor(QChart *chart) : QGraphicsItem(chart) , m_chart(chart) { }
      QRectF boundingRect() const override;
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
   private:
      QChart *m_chart = nullptr;
};

#endif // MCHARTVIEWCURSOR_H
