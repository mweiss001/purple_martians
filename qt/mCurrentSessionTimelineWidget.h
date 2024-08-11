#ifndef MCURRENTSESSIONTIMELINEWIDGET_H
#define MCURRENTSESSIONTIMELINEWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QBoxLayout>
#include <QFormLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include "m_base.h"
#include "mGraphicsViewSessionTimeline.h"

class mCurrentSessionTimelineWidget : public QWidget
{
   Q_OBJECT
   public:
      mCurrentSessionTimelineWidget(QWidget *parent = nullptr);
      void createLabelHelper(QHBoxLayout * rll, QString title, QLabel* start, QLabel* end, QLabel* range, QColor col);
   private slots:
      void updateUI();
      void onTimelineClicked(int);

   private:
      QGraphicsScene * scene;
      mGraphicsViewSessionTimeline * view;
      qreal timelineHeight = 40;
      qreal mapInStart, mapInEnd, mapOutStart, mapOutEnd;
      qreal mapToPos(qreal val) { return (val - mapInStart) / (mapInEnd - mapInStart) * (mapOutEnd - mapOutStart) + mapOutStart; }
      qreal mapToVal(qreal pos) { return (pos - mapOutStart) / (mapOutEnd - mapOutStart) * (mapInEnd - mapInStart) + mapInStart; }

      void addRect(QDateTime start, QDateTime end, qreal y1, qreal y2, QColor col)
      {
         qreal ds = mapToPos(start.toMSecsSinceEpoch());
         qreal de = mapToPos(end.toMSecsSinceEpoch());
         QRectF r(QPointF(ds, y1), QPointF(de, y2));
         scene->addRect(r, QPen(col));
      }

      void addLine(QDateTime pos, qreal y1, qreal y2, QColor col)
      {
         qreal dp = mapToPos(pos.toMSecsSinceEpoch());
         QRectF r(QPointF(dp, y1), QPointF(dp, y2));
         scene->addRect(r, QPen(col));
      }

      QLabel * labelSessionStart = new QLabel;
      QLabel * labelSessionEnd = new QLabel;
      QLabel * labelSessionRange = new QLabel;

      QLabel * labelTableModelStart = new QLabel;
      QLabel * labelTableModelEnd = new QLabel;
      QLabel * labelTableModelRange = new QLabel;

      QLabel * labelChartModelStart = new QLabel;
      QLabel * labelChartModelEnd = new QLabel;
      QLabel * labelChartModelRange = new QLabel;

      QLabel * labelCurrentPosition = new QLabel;
};
#endif // MCURRENTSESSIONTIMELINEWIDGET_H
