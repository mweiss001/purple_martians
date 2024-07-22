#include "mSessionsTimelineWidget.h"
#include "mGraphicsViewZoom.h"

#include "customgraphicsview.h"



mSessionsTimelineWidget::mSessionsTimelineWidget(QWidget *parent) : QWidget{parent}
{
   scene = new QGraphicsScene();
   connect(scene, SIGNAL (selectionChanged()), this, SLOT (onSelectionChanged()));

   // add rects from session model
   // cycle all rows in model
   for(int i=0; i<mbase.sessionsModel->rowCount(); i++)
   {
      // get dt_start from column 0
      QModelIndex qmi = mbase.sessionsModel->index(i, 0);
      QVariant v = qmi.model()->data(qmi, Qt::DisplayRole);
      QDateTime dts = v.toDateTime();

      // get dt_end from column 1
      qmi = mbase.sessionsModel->index(i, 1);
      v = qmi.model()->data(qmi, Qt::DisplayRole);
      QDateTime dte = v.toDateTime();

      // get player_num from column 3
      qmi = mbase.sessionsModel->index(i, 3);
      v = qmi.model()->data(qmi, Qt::DisplayRole);
      int p = v.toInt();

      // get host from column 4
      qmi = mbase.sessionsModel->index(i, 4);
      v = qmi.model()->data(qmi, Qt::DisplayRole);
      QString host = v.toString();

      // get color from player num
      QColor col = mbase.statChartSeriesStructArray[p].col;

      qreal ds = dts.toSecsSinceEpoch()/100;
      qreal de = dte.toSecsSinceEpoch()/100;

      qreal rectHeight = 20;

      QPointF topLeft(ds, p * rectHeight);
      QPointF bottomRight(de, (p+1) * rectHeight - 4);

      QRectF r(topLeft, bottomRight);

      mSessionsGraphicsItem * temp = new mSessionsGraphicsItem(i, r, host, col);
      scene->addItem(temp);
   }

   // top level hbox
   QHBoxLayout *topHBox = new QHBoxLayout;
   this->setLayout(topHBox);


   QGroupBox * gb = new QGroupBox("Sessions", this);
   topHBox->addWidget(gb);


   QHBoxLayout *hbox = new QHBoxLayout;
   gb->setLayout(hbox);


   view = new CustomGraphicsView(this);
   view->setScene(scene);

//   view = new QGraphicsView(scene, this);

   view->setRenderHint(QPainter::Antialiasing);

   // view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
   // view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
   // view->setDragMode(QGraphicsView::ScrollHandDrag);
   view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
   view->setMinimumHeight(156);
   view->scale(1, 1);

   hbox->addWidget(view);

//   mGraphicsViewZoom * z = new mGraphicsViewZoom(view);
//   z->set_modifiers(Qt::NoModifier);


}
void mSessionsTimelineWidget::onSelectionChanged()
{
/*
   QDateTime dts = QDateTime::fromString("2100", "yyyy"); // set start to max
   QDateTime dte = QDateTime::fromString("1900", "yyyy"); // set end to min

   qDebug() << "dts" << dts;
   qDebug() << "dte" << dte;

   qDebug() << "dts.toSecsSinceEpoch()" << dts.toSecsSinceEpoch();
   qDebug() << "dte.toSecsSinceEpoch()" << dte.toSecsSinceEpoch();

   qint64 dsi = dts.toSecsSinceEpoch();
   qint64 dei = dte.toSecsSinceEpoch();

   qDebug() << "dts_qint64" << dsi;
   qDebug() << "dte_qint64" << dei;

   qreal dsr = dts.toSecsSinceEpoch();
   qreal der = dte.toSecsSinceEpoch();

   qDebug() << "dts_real" << dsr;
   qDebug() << "dte_real" << der;
  */

   QDateTime dt = QDateTime::fromString("2020", "yyyy");
   qDebug() << "dt" << dt;
   qDebug() << "dt.toSecsSinceEpoch()" << dt.toSecsSinceEpoch();
   qint64 ds = dt.toSecsSinceEpoch();
   qDebug() << "ds_qint64" << ds;
   qreal dr = dt.toSecsSinceEpoch();
   qDebug() << "dts_real" << dr;


   QList<QGraphicsItem *> items = scene->selectedItems();
   if (items.count())
   {
      for (int i=0; i<items.count(); i++)
      {
         QRectF br = items[i]->boundingRect();
         QDateTime dts = QDateTime::fromSecsSinceEpoch(br.left());
         QDateTime dte = QDateTime::fromSecsSinceEpoch(br.right());
         if (dts < mbase.sessionsDtStart) mbase.sessionsDtStart = dts;
         if (dte > mbase.sessionsDtEnd)   mbase.sessionsDtEnd = dte;
      }
      mbase.sessionSelectionChanged();
   }
}













