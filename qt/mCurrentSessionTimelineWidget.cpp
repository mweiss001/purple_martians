#include "mCurrentSessionTimelineWidget.h"

mCurrentSessionTimelineWidget::mCurrentSessionTimelineWidget(QWidget *parent) : QWidget{parent}
{
   connect(&mbase, SIGNAL(mCurrentSessionTimelineWidgetUpdateSignal()), this, SLOT(update()));

   scene = new QGraphicsScene();

   // top level hbox
   QHBoxLayout *topHBox = new QHBoxLayout;
   this->setLayout(topHBox);

   QGroupBox * gb = new QGroupBox("Current Session Range", this);

   gb->setMaximumHeight(160);

   topHBox->addWidget(gb);




   QHBoxLayout *hbox = new QHBoxLayout;
   gb->setLayout(hbox);

   view = new mGraphicsViewSessionTimeline(this);
   view->setScene(scene);

   connect(view, SIGNAL(timelineClicked(int)), this, SLOT(onTimelineClicked(int)));

   view->setRenderHint(QPainter::Antialiasing);
   view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
   view->setMinimumHeight(120);
   hbox->addWidget(view);
}


void mCurrentSessionTimelineWidget::onTimelineClicked(int pos)
{
   //qDebug() << "void mCurrentSessionTimelineWidget::onTimelineClicked(int pos)" << pos;
   mbase.updateGlobalPosition(QDateTime::fromMSecsSinceEpoch(mapToVal(pos)));
}


qreal mCurrentSessionTimelineWidget::mapToPos(qreal val)
{
   return (val - mapInStart) / (mapInEnd - mapInStart) * (mapOutEnd - mapOutStart) + mapOutStart;
}

qreal mCurrentSessionTimelineWidget::mapToVal(qreal pos)
{
   return (pos - mapOutStart) / (mapOutEnd - mapOutStart) * (mapInEnd - mapInStart) + mapInStart;
}




void mCurrentSessionTimelineWidget::addRect(QDateTime start, QDateTime end, qreal y1, qreal y2, QColor col)
{
   qreal ds = mapToPos(start.toMSecsSinceEpoch());
   qreal de = mapToPos(end.toMSecsSinceEpoch());
   QRectF r(QPointF(ds, y1), QPointF(de, y2));
   scene->addRect(r, QPen(col));
}

void mCurrentSessionTimelineWidget::addLine(QDateTime pos, qreal y1, qreal y2, QColor col)
{
   qreal dp = mapToPos(pos.toMSecsSinceEpoch());
   QRectF r(QPointF(dp, y1), QPointF(dp, y2));
   scene->addRect(r, QPen(col));
}

void mCurrentSessionTimelineWidget::update(void)
{
   // qDebug() << "void mCurrentSessionTimelineWidget::update(void)";

   // set the mapping ranges
   mapInStart = mbase.sessionsDtStart.toMSecsSinceEpoch();
   mapInEnd = mbase.sessionsDtEnd.toMSecsSinceEpoch();
   mapOutStart = 4;
   mapOutEnd = view->viewport()->width()-8;

   // reset scene
   scene->clear();

   // entire range
   addRect(mbase.sessionsDtStart, mbase.sessionsDtEnd, 2, 98, Qt::black);

   // chart model range
   addRect(mbase.mChartsWidgetModelXAxisDateTimeStart, mbase.mChartsWidgetModelXAxisDateTimeEnd, 6, 94, Qt::red);

   // table model range
   addRect(mbase.mTablesWidgetModelXAxisDateTimeStart, mbase.mTablesWidgetModelXAxisDateTimeEnd, 10, 90, Qt::green);

   // cursor (globalPosition)
   addLine(mbase.globalPosition, 0, 100, Qt::blue);

   scene->setSceneRect(scene->itemsBoundingRect());

}


