#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>

class CustomGraphicsView : public QGraphicsView
{
   Q_OBJECT
   public:
   CustomGraphicsView(QWidget* parent = nullptr);

   protected:
   virtual void wheelEvent(QWheelEvent* event) override;
   virtual void mouseMoveEvent(QMouseEvent* event) override;
   virtual void mousePressEvent(QMouseEvent* event) override;
   virtual void mouseReleaseEvent(QMouseEvent* event) override;

   void autocomputeSceneSize(const QList<QRectF>& region);
};

#endif  // CUSTOMGRAPHICSVIEW_H

