#ifndef MCHARTVIEW_H
#define MCHARTVIEW_H

#include <QChartView>
#include <QMenu>
#include <QValueAxis>

#include "m_base.h"
#include "mChartViewCrosshairs.h"
#include "mChartViewCursor.h"

class mChartView : public QChartView
{
   Q_OBJECT
   public:

      mChartView(QChart* chart = 0, QWidget* parent = 0, int index = 0) : QChartView(chart, parent)
      {
         // qDebug() << "mChartView(QChart*) constructor";

         series_index = index;

         mChartViewCrosshairsInstance = new mChartViewCrosshairs(chart);
         mChartViewCrosshairsInstance->setZValue(100); // to draw on top
         mChartViewCrosshairsInstance->setVisible(false);

         // if no scene has been set, set scene
         if (!mChartViewCrosshairsInstance->scene()) scene()->addItem(mChartViewCrosshairsInstance);

         mChartViewCursorInstance = new mChartViewCursor(chart);
         mChartViewCursorInstance->setZValue(100); // to draw on top

         // if no scene has been set, set scene
         if (!mChartViewCursorInstance->scene()) scene()->addItem(mChartViewCursorInstance);

         this->setRenderHint(QPainter::Antialiasing);

         setContextMenuPolicy(Qt::CustomContextMenu);
         connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextMenu(QPoint)));
      }

   protected:



       void mousePressEvent(QMouseEvent *event)
       {
          // call the main handler for this event
          QChartView::mousePressEvent(event);
          if (event->button() == Qt::MouseButton::LeftButton)
          {
             // set global cursor position
             QPointF val = chart()->mapToValue(event->position());
             mbase.updateGlobalPosition(QDateTime::fromMSecsSinceEpoch(val.x()));
          }
       }


      void leaveEvent(QEvent *event)
      {
         //qDebug() << "leaveEvent";
         mChartViewCrosshairsInstance->setVisible(false);
      }



      void mouseMoveEvent(QMouseEvent *event)
      {
         // detect if on plot area
         QRectF pa = chart()->plotArea();
         if (pa.contains(event->position()))
         {
            mChartViewCrosshairsInstance->setVisible(true);
            mChartViewCrosshairsInstance->setMousePos(event->position());
            event->accept();
         }
         else mChartViewCrosshairsInstance->setVisible(false);
      }

      void keyPressEvent(QKeyEvent *event)
      {
         switch (event->key())
         {
   //         case Qt::Key_Plus:   chart()->zoomIn();      break;
   //         case Qt::Key_Minus:  chart()->zoomOut();     break;
   //         case Qt::Key_R:      chart()->zoomReset();   break;
         }
      }



      void wheelEvent(QWheelEvent *event)
      {
          // get first x axis axis of chart
         QList qlxa = this->chart()->axes(Qt::Horizontal);

         // cast to QValueAxis to get min and max
         const auto va = static_cast<QValueAxis*>(qlxa.at(0));

         // find min, max and range
         double min = va->min();
         double max = va->max();
         double rng = max - min;

         // --- scroll in x axis with SHIFT ---------------
         if (event->modifiers() & Qt::ShiftModifier)
         {
            QPoint numDegrees = event->angleDelta() / 8;
            int y = numDegrees.ry();
            this->chart()->scroll(y, 0);
         }
         // --- zoom in x axis centered on mouse pos -------
         else
         {
            // get amount and direction of mouse wheel scroll
            int mouseScroll = event->angleDelta().ry();

            // get mouse position
            QPointF mousePos = event->position();

            // get value at position
            QPointF val = chart()->mapToValue(mousePos);

            // get 10% of range with mouseScroll sign
            double rng_inc = rng * 0.1;
            if (mouseScroll > 0) rng_inc = rng * -0.1;

            // adjust axis min and max with rng_inc
            va->setRange(min+rng_inc, max-rng_inc);

            // find the new screen position of the original value
            QPointF newpos = chart()->mapToPosition(val);

            // scroll the chart by the difference between newpos and original mouse position
            this->chart()->scroll(newpos.x() - mousePos.x(), 0);

            // update the chart as if the mouse moved
            mChartViewCrosshairsInstance->setMousePos(mousePos);
         }

         // set new min and max
         mbase.mChartsWidgetXAxisMin = va->min();
         mbase.mChartsWidgetXAxisMax = va->max();
         mbase.mChartsWidgetControlsChangedFunction();
      }

   private slots:

      void hideThisChart()
      {
         qDebug("Hide This Chart");
         mbase.statChartGraphTypeArray[series_index].visible = 0;
         mbase.mChartsWidgetControlsChangedFunction();
      }

      void ShowContextMenu(const QPoint &pos)
      {
         QMenu contextMenu(this);

         QAction action2("Hide This Chart", this);
         connect(&action2, SIGNAL(triggered()), this, SLOT(hideThisChart()));
         contextMenu.addAction(&action2);

         contextMenu.exec(mapToGlobal(pos));
      }

   private:
      mChartViewCrosshairs * mChartViewCrosshairsInstance = nullptr;
      mChartViewCursor * mChartViewCursorInstance = nullptr;
      QChart * m_chart = nullptr;
      int series_index;
};
#endif // MCHARTVIEW_H


















/*


   // this scrolls x
   void wheelEvent(QWheelEvent *event)
   {
      QPoint numDegrees = event->angleDelta() / 8;
      int y = numDegrees.ry();
      this->chart()->scroll(y, 0);
   }










   // this does 2 things
   void wheelEvent(QWheelEvent *event)
   {
      if( event->modifiers() & Qt::ShiftModifier )
      {
         QPoint numDegrees = event->angleDelta() / 8;
         int y = numDegrees.ry();
         this->chart()->scroll(y, 0);
      }
      else
      {
         QPoint numDegrees = event->angleDelta() / 8;
         int y = numDegrees.ry();

         qreal factor;
         if ( y > 0 ) factor = 2.0;
         else factor = 0.5;

         QRectF r = QRectF(this->chart()->plotArea().left(),this->chart()->plotArea().top(),
                           this->chart()->plotArea().width()/factor,this->chart()->plotArea().height());

         QPointF mousePos = mapFromGlobal(QCursor::pos());
         r.moveCenter(mousePos);
         this->chart()->zoomIn(r);

         QPointF delta = this->chart()->plotArea().center() -mousePos;
         this->chart()->scroll(delta.x(), 0);
      }
   }


*/

























/*

   void wheelEvent(QWheelEvent *event)
   {

      QPoint numDegrees = event->angleDelta() / 8;
      int y = numDegrees.ry();

      qDebug() << "num deg:" << y;

      scrollContentsBy(0, y);


      // QPoint numPixels = event->pixelDelta();
      // QPoint numDegrees = event->angleDelta() / 8;

      // if (!numPixels.isNull())
      // {

      //    qDebug() << "num pixels:" << numPixels;
      //    //         scrollWithPixels(numPixels);


      // } else if (!numDegrees.isNull())
      // {
      //    qDebug() << "num deg:" << numDegrees;


      //    // QPoint numSteps = numDegrees / 15;
      //    // qDebug() << "num Steps:" << numPixels;

      //    scrollContentsBy(0, (int) numDegrees);


      //    //         scrollWithDegrees(numSteps);
      // }

      event->accept();
   }

  */


/*
// this works and does .5 and 2.0 zoom in both x and y
   void wheelEvent(QWheelEvent *event)
   {
      qreal factor;

      QPoint numDegrees = event->angleDelta() / 8;
      int y = numDegrees.ry();

      if ( y > 0 ) factor = 2.0;
      else factor = 0.5;

      QRectF r = QRectF(this->chart()->plotArea().left(),this->chart()->plotArea().top(),
                        this->chart()->plotArea().width()/factor,this->chart()->plotArea().height()/factor);
      QPointF mousePos = mapFromGlobal(QCursor::pos());
      r.moveCenter(mousePos);
      this->chart()->zoomIn(r);
      QPointF delta = this->chart()->plotArea().center() -mousePos;
      this->chart()->scroll(delta.x(),-delta.y());
   }

  */


/*

   // this scrolls x
   void wheelEvent(QWheelEvent *event)
   {
      QPoint numDegrees = event->angleDelta() / 8;
      int y = numDegrees.ry();
      this->chart()->scroll(y, 0);
   }

*/


/*

   // this does 2 things
   void wheelEvent(QWheelEvent *event)
   {
      if( event->modifiers() & Qt::ShiftModifier )
      {
         QPoint numDegrees = event->angleDelta() / 8;
         int y = numDegrees.ry();
         this->chart()->scroll(y, 0);
      }
      else
      {
         QPoint numDegrees = event->angleDelta() / 8;
         int y = numDegrees.ry();

         qreal factor;
         if ( y > 0 ) factor = 2.0;
         else factor = 0.5;

         QRectF r = QRectF(this->chart()->plotArea().left(),this->chart()->plotArea().top(),
                           this->chart()->plotArea().width()/factor,this->chart()->plotArea().height());

         QPointF mousePos = mapFromGlobal(QCursor::pos());
         r.moveCenter(mousePos);
         this->chart()->zoomIn(r);

         QPointF delta = this->chart()->plotArea().center() -mousePos;
         this->chart()->scroll(delta.x(), 0);
      }
   }


  */


       /*

       // try for simpler zoom x
       void wheelEvent(QWheelEvent *event)
       {

          int top = this->chart()->plotArea().top();
          int height = this->chart()->plotArea().height();


//          qDebug() << "1top:" << top << "  height:" << height;


          QPoint numDegrees = event->angleDelta() / 8;
          int y = numDegrees.ry();

          qreal factor;
          if ( y > 0 ) factor = 2.0;
          else factor = 0.5;

          QRectF r = QRectF(this->chart()->plotArea().left(),this->chart()->plotArea().top(),
                            this->chart()->plotArea().width()/factor,this->chart()->plotArea().height());

          QPointF mousePos = mapFromGlobal(QCursor::pos());

          int yc = (this->chart()->plotArea().top() + this->chart()->plotArea().height()) /2;

          QPointF np(mousePos.rx(), yc);

          r.moveCenter(mousePos);


          this->chart()->zoomIn(r);

          QPointF delta = this->chart()->plotArea().center() -mousePos;
          this->chart()->scroll(delta.x(), 0);


          this->chart()->plotArea().setTop(top);
          this->chart()->plotArea().setHeight(height);

//          qDebug() << "2top:" << top << "  height:" << height;

//          float min = this->chart()->axes()max();

//          QList<QAbstractAxis *> 	axes(Qt::Orientations orientation = Qt::Horizontal|Qt::Vertical, QAbstractSeries *series = nullptr) const
//              QBrush 	backgroundBrush() const
//              QPen 	backgroundPen() const






          // iterate all series for this chart

          QList qls = this->chart()->series();
          for (int i = 0; i < qls.size(); ++i)
          {
             qDebug() << i << qls.at(i);
             // when debug prints this I get:
             // 0 QLineSeries(0x1cd1f6ae720)
             // 1 QLineSeries(0x1cd1f6aeee0)
             // 2 QLineSeries(0x1cd1f6af4e0)
             // 3 QLineSeries(0x1cd1f6af420)
             // 4 QLineSeries(0x1cd1f6ada60)
             // 5 QLineSeries(0x1cd1f6adb00)
             // 6 QLineSeries(0x1cd1f6ade20)
             // 7 QLineSeries(0x1cd1f6ad880)
          }


          // // iterate all axes for this chart
          // QList qla = this->chart()->axes(Qt::Horizontal);
          // for (int i = 0; i < qla.size(); ++i)
          // {
          //    qDebug() << i << qla.at(i);
          //    // when debug prints this I get: 0 QDateTimeAxis(0x1efaf7220e0)
          // }

      QList qla = this->chart()->axes(Qt::Horizontal);
      const auto dta = static_cast<const QDateTimeAxis*>(qla.at(0));
      QDateTime dtmn = dta->min();
      QDateTime dtmx = dta->max();
      qDebug() << "min:" << dtmn << "  max:" << dtmx;


*/

/*

      // try manipulating axis directly
      void wheelEvent(QWheelEvent *event)
      {

         QPoint numDegrees = event->angleDelta() / 8;
         int y = numDegrees.ry();


         QList qla = this->chart()->axes(Qt::Horizontal);
         const auto dta = static_cast<QDateTimeAxis*>(qla.at(0));
         QDateTime dtmn = dta->min();
         QDateTime dtmx = dta->max();
         qDebug() << "min:" << dtmn << "  max:" << dtmx;


         qint64 mn = dtmn.toMSecsSinceEpoch();
         qint64 mx = dtmx.toMSecsSinceEpoch();
         qint64 rn = mx - mn;


//         mn += y*10;

         dtmn = dtmn.addMSecs(y*10);
         dtmx = dtmx.addMSecs(y*10);

//         QDateTime dtmn2 = dtmn.addMSecs(y*10);


         // dta->setMin(dtmn);
         // dta->setMax(dtmx);

         dta->setRange(dtmn, dtmx);

   }

  */
/*
   // a very simple x axis scroll
   void wheelEvent(QWheelEvent *event)
   {

      QPoint numDegrees = event->angleDelta() / 8;
      int y = numDegrees.ry();

      QList qla = this->chart()->axes(Qt::Horizontal);
      const auto dta = static_cast<QDateTimeAxis*>(qla.at(0));

      dta->setRange(dta->min().addMSecs(y*10), dta->max().addMSecs(y*10));

      // QDateTime dtmn = dta->min();
      // QDateTime dtmx = dta->max();
      // dta->setRange(dtmn.addMSecs(y*10), dtmx.addMSecs(y*10));

      // dtmn = dtmn.addMSecs(y*10);
      // dtmx = dtmx.addMSecs(y*10);

      // dta->setRange(dtmn, dtmx);

//      dtmn = dtmn.addMSecs(y*10);
//      dtmx = dtmx.addMSecs(y*10);
   }

  */

       /*

       // a very simple x axis zoom while keeping min constant
   void wheelEvent(QWheelEvent *event)
   {

      QList qls = this->chart()->series();
      QAbstractSeries *series = qls.at(0);

      QPoint numDegrees = event->angleDelta() / 8;
      int y = numDegrees.ry();

      int top = this->chart()->plotArea().top();
      int left = this->chart()->plotArea().left();

      QPointF pos = event->position();
      qDebug() << "p posx:" << pos.rx() -left << "  posy:" << pos.ry() - top;

      QPointF vp = this->chart()->mapToPosition(pos, series);
      qDebug() << "v posx:" << vp.rx() << "  posy:" << vp.ry() ;

      QList qla = this->chart()->axes(Qt::Horizontal);
      const auto dta = static_cast<QDateTimeAxis*>(qla.at(0));

      dta->setMax(dta->max().addMSecs(y*10));
   }
*/




/*
   void mouseMoveEvent(QMouseEvent *event)
   {
      QPointF pos = event->position();

      float xp = pos.rx();
      float yp = pos.ry();

      QPointF val = this->chart()->mapToValue(pos);
      double xv = val.rx();
      double yv = val.ry();

      QDateTime dt = QDateTime::fromMSecsSinceEpoch(xv);

      char msg[100];
      sprintf(msg, "xp: %f  yp: %f  xv: %f  yv: %f", xp, yp, xv, yv);
//      qDebug() << msg << dt;



   }

*/


/*





      //      connect(series, &QLineSeries::clicked, this, &CalloutView::keepCallout);

      //      connect(m_chart()->series, &QLineSeries::hovered, this, &CalloutView::tooltip);

      // get series from chart

      const auto sers = chart()->series();
      for (QAbstractSeries * ser : sers)
      {

         QLineSeries * ql = static_cast<QLineSeries*>(ser);



         qDebug() << ql;

         qDebug() << ql->name();

         connect(ql, &QLineSeries::hovered, this, &mChartView::mhov);


//         //      connect(m_chart()->series, &QLineSeries::hovered, this, &CalloutView::tooltip);


      }





      //       QList qla = this->chart()->axes(Qt::Horizontal);
      //       const auto dta = static_cast<QDateTimeAxis*>(qla.at(0));


   }


   public slots:

   void mhov(QPointF point, bool state)
   {
      qDebug() << "hov" << point << state;
   }


*/















