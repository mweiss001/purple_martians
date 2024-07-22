#ifndef MCURRENTSESSIONTIMELINEWIDGET_H
#define MCURRENTSESSIONTIMELINEWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "m_base.h"
#include "mGraphicsViewSessionTimeline.h"


class mCurrentSessionTimelineWidget : public QWidget
{
   Q_OBJECT
   public:
      mCurrentSessionTimelineWidget(QWidget *parent = nullptr);

      qreal mapToPos(qreal val);
      qreal mapToVal(qreal pos);

      void addRect(QDateTime start, QDateTime end, qreal y1, qreal y2, QColor col);
      void addLine(QDateTime pos, qreal y1, qreal y2, QColor col);



   private slots:
      void update();
      void onTimelineClicked(int);


   private:
      QGraphicsScene * scene;
      mGraphicsViewSessionTimeline * view;

      qreal mapInStart;
      qreal mapInEnd;

      qreal mapOutStart;
      qreal mapOutEnd;

};

#endif // MCURRENTSESSIONTIMELINEWIDGET_H
