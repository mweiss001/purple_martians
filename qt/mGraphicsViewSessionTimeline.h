#ifndef MGRAPHICSVIEWSESSIONTIMELINE_H
#define MGRAPHICSVIEWSESSIONTIMELINE_H

#include <QGraphicsView>
#include <QMouseEvent>

class mGraphicsViewSessionTimeline : public QGraphicsView
{
   Q_OBJECT
   public:
      mGraphicsViewSessionTimeline(QWidget* parent = nullptr) { }

   protected:
      virtual void mousePressEvent(QMouseEvent* event) override
      {
         emit timelineClicked(event->pos().rx());
      }

   signals:
      void timelineClicked(int xpos);

};
#endif // MGRAPHICSVIEWSESSIONTIMELINE_H
