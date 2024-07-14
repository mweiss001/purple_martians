#include "mChartsWidget.h"

mChartsWidget::mChartsWidget(QWidget *parent)
    : QWidget{parent}
{
   connect(&mbase, SIGNAL(updateChartsWidgetSignal()), this, SLOT(update()));
   connect(&mbase, SIGNAL(chartsWidgetsControlsChangedSignal()), this, SLOT(controlsChanged()));

   connect(&mbase, SIGNAL(updateChartThemeSignal()), this, SLOT(changeTheme()));

   connect(&mbase, SIGNAL(resetChartSizeSignal()), this, SLOT(resetSplitter()));




   // ------------------------------
   // setup charts in constructor
   // ------------------------------
   statChartModel = new QSqlQueryModel;

   // vertical box layout
   vbox = new QVBoxLayout;
   this->setLayout(vbox);

   splitter = new QSplitter(parent);
   splitter->setOrientation(Qt::Vertical);
   vbox->addWidget(splitter);


   for (int i=0; i<NUM_CHARTS; i++)
   {
      statChart[i] = new QChart;

      statChart[i]->setAcceptedMouseButtons(Qt::AllButtons);
      statChart[i]->setAcceptHoverEvents(true);

      int m = 1;
      statChart[i]->layout()->setContentsMargins(m, m, m, m);

      m = 0;
      statChart[i]->setMargins(QMargins(m, m, m, m));

      statChart[i]->legend()->setVisible(false);

      //statChart[i]->setTitle(mbase.statChartGraphTypeArray[i].display_name);

      statChart[i]->setBackgroundRoundness(2);

      QColor col = mbase.statChartGraphTypeArray[i].color;
      statChart[i]->setBackgroundPen(QPen(col, 1));
      statChart[i]->setBackgroundBrush(QBrush(col.lighter(192)));

      statChartXaxis[i] = new QDateTimeAxis;
      //statChartXaxis[i]->setTitleText("Time");
      statChartXaxis[i]->setFormat("HH:mm:ss.zzz");
      statChart[i]->addAxis(statChartXaxis[i], Qt::AlignBottom);

    // statChartXaxis[i]->setVisible(false);

      statChartYaxis[i] = new QValueAxis;
      statChartYaxis[i]->setTitleText(mbase.statChartGraphTypeArray[i].display_name);
      statChartYaxis[i]->setLabelFormat("%3.1f");
      statChart[i]->addAxis(statChartYaxis[i], Qt::AlignLeft);

      QFont labelsFont;
      labelsFont.setPixelSize(16);
      labelsFont.setWeight(QFont::Bold);

      statChartYaxis[i]->setTitleFont(labelsFont);
      statChartYaxis[i]->setTitleBrush(col.darker(150));

      for (int s=0; s<8; s++)
      {
         statChartSeries[i][s] = new QLineSeries;
         statChart[i]->addSeries(statChartSeries[i][s]);
         statChartSeries[i][s]->setName(mbase.statChartSeriesStructArray[s].name);
         statChartSeries[i][s]->setPen(QPen(mbase.statChartSeriesStructArray[s].col, mbase.mChartsWidgetPlotLineSize));
         statChartSeries[i][s]->setPointsVisible(true);

         //statChartSeries[i][s]->setColor(mbase.statChartSeriesStructArray[s].col);
         // constexpr qreal marker_size = 4.;
         // statChartSeries[i][s]->setLightMarker(mbase.circle(marker_size, mbase.statChartSeriesStructArray[s].col));
         // statChartSeries[i][s]->setMarkerSize(marker_size);

         statChartSeries[i][s]->attachAxis(statChartXaxis[i]);
         statChartSeries[i][s]->attachAxis(statChartYaxis[i]);
      }

      statChartView[i] = new mChartView(statChart[i], this, i);

      statChartView[i]->setMinimumHeight(100);

      splitter->addWidget(statChartView[i]);

      statChartView[i]->setVisible(mbase.statChartGraphTypeArray[i].active);
   }
}


void mChartsWidget::resizeEvent(QResizeEvent *event)
{
}


void mChartsWidget::update()
{

   // update the sql model
   // ----------------------------
   QString sqlw = "timestamp BETWEEN '";
   sqlw += mbase.sessionsDtStart.toString("yyyy-MM-dd HH:mm:ss.z");
   sqlw += "' AND '";
   sqlw += mbase.sessionsDtEnd.toString("yyyy-MM-dd HH:mm:ss.z");
   sqlw += "'";
   QString sql = "SELECT * FROM status WHERE " + sqlw;
   sql += " ORDER BY timestamp ";
   sql += " LIMIT 400";
   //   qDebug() << "sql:" << sql;
   statChartModel->setQuery(sql);


   // clear the series and reset min max
   // -----------------------------------
   for (int i=0; i<NUM_CHARTS; i++)
   {
      for (int s=0; s<8; s++)
      {
         statChartSeries[i][s]->clear();
         statChartSeries[i][s]->setVisible(true);
      }
      statChartMinY[i] =  99999999.0;
      statChartMaxY[i] = -99999999.0;
   }

   // clear the series legend variables
   for (int s=0; s<8; s++)
   {
      mbase.statChartSeriesStructArray[s].active = false;
      mbase.statChartSeriesStructArray[s].visible = false;
   }

   // store the actual last timestamp (may be less than query, due to LIMIT clause)
   QDateTime last;


   // iterate all rows in the model and fill series
   // ---------------------------------------------
   for (int r=0; r<statChartModel->rowCount(); r++)
   {
      // get datetime from column 1
      QModelIndex index = statChartModel->index(r, 1);
      QDateTime ts = statChartModel->data(index, Qt::DisplayRole).toDateTime();

      // update last with current ts
      last = ts;

      // get frame number from column 2
      index = statChartModel->index(r, 2);
      int f = statChartModel->data(index, Qt::DisplayRole).toInt();

      // get player number from column 3
      index = statChartModel->index(r, 3);
      int p = statChartModel->data(index, Qt::DisplayRole).toInt();

      // if there are data points for series, set series visible on legend
      mbase.statChartSeriesStructArray[p].active = true;
      mbase.statChartSeriesStructArray[p].visible = true;

      // extract data for all charts from remaining columns
      for (int s=0; s<NUM_CHARTS; s++)
         extract_series(s, r, mbase.statChartGraphTypeArray[s].sql_col_index, p, ts );

   }

   // set the range for all charts
   // -----------------------------------
   for (int i=0; i<NUM_CHARTS; i++)
   {
      statChartXaxis[i]->setRange(mbase.sessionsDtStart, last);
      statChartYaxis[i]->setRange(statChartMinY[i], statChartMaxY[i]);
   }

   mbase.updateLegendFunction();
}


void mChartsWidget::resetSplitter()
{
   // get the height of the widget
   int height = this->height();

   // get a list of all splitter sizes
   QList <int> sizes = splitter->sizes();

   // count how many charts have size > 0
   int count = 0;
   for (int i=0; i<splitter->count(); i++)
      if (sizes[i] > 0) count++;

   // split the height evenly among the visible charts
   int avg = (float) height / (float) count;

   qDebug() << "height:" << height << "count:" << count << "avg:" << avg;

   // set height for all visible charts
   for (int i=0; i<splitter->count(); i++)
      if (sizes[i] > 0) sizes[i] = avg;

   splitter->setSizes(sizes);



//   splitter->setSizes(QList<int>({INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX}));


   /*

   QList <int> sizes = splitter->sizes();

   int count = 0;
   int rt = 0;

   for (int i=0; i<splitter->count(); i++)
      if (sizes[i] > 0)
      {
         count++;
         rt+= sizes[i];
      }

   int avg = (float) rt / (float) count;

   for (int i=0; i<splitter->count(); i++)
      if (sizes[i] > 0) sizes[i] = avg;

   splitter->setSizes(sizes);

*/
}


void mChartsWidget::extract_series(int i, int r, int c, int p, QDateTime ts)
{
   QModelIndex index = statChartModel->index(r, c);
   float val = statChartModel->data(index, Qt::DisplayRole).toFloat();
   statChartMinY[i] = qMin(statChartMinY[i], val);
   statChartMaxY[i] = qMax(statChartMaxY[i], val);
   statChartSeries[i][p]->append(ts.toMSecsSinceEpoch(), val);
}

void mChartsWidget::controlsChanged()
{
   qDebug() << "mChartsWidget::controlsChanged()";
   for (int i=0; i<NUM_CHARTS; i++)
   {
      // set charts visible
      statChartView[i]->setVisible(mbase.statChartGraphTypeArray[i].visible);

      // set charts common x axis range
      statChartXaxis[i]->setRange(mbase.mChartsWidgetXAxisMin, mbase.mChartsWidgetXAxisMax);

      for (int s=0; s<8; s++)
      {
         // series visible or not
         statChartSeries[i][s]->setVisible(mbase.statChartSeriesStructArray[s].visible);

         // series line size
         statChartSeries[i][s]->setPen(QPen(mbase.statChartSeriesStructArray[s].col, mbase.mChartsWidgetPlotLineSize));
      }
   }

//   resetSplitter();

}



void mChartsWidget::changeTheme()
{
   qDebug() << "mChartsWidget::changeTheme()";
   for (int i=0; i<NUM_CHARTS; i++)
//      statChartView[i]->chart()->setTheme(QChart::ChartThemeBlueCerulean);
      statChartView[i]->chart()->setTheme(static_cast<QChart::ChartTheme>(mbase.mChartsWidgetChartTheme));
}



































































/*

void mChartsWidget::connectMarkers(int i)
{
   const auto markers = statChart[i]->legend()->markers();
   for (QLegendMarker *marker : markers)
   {
      // Disconnect possible existing connection to avoid multiple connections
      QObject::disconnect(marker, &QLegendMarker::clicked, this, &mChartsWidget::handleMarkerClicked);
      QObject::connect(   marker, &QLegendMarker::clicked, this, &mChartsWidget::handleMarkerClicked);
   }
}


void mChartsWidget::legendSetup()
{
   QLegend *legend = statChart[0]->legend();

   legend->setVisible(true);
   legend->detachFromChart();
   legend->setInteractive(false);
   legend->setBackgroundVisible(true);

   QFont Font;
   Font.setPixelSize(10);
   Font.setWeight(QFont::Bold);
   legend->setFont(Font);

   // count number of visible legend markers
   int nvlm = 0;
   const auto markers = statChart[0]->legend()->markers();
   for (QLegendMarker *marker : markers)
      if (marker->isVisible()) nvlm++;

   // use nvlm to set legend height
   float h = 19 + nvlm * 22;
   float y = 10;
   float w = 50;
   float x = QRectF(statChart[0]->geometry()).right() - w - 10;

   legend->setGeometry(QRectF(x, y, w, h));
   legend->update();
}



void mChartsWidget::hMC_helper(QLegendMarker * marker)
{
   qreal alpha = 1.0;

   // dim the marker, if series is not visible
   if (!marker->series()->isVisible()) alpha = 0.5;

   QColor color;
   QBrush brush = marker->labelBrush();
   color = brush.color();
   color.setAlphaF(alpha);
   brush.setColor(color);
   marker->setLabelBrush(brush);

   brush = marker->brush();
   color = brush.color();
   color.setAlphaF(alpha);
   brush.setColor(color);
   marker->setBrush(brush);

   QPen pen = marker->pen();
   color = pen.color();
   color.setAlphaF(alpha);
   pen.setColor(color);
   marker->setPen(pen);
}


void mChartsWidget::handleMarkerClicked()
{
   QLegendMarker * marker = qobject_cast<QLegendMarker *>(sender());
   switch (marker->type())
   {
   case QLegendMarker::LegendMarkerTypeXY:
   {
      // Toggle visibility of series
      marker->series()->setVisible(!marker->series()->isVisible());

      // Turn legend marker back to visible, since hiding series also hides the marker and we don't want it to happen now.
      marker->setVisible(true);

      hMC_helper(marker);

      // copy series visible to other charts
      for (int s=0; s<8; s++)
         for (int i=1; i<NUM_CHARTS; i++)
            statChartSeries[i][s]->setVisible(statChartSeries[0][s]->isVisible());
      break;
   }
   default:
   {
      qDebug("Unknown marker type");
      break;
   }
   }
   legendSetup();
}

*/






















































/*

void mChartsWidget::show_l(void)
{
   char m1[100];
   char m2[100];
   for (int i=0; i<8; i++)
   {
      sprintf(m1, "i:%d - ", i);
      for (int s=0; s<8; s++)
      {
         sprintf(m2, "%d ", statChartSeries[i][s]->isVisible());
         strcat(m1, m2);
      }
      qDebug() << m1;
   }
}


void mChartsWidget::copyLegendMarkers(void)
{



   //qDebug("copyLegendMarkers(void)");

   // make all markers for other charts the same as this one


//   qDebug() << "-b4-";
//   show_l();

   // do series visible
   for (int s=0; s<8; s++)
   {
      for (int i=1; i<8; i++)
         statChartSeries[i][s]->setVisible(statChartSeries[0][s]->isVisible());
   }

//   qDebug() << "-ft-";
//   show_l();


   // do legend markers

   // make a 2D array for all markers
   QLegendMarker * markerArr[10][10];

   for (int i=0; i<8; i++)
   {
      const auto markers = statChart[i]->legend()->markers();
      int s = 0;
      for (QLegendMarker *marker : markers)
      {
         markerArr[i][s] = marker;
         s++;
      }
   }

   for (int s=0; s<8; s++)
      for (int i=0; i<8; i++)
      {
         markerArr[i][s]->setVisible(markerArr[0][s]->isVisible());
         hMC_helper(markerArr[i][s]);
      }

}
*/











































   // change point size for all point in a series
   // for (int index=0; index<statChartSeries[0][0]->count(); index++)
   // {
   //    PointConfigurations config = statChartSeries[0][0]->pointConfiguration(index);
   //    config[QXYSeries::PointConfiguration::Size] = 4.0;
   //    statChartSeries[0][0]->setPointConfiguration(index, config);
   // }



   // can i get min and max for date axis ??

   // QDateTime dtmn = statChartXaxis[0]->min();
   // QDateTime dtmx = statChartXaxis[0]->max();

   // qDebug() << "min:" << dtmn << "  max:" << dtmx;





   /*
   // iterate all axes for this chart
   QList qla = statChart[0]->axes(Qt::Horizontal);
   for (int i = 0; i < qla.size(); ++i)
   {
      qDebug() << i << qla.at(i);
      // when debug prints this I get: 0 QDateTimeAxis(0x1efaf7220e0)
      // so I know I am getting a QDateTimeAxis...
   }
*/

   // so why does this say 'no member named 'min' in 'QAbstractAxis'

   // const auto dta = static_cast<const QDateTimeAxis*>(qla.at(0));


   // QDateTime dtmn = dta->min();
   // QDateTime dtmx = dta->max();
   // qDebug() << "min:" << dtmn << "  max:" << dtmx;





// // set the light marker
// constexpr qreal marker_size = 3.;
// statChartSeries[0][0]->setLightMarker(circle(marker_size, Qt::red));
// statChartSeries[0][0]->setMarkerSize(marker_size);


// this is the custom markers and changing them
// constexpr qreal marker_size = 16.;
// statChartSeries[0][0]->setMarkerSize(marker_size);
// statChartSeries[0][0]->setLightMarker(rectangle(marker_size, Qt::red));
// statChartSeries[0][0]->setSelectedLightMarker(rectangle(marker_size, Qt::blue));
// QObject::connect(statChartSeries[0][0], &QXYSeries::clicked, statChartSeries[0][0], [=](const QPointF &point) {
//    int index = statChartSeries[0][0]->points().indexOf(point);
//    qDebug() << "index:" << index;
//    if (index != -1)
//       statChartSeries[0][0]->toggleSelection({index});
// });








// generates random stuff
/*  status_chart_series1->clear();
   for (int i=0; i<100; i++)
      status_chart_series1->append(QRandomGenerator::global()->bounded(0, 10), QRandomGenerator::global()->bounded(0, 10));
*/

















