#include "mGraphicsViewZoom.h"

#include <QMouseEvent>
#include <QApplication>
#include <QScrollBar>
#include <qmath.h>



mGraphicsViewZoom::mGraphicsViewZoom(QGraphicsView* view)
    : QObject(view), _view(view)
{
   _view->viewport()->installEventFilter(this);
   _view->setMouseTracking(true);
   _modifiers = Qt::ControlModifier;
   _zoom_factor_base = 1.0015;
}

void mGraphicsViewZoom::gentle_zoom(double factor)
{
   _view->scale(factor, 1.0);
   _view->centerOn(target_scene_pos);
   QPointF delta_viewport_pos = target_viewport_pos - QPointF(_view->viewport()->width() / 2.0,
                                                              _view->viewport()->height() / 1.0);
   QPointF viewport_center = _view->mapFromScene(target_scene_pos) - delta_viewport_pos;
   _view->centerOn(_view->mapToScene(viewport_center.toPoint()));
   emit zoomed();
}

void mGraphicsViewZoom::set_modifiers(Qt::KeyboardModifiers modifiers)
{
   _modifiers = modifiers;
}

void mGraphicsViewZoom::set_zoom_factor_base(double value)
{
   _zoom_factor_base = value;
}

bool mGraphicsViewZoom::eventFilter(QObject *object, QEvent *event)
{
   if (event->type() == QEvent::MouseMove)
   {
      // qDebug() << "QEvent::MouseMove";

      QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
      QPointF delta = target_viewport_pos - mouse_event->pos();
      if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5)
      {
         target_viewport_pos = mouse_event->pos();
         target_scene_pos = _view->mapToScene(mouse_event->pos());
      }
   }
   else if (event->type() == QEvent::Wheel)
   {
      // qDebug() << "QEvent::Wheel";
      QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
      if (QApplication::keyboardModifiers() == _modifiers)
      {
//         if (wheel_event->orientation() == Qt::Vertical)
         {
            double angle = wheel_event->angleDelta().y();
            double factor = qPow(_zoom_factor_base, angle);
            gentle_zoom(factor);
            return true;
         }
      }
   }
   Q_UNUSED(object)
   return false;
}









