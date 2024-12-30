#include "mCurrentSessionTimelineWidget.h"

mCurrentSessionTimelineWidget::mCurrentSessionTimelineWidget(QWidget *parent) : QWidget{parent}
{
   connect(&mbase, SIGNAL(mCurrentSessionTimelineWidgetUpdateSignal()), this, SLOT(updateUI()));

   // create scene
   scene = new QGraphicsScene();

   // create view and add scene
   view = new mCurrentSessionTimelineGraphicsView(this);
   view->setRenderHint(QPainter::Antialiasing);
   view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
   view->setMinimumHeight(timelineHeight);
   view->setMaximumHeight(timelineHeight);
   view->setScene(scene);
   connect(view, SIGNAL(timelineClicked(int)), this, SLOT(onTimelineClicked(int)));

   // create groupbox
   QGroupBox * gb = new QGroupBox("Current Session", this);

   // layout for groupbox
   QVBoxLayout *vbox = new QVBoxLayout;
   gb->setLayout(vbox);

   // add view to layout
   vbox->addWidget(view);

   // create a layout for all range labels group boxes and add to vbox layout
   QHBoxLayout * rangeLabelsLayout = new QHBoxLayout;
   vbox->addLayout(rangeLabelsLayout);

   createLabelHelper(rangeLabelsLayout, "Session Range",     labelSessionStart,    labelSessionEnd,    labelSessionRange,    mbase.sessionsColor);
   createLabelHelper(rangeLabelsLayout, "Table Model Range", labelTableModelStart, labelTableModelEnd, labelTableModelRange, mbase.mTablesWidgetColor);
   createLabelHelper(rangeLabelsLayout, "Chart Model Range", labelChartModelStart, labelChartModelEnd, labelChartModelRange, mbase.mChartsWidgetColor);

   // create outer groupbox
   QGroupBox * outerGroupBox = new QGroupBox("Current Position");
   QSize outerGroupBoxSize(150, 40);
   outerGroupBox->setMinimumSize(outerGroupBoxSize);
   outerGroupBox->setMaximumSize(outerGroupBoxSize);
   outerGroupBox->setObjectName("t1");

   QString style = "QGroupBox#t1 { border: 1px solid " + mbase.globalPositionColor.name();
   style += "; margin: 0px; margin-top:4px; background-color:" + mbase.globalPositionColor.lighter(195).name();
   style += "; } QGroupBox#t1::title { left: 10px; top: -8px; }";

   outerGroupBox->setStyleSheet(style);

   QVBoxLayout *vbox2 = new QVBoxLayout;
   outerGroupBox->setLayout(vbox2);
   vbox2->addWidget(labelCurrentPosition);

   rangeLabelsLayout->addWidget(outerGroupBox);

   // create top level hbox and add groupbox
   QHBoxLayout *topLayout = new QHBoxLayout;
   this->setLayout(topLayout);
   topLayout->addWidget(gb);
}

void mCurrentSessionTimelineWidget::createLabelHelper(QHBoxLayout* rll,  QString title, QLabel* start, QLabel* end, QLabel* range, QColor col)
{
   QSize outerGroupBoxSize(200, 64);

   // create outer groupbox
   QGroupBox * outerGroupBox = new QGroupBox(title);
   outerGroupBox->setObjectName("t1");
   outerGroupBox->setMinimumSize(outerGroupBoxSize);
   outerGroupBox->setMaximumSize(outerGroupBoxSize);
   QString style = "QGroupBox#t1 { border: 1px solid " + col.name();
   style += "; margin: 0px; margin-top:4px; background-color:" + col.lighter(195).name();
   style += "; } QGroupBox#t1::title { left: 10px; top: -8px; }";
   outerGroupBox->setStyleSheet(style);

   // create form layout
   QFormLayout *formLayout = new QFormLayout(this);
   formLayout->setLabelAlignment(Qt::AlignRight);
   formLayout->setSpacing(0);
   formLayout->setContentsMargins(6, 8, 6, 0);

   // apply layout to outerGroupBox
   outerGroupBox->setLayout(formLayout);

   start->setAlignment(Qt::AlignRight);
   end->setAlignment(Qt::AlignRight);
   range->setAlignment(Qt::AlignRight);

   formLayout->addRow(tr("Start:"), start);
   formLayout->addRow(tr("End:"), end);
   formLayout->addRow(tr("Range:"), range);

   // add outerGroupBox to layout
   rll->addWidget(outerGroupBox);
}

void mCurrentSessionTimelineWidget::onTimelineClicked(int pos)
{
   //qDebug() << "void mCurrentSessionTimelineWidget::onTimelineClicked(int pos)" << pos;
   mbase.updateGlobalPosition(QDateTime::fromMSecsSinceEpoch(mapToVal(pos)));
}



void mCurrentSessionTimelineWidget::updateUI(void)
{
   // qDebug() << "void mCurrentSessionTimelineWidget::update(void)";

   // set the mapping ranges
   mapInStart = mbase.sessionsDtStart.toMSecsSinceEpoch();
   mapInEnd = mbase.sessionsDtEnd.toMSecsSinceEpoch();
   mapOutStart = 1;
   mapOutEnd = view->viewport()->width()-1;

   // reset scene
   scene->clear();

   // entire range
   addRect(mbase.sessionsDtStart, mbase.sessionsDtEnd, 2, timelineHeight-2, mbase.sessionsColor);

   // chart model range
   addRect(mbase.mChartsWidgetModelXAxisDateTimeStart, mbase.mChartsWidgetModelXAxisDateTimeEnd, 6, timelineHeight-6, mbase.mChartsWidgetColor);

   // table model range
   addRect(mbase.mTablesWidgetModelXAxisDateTimeStart, mbase.mTablesWidgetModelXAxisDateTimeEnd, 10, timelineHeight-10, mbase.mTablesWidgetColor);

   // cursor (globalPosition)
   addLine(mbase.globalPosition, 2, timelineHeight-2, mbase.globalPositionColor);

   scene->setSceneRect(scene->itemsBoundingRect());

   labelSessionStart->setText(mbase.sessionsDtStart.toString("yyyy-MM-dd HH:mm:ss"));
   labelSessionEnd->setText(mbase.sessionsDtEnd.toString("yyyy-MM-dd HH:mm:ss"));
   labelSessionRange->setText(mbase.getRangeText(mbase.sessionsDtStart, mbase.sessionsDtEnd, 1000));

   labelChartModelStart->setText(mbase.mChartsWidgetModelXAxisDateTimeStart.toString("yyyy-MM-dd HH:mm:ss.z"));
   labelChartModelEnd->setText(mbase.mChartsWidgetModelXAxisDateTimeEnd.toString("yyyy-MM-dd HH:mm:ss.z"));
   labelChartModelRange->setText(mbase.getRangeText(mbase.mChartsWidgetModelXAxisDateTimeStart, mbase.mChartsWidgetModelXAxisDateTimeEnd, 1));

   labelTableModelStart->setText(mbase.mTablesWidgetModelXAxisDateTimeStart.toString("yyyy-MM-dd HH:mm:ss.z"));
   labelTableModelEnd->setText(mbase.mTablesWidgetModelXAxisDateTimeEnd.toString("yyyy-MM-dd HH:mm:ss.z"));
   labelTableModelRange->setText(mbase.getRangeText(mbase.mTablesWidgetModelXAxisDateTimeStart, mbase.mTablesWidgetModelXAxisDateTimeEnd, 1));

   labelCurrentPosition->setText(mbase.globalPosition.toString("yyyy-MM-dd HH:mm:ss.z"));

}


