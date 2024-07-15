#include "mChartsWidget.h"

mChartsWidget::mChartsWidget(QWidget *parent) : QWidget{parent}
{
   connect(&mbase, SIGNAL(mChartsWidgetUpdateSignal()),           this, SLOT(update()));
   connect(&mbase, SIGNAL(mChartsWidgetControlsChangedSignal()),  this, SLOT(controlsChanged()));
   connect(&mbase, SIGNAL(mChartsWidgetChangeThemeSignal()),      this, SLOT(changeTheme()));
   connect(&mbase, SIGNAL(mChartsWidgetResetSplitterSignal()),    this, SLOT(resetSplitter()));

   statChartModel = new QSqlQueryModel;

   buildCharts();
}



void mChartsWidget::buildCharts()
{
   delete(this->layout());

   for (int i=0; i<NUM_CHARTS; i++)
   {
      delete statChart[i];
      delete statChartView[i];
   }

   delete splitter;



   // vertical box layout
   vbox = new QVBoxLayout;
   this->setLayout(vbox);

   splitter = new QSplitter(this);
   splitter->setChildrenCollapsible(false);
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

      if (mbase.mChartsWidgetXAxisFrame)
      {
         //qDebug() << "value axis";
         QValueAxis * statChartXaxis = new QValueAxis;
         //statChartXaxis->setTitleText("Frame");
         statChartXaxis->setLabelFormat("%3.0f");
         statChart[i]->addAxis(statChartXaxis, Qt::AlignBottom);
      }
      else
      {
         //qDebug() << "datetime axis";
         QDateTimeAxis * statChartXaxis = new QDateTimeAxis;
         // statChartXaxis->setTitleText("Time");
         statChartXaxis->setFormat("HH:mm:ss.zzz");
         statChart[i]->addAxis(statChartXaxis, Qt::AlignBottom);
      }
//      statChartXaxis[i]->setTitleFont(labelsFont);
      // statChartXaxis[i]->setVisible(false);

      QValueAxis * statChartYaxis = new QValueAxis;
      statChartYaxis->setTitleText(mbase.statChartGraphTypeArray[i].display_name);
      statChartYaxis->setLabelFormat("%3.1f");
      statChart[i]->addAxis(statChartYaxis, Qt::AlignLeft);

      QFont labelsFont;
      labelsFont.setPixelSize(16);
      labelsFont.setWeight(QFont::Bold);

      statChartYaxis->setTitleFont(labelsFont);
      statChartYaxis->setTitleBrush(col.darker(150));

      // get the first x axis from the chart
      QList qlxa = this->statChart[i]->axes(Qt::Horizontal);

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

         statChartSeries[i][s]->attachAxis(qlxa.at(0));
         statChartSeries[i][s]->attachAxis(statChartYaxis);
      }

      statChartView[i] = new mChartView(statChart[i], this, i);
      statChartView[i]->setMinimumHeight(100);
      statChartView[i]->setVisible(mbase.statChartGraphTypeArray[i].active);
      statChartView[i]->chart()->setTheme(static_cast<QChart::ChartTheme>(mbase.mChartsWidgetChartTheme));

      splitter->addWidget(statChartView[i]);
   }

   readSplitterSizes();
   connect(splitter, SIGNAL(splitterMoved(int, int)), this, SLOT(saveSplitterSizes(int, int)));
   controlsChanged();
}





void mChartsWidget::update()
{
   qDebug() << "mChartsWidget::update()";

   // update the sql model
   // ----------------------------
   QString sql = "SELECT * FROM status WHERE timestamp " + mbase.sessionsSqlWhereDateClause;
   if (mbase.mChartsWidgetXAxisFrame) sql += " ORDER BY frame LIMIT ";
   else sql += " ORDER BY timestamp LIMIT ";
   sql += QString::number(mbase.mChartsWidgetSqlModelLimit);
   qDebug() << "sql: " << sql;
   statChartModel->setQuery(sql);

   buildCharts();

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

   // get the actual start and end of x axis (may be less than query, due to LIMIT clause)
   mbase.mChartsWidgetModelXAxisDateTimeStart = QDateTime::fromString("2100", "yyyy");
   mbase.mChartsWidgetModelXAxisDateTimeEnd   = QDateTime::fromString("1900", "yyyy");

   mbase.mChartsWidgetModelXAxisFrameStart =  999999999;
   mbase.mChartsWidgetModelXAxisFrameEnd   = -999999999;


   // iterate all rows in the model and fill series
   // ---------------------------------------------
   for (int r=0; r<statChartModel->rowCount(); r++)
   {
      // get datetime from column 1
      QModelIndex index = statChartModel->index(r, 1);
      QDateTime ts = statChartModel->data(index, Qt::DisplayRole).toDateTime();

      // check and update dateTimeStart and dateTimeEnd
      if (ts < mbase.mChartsWidgetModelXAxisDateTimeStart) mbase.mChartsWidgetModelXAxisDateTimeStart = ts;
      if (ts > mbase.mChartsWidgetModelXAxisDateTimeEnd)   mbase.mChartsWidgetModelXAxisDateTimeEnd   = ts;

      // get frame number from column 2
      index = statChartModel->index(r, 2);
      int f = statChartModel->data(index, Qt::DisplayRole).toInt();

       // check and update frameStart and frameEnd
      if (f < mbase.mChartsWidgetModelXAxisFrameStart) mbase.mChartsWidgetModelXAxisFrameStart = f;
      if (f > mbase.mChartsWidgetModelXAxisFrameEnd)   mbase.mChartsWidgetModelXAxisFrameEnd   = f;

      // get player number from column 3
      index = statChartModel->index(r, 3);
      int p = statChartModel->data(index, Qt::DisplayRole).toInt();

      // if there are data points for series (players), set series visible
      mbase.statChartSeriesStructArray[p].active = true;
      mbase.statChartSeriesStructArray[p].visible = true;

      // set x axis to either dateTime or frame
      double xAxisVal = ts.toMSecsSinceEpoch();
      if (mbase.mChartsWidgetXAxisFrame) xAxisVal = f;

      // extract data for all charts from remaining columns
      for (int s=0; s<NUM_CHARTS; s++)
         extract_series(s, r, mbase.statChartGraphTypeArray[s].sql_col_index, p, xAxisVal);

   }

   // set the range for all charts
   // -----------------------------------
   for (int i=0; i<NUM_CHARTS; i++)
   {
      // get the first x axis from the chart
      QList qlxa = this->statChart[i]->axes(Qt::Horizontal);
      if (mbase.mChartsWidgetXAxisFrame)  qlxa.at(0)->setRange(mbase.mChartsWidgetModelXAxisFrameStart,    mbase.mChartsWidgetModelXAxisFrameEnd);
      else                                qlxa.at(0)->setRange(mbase.mChartsWidgetModelXAxisDateTimeStart, mbase.mChartsWidgetModelXAxisDateTimeEnd);

      // get the first y axis from the chart
      QList qlya = this->statChart[i]->axes(Qt::Vertical);
      qlya.at(0)->setRange(statChartMinY[i], statChartMaxY[i]);
   }
   mbase.updateLegendFunction();

}

void mChartsWidget::extract_series(int i, int r, int c, int p, double xAxisVal)
{
   QModelIndex index = statChartModel->index(r, c);
   float val = statChartModel->data(index, Qt::DisplayRole).toFloat();
   statChartMinY[i] = qMin(statChartMinY[i], val);
   statChartMaxY[i] = qMax(statChartMaxY[i], val);
   statChartSeries[i][p]->append(xAxisVal, val);
}






void mChartsWidget::resetSplitter()
{
   qDebug() << "void mChartsWidget::resetSplitter()";

   // get the height of the widget
   int height = this->height();

   // get a list of all splitter sizes
   QList <int> sizes = splitter->sizes();

   // count how many charts are visible
   int count = 0;
   for (int i=0; i<NUM_CHARTS; i++)
      if (statChartView[i]->isVisible()) count++;

   // split the height evenly among the visible charts
   int avg = (float) height / (float) count;

   qDebug() << "height:" << height << "count:" << count << "avg:" << avg;

   // set height for all visible charts
   for (int i=0; i<splitter->count(); i++)
      if (statChartView[i]->isVisible()) sizes[i] = avg;

   splitter->setSizes(sizes);
   saveSplitterSizes(0, 0);
}

void mChartsWidget::saveSplitterSizes(int pos, int index)
{
   //qDebug() << "void mChartsWidget::saveSplitterSizes()";

   // get a list of all splitter sizes
   QList <int> sizes = splitter->sizes();

   mbase.settings->beginWriteArray("splitter");
   for (int i=0; i<splitter->count(); i++)
   {
      mbase.settings->setArrayIndex(i);
      mbase.settings->setValue("size", sizes[i]);
      //qDebug() << "w:" << i << " - " << sizes[i];
   }
   mbase.settings->endArray();
}

void mChartsWidget::readSplitterSizes()
{
   //qDebug() << "void mChartsWidget::readSplitterSizes()";
   // get a list of all splitter sizes
   QList <int> sizes = splitter->sizes();

   int size = mbase.settings->beginReadArray("splitter");
   for (int i = 0; i < size; ++i)
   {
      mbase.settings->setArrayIndex(i);
      sizes[i] = mbase.settings->value("size", 0).toInt();
      //qDebug() << "r:" << i << " - " << sizes[i];
   }
   mbase.settings->endArray();
   splitter->setSizes(sizes);
}


void mChartsWidget::controlsChanged()
{
   // qDebug() << "mChartsWidget::controlsChanged()";
   for (int i=0; i<NUM_CHARTS; i++)
   {
      // set charts visible
      statChartView[i]->setVisible(mbase.statChartGraphTypeArray[i].visible);

      // get the first x axis from the chart
      QList qlxa = this->statChart[i]->axes(Qt::Horizontal);

      // set charts common x axis range
      if (mbase.mChartsWidgetXAxisFrame) qlxa.at(0)->setRange(mbase.mChartsWidgetXAxisMin, mbase.mChartsWidgetXAxisMax);
      else qlxa.at(0)->setRange(QDateTime::fromMSecsSinceEpoch(mbase.mChartsWidgetXAxisMin), QDateTime::fromMSecsSinceEpoch(mbase.mChartsWidgetXAxisMax));

      for (int s=0; s<8; s++)
      {
         // series visible or not
         statChartSeries[i][s]->setVisible(mbase.statChartSeriesStructArray[s].visible);

         // series line size
         statChartSeries[i][s]->setPen(QPen(mbase.statChartSeriesStructArray[s].col, mbase.mChartsWidgetPlotLineSize));
      }
   }
}


void mChartsWidget::changeTheme()
{
   qDebug() << "mChartsWidget::changeTheme()";

   // test if theme is the same as current theme
   int newTheme = mbase.mChartsWidgetChartTheme;
   int oldTheme = statChartView[0]->chart()->theme();
   if (oldTheme == newTheme)
   {
      int tmpTheme = newTheme + 1;
      if (tmpTheme > 7) tmpTheme = 0;
      // set temp theme
      for (int i=0; i<NUM_CHARTS; i++)
         statChartView[i]->chart()->setTheme(static_cast<QChart::ChartTheme>(tmpTheme));
   }
   // set new theme
   for (int i=0; i<NUM_CHARTS; i++)
      statChartView[i]->chart()->setTheme(static_cast<QChart::ChartTheme>(newTheme));

   if (mbase.mChartsWidgetForceMySeriesColors) mbase.setStatChartSeriesStructArrayColors();
   else
   {
      // get series colors from newly applied theme and overwrite colors in array
      for (int s=0; s<8; s++)
         mbase.statChartSeriesStructArray[s].col = statChartSeries[0][s]->color();
   }

   // change all series colors
   controlsChanged();

   // change legend colors
   mbase.updateLegendFunction();
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

















