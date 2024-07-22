#ifndef MGRAPHICSSCENE_H
#define MGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>


#include <QPoint>
#include <QMouseEvent>

class mGraphicsScene : public QGraphicsScene
{
   Q_OBJECT
   public:
   explicit mGraphicsScene(QObject *parent = 0) {}

   protected:

/*


       void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
       {
         qDebug() << "void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)";

//         QGraphicsScene::mousePressEvent(mouseEvent);
//         mouseEvent->setAccepted(false);

         if (mouseEvent->button() == Qt::LeftButton)
         {

            qDebug() << "(mouseEvent->button() == Qt::LeftButton)";

            QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());

            qDebug() << "item at:" << item;

            QString dta = item->data(0).toString();

            qDebug() << "data" << dta;




         }
       }
*/



};

#endif // MGRAPHICSSCENE_H












