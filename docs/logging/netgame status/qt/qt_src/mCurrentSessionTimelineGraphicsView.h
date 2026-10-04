#ifndef MCURRENTSESSIONTIMELINEGRAPHICSVIEW_H
#define MCURRENTSESSIONTIMELINEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class mCurrentSessionTimelineGraphicsView : public QGraphicsView
{
   Q_OBJECT
   public:
   mCurrentSessionTimelineGraphicsView(QWidget* parent = nullptr) { }

   protected:
   virtual void mousePressEvent(QMouseEvent* event) override  { emit timelineClicked(event->pos().rx());  }

   signals:
   void timelineClicked(int xpos);
};
#endif // MCURRENTSESSIONTIMELINEGRAPHICSVIEW_H
