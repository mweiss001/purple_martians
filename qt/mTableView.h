#ifndef MTABLEVIEW_H
#define MTABLEVIEW_H

#include <QTableView>
#include <QHeaderView>
#include <QPainter>
#include <QGraphicsItem>

#include <QMouseEvent>


#include "m_base.h"
#include "mTableViewDelegate.h"


class mTableView : public QTableView
{
   public:
      mTableView()
      {
         this->setItemDelegate(new mTableViewDelegate);

         this->horizontalHeader()->setStretchLastSection(true);
         this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
         this->horizontalHeader()->setResizeContentsPrecision(0); // only look at visible area

         this->verticalHeader()->setVisible(false);
         this->verticalHeader()->setSectionResizeMode( QHeaderView::Fixed );

         this->setSelectionBehavior(QAbstractItemView::SelectRows);
         this->setSelectionMode(QAbstractItemView::QAbstractItemView::SingleSelection);
      }

  protected:
      void paintEvent(QPaintEvent *event)
      {
//         qDebug() << "mTableView::paintEvent(QPaintEvent *event)";

         // call the main handler for this event
         QTableView::paintEvent(event);

         // draw a frame to highlight the selection
         QPainter painter(viewport());
         painter.setBrush(QColor(0, 0, 0, 0));

         QPen pen(mbase.globalPositionColor);
         pen.setWidthF(1.8);
         painter.setPen(pen);



         // get first item from list of selected items
         QModelIndexList selection = this->selectionModel()->selectedRows();
         //qDebug() << "selection.size()" << selection.size();
         if (selection.size())
         {
            // find first non hidden column
            int col = -1;
            for (int i=9; i>-1; i--)
               if (mbase.col_types[i].valid)
                  if (mbase.col_types[i].shown) col = i;

            if (col != -1)
            {
               QModelIndex index = selection.at(0).siblingAtColumn(col);
               QRect rgr = visualRect(index);
               rgr.setWidth(this->viewport()->width() - 1);
               QPainterPath path;
               path.addRect(rgr);
               painter.drawPath(path);
            }
         }
      }

      void mousePressEvent(QMouseEvent *event)
      {
         // call the main handler for this event
         QTableView::mousePressEvent(event);

         // set global cursor position
         if (event->button() == Qt::MouseButton::LeftButton)
         {
            // get first item from list of selected items
            QModelIndexList selection = this->selectionModel()->selectedRows();
            if (selection.size())
            {
               QModelIndex qmi = selection.at(0).siblingAtColumn(3);
               QVariant v = qmi.model()->data(qmi, Qt::DisplayRole);
               mbase.updateGlobalPosition(v.toDateTime());
            }
         }
      }

















  private:
};
#endif // MTABLEVIEW_H






































































































/*

         QPainterPath path;


         QRect pa2(0, 0, w, h);

         path.addRect(pa2);
         painter.setPen(QColor(255, 0, 0));
         painter.drawPath(path);

         path.clear();
         pa2.adjust(2, 2, -2, -2);
         path.addRect(pa2);

         painter.setPen(QColor(255, 255, 0));
         painter.drawPath(path);

         path.clear();
         pa2.adjust(24, 24, -24, -24);
         path.addRoundedRect(pa2, 5, 5);

         path.moveTo(100, pa2.top());
         path.lineTo(100, pa2.bottom());
         path.moveTo(pa2.left(), 200);
         path.lineTo(pa2.right(), 200);

         painter.setPen(QColor(255, 0, 255));
         painter.drawPath(path);
*/
/*
         QRectF pa(100, 100, 200, 200);
         path.clear();
         path.addRoundedRect(pa, 5, 5);

         painter.setPen(QColor(255, 255, 255));
         painter.setBrush(QColor(100, 100, 100));

         painter.drawPath(path);
*/

// // get first item from list of selected items
// QModelIndexList selection = this->selectionModel()->selectedRows();
// if (selection.size())
// {
//    QModelIndex index = selection.at(0);
//    qDebug() << index;

//    QRect rgr = visualRect(index);

//    rgr.setWidth(w);

//    qDebug() << rgr;

//    QPainterPath p;
//    p.addRect(rgr);
//    painter.setBrush(QColor(0, 0, 0, 0));
//    painter.setPen(QColor(255, 255, 255));
//    painter.drawPath(p);


// }




/*

      mTableViewOverlayInstance = new mTableViewOverlay(this);
      mTableViewOverlayInstance->setZValue(100); // to draw on top
      mTableViewOverlayInstance->setVisible(true);

      // if no scene has been set, set scene
      if (!mTableViewOverlayInstance->scene()) scene()->addItem(mTableViewOverlayInstance);

}


protected:

void paintEvent(QPaintEvent *event)
{
   Q_UNUSED(event);


   QTableView::paintEvent(event); // draw original content

   QPainter painter(this->viewport());
   painter.drawRect(0, 0, 20, 20);


   painter.setBrush(QColor(255, 255, 255));



   QRectF pa(100, 100, 200, 200);
   QPainterPath path;

   path.addRoundedRect(pa, 5, 5);

   painter.drawPath(path);




}







       void paint(QPainter * painter)
       {

          painter->setBrush(QColor(255, 255, 255));

         QRectF pa(100, 100, 200, 200);
         QPainterPath path;

         path.addRoundedRect(pa, 5, 5);

         painter->drawPath(path);


       }





















       //based on the implementation of QLabel::paintEvent
       void PixmapLabel::paintEvent(QPaintEvent *event){
          //if this is assigned to a pixmap
          if(pixmap() && !pixmap()->isNull()){
             QStyle* style= PixmapLabel::style();
             QPainter painter(this);
             drawFrame(&painter);
             QRect cr = contentsRect();
             cr.adjust(margin(), margin(), -margin(), -margin());
             int align= QStyle::visualAlignment(layoutDirection(), alignment());
             QPixmap pix;
             if(hasScaledContents()){ //if scaling is enabled
                QSize scaledSize= cr.size() * devicePixelRatioF();
                //if scaledPixmap is invalid
                if(scaledPixmap.isNull() || scaledPixmap.size()!=scaledSize
                    || pixmap()->cacheKey()!=cacheKey){
                   //if cachedImage is also invalid
                   if(pixmap()->cacheKey() != cacheKey){
                      //reconstruct cachedImage
                      cachedImage= pixmap()->toImage();
                   }
                   QImage scaledImage= cachedImage.scaled(
                       scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                   scaledPixmap= QPixmap::fromImage(scaledImage);
                   scaledPixmap.setDevicePixelRatio(devicePixelRatioF());
                }
                pix= scaledPixmap;
             } else { // no scaling, Just use pixmap()
                pix= *pixmap();
             }
             QStyleOption opt;
             opt.initFrom(this);
             if(!isEnabled())
                pix= style->generatedIconPixmap(QIcon::Disabled, pix, &opt);
             style->drawItemPixmap(&painter, cr, align, pix);
          } else { //otherwise (if the label is not assigned to a pixmap)
             //call base paintEvent
             QLabel::paintEvent(event);
          }
       }






       void QWidgetDerivedWhatchamit::paintEvent(QPaintEvent *)
       {
          QStyleOption opt;
          opt.init(this);
          QPainter p(this);
          style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
       }




//        void paintEvent(QPaintEvent *event)
// {
//    Q_UNUSED(event);

//      QTableView::paintEvent(event);


//      QPainter painter(this);


// QStyleOption opt;
// opt.initFrom(this);


      painter.setBrush(QColor(255, 255, 255));

      QRectF pa(100, 100, 200, 200);
      QPainterPath path;

      path.addRoundedRect(pa, 5, 5);

      painter.drawPath(path);





//      style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

//      style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);


//      style->drawItemPixmap(&painter, cr, align, pix);

//      drawFrame(&painter);






      painter->setBrush(QColor(255, 255, 255));



      QRectF pa(100, 100, 200, 200);
      QPainterPath path;

      path.addRoundedRect(pa, 5, 5);

      painter->drawPath(path);



   path.moveTo(mousePos.x(), pa.top());
   path.lineTo(mousePos.x(), pa.bottom());
   path.moveTo(pa.left(), mousePos.y());
   path.lineTo(pa.right(), mousePos.y());
   path.addRoundedRect(XAxisRect, 5, 5);
   path.addRoundedRect(YAxisRect, 5, 5);
   painter->drawPath(path);

   painter->drawText(XAxisTextRect, XAxisText);
   painter->drawText(YAxisTextRect, YAxisText);















private:
mTableViewOverlay * mTableViewOverlayInstance = nullptr;



};

#endif // MTABLEVIEW_H
*/
