#include "mChartsWidget.h"

mChartsWidget::mChartsWidget(QWidget *parent) : QWidget{parent}
{
   connect(&mbase, SIGNAL(mChartsWidgetReloadModelSignal()),      this, SLOT(reloadModel()));
   connect(&mbase, SIGNAL(mChartsWidgetControlsChangedSignal()),  this, SLOT(controlsChanged()));
   connect(&mbase, SIGNAL(mChartsWidgetChangeThemeSignal()),      this, SLOT(changeTheme()));
   connect(&mbase, SIGNAL(mChartsWidgetResetSplitterSignal()),    this, SLOT(resetSplitter()));
   connect(&mbase, SIGNAL(updateGlobalPositionSignal()),          this, SLOT(updateCursor()));

   statChartModel = new QSqlQueryModel;
   buildCharts();
}


void mChartsWidget::buildCharts()
{
   // vertical box layout
   vbox = new QVBoxLayout;
   vbox->setContentsMargins(0, 0, 0, 0);
   this->setLayout(vbox);



   // create groupBox and add to topLayout
   QGroupBox * topGroupBox = new QGroupBox("Charts");
   vbox->addWidget(topGroupBox);
   topGroupBox->setObjectName("t1");
   QString style = "QGroupBox#t1 { border: 1px solid " + mbase.mChartsWidgetColor.name();
   style += "; margin: 0px; margin-top:4px; background-color:" + mbase.mChartsWidgetColor.lighter(195).name();
   style += "; } QGroupBox#t1::title { left: 10px; top: -8px; }";
   topGroupBox->setStyleSheet(style);

   // create layout for topGroupBox
   QHBoxLayout *topGroupBoxLayout = new QHBoxLayout;
   topGroupBox->setLayout(topGroupBoxLayout);
   topGroupBoxLayout->setContentsMargins(4, 8, 4, 4);









   splitter = new QSplitter();
   splitter->setChildrenCollapsible(false);
   splitter->setOrientation(Qt::Vertical);

   topGroupBoxLayout->addWidget(splitter);




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

//      statChart[i]->setTitle(mbase.statChartGraphTypeArray[i].display_name);

      statChart[i]->setBackgroundRoundness(2);

      QColor col = mbase.statChartGraphTypeArray[i].color;
      statChart[i]->setBackgroundPen(QPen(col, 1));
      statChart[i]->setBackgroundBrush(QBrush(col.lighter(192)));

      QDateTimeAxis * statChartXaxis = new QDateTimeAxis;
      statChartXaxis->setFormat("HH:mm:ss.zzz");
      statChart[i]->addAxis(statChartXaxis, Qt::AlignBottom);
      statChartXaxis->hide();


      QValueAxis * statChartYaxis = new QValueAxis;
      statChartYaxis->setTitleText(mbase.statChartGraphTypeArray[i].display_name);
      statChartYaxis->setLabelFormat("%3.1f");
      statChart[i]->addAxis(statChartYaxis, Qt::AlignLeft);

      QFont labelsFont;
      labelsFont.setPixelSize(16);
      labelsFont.setWeight(QFont::Bold);

      statChartYaxis->setTitleFont(labelsFont);
      statChartYaxis->setTitleBrush(col.darker(150));

      for (int s=0; s<8; s++)
      {
         statChartSeries[i][s] = new QLineSeries;
         statChart[i]->addSeries(statChartSeries[i][s]);
         statChartSeries[i][s]->setName(mbase.statChartSeriesStructArray[s].name);
         statChartSeries[i][s]->setPen(QPen(mbase.statChartSeriesStructArray[s].col, mbase.mChartsWidgetPlotLineSize));
         statChartSeries[i][s]->setPointsVisible(true);
         statChartSeries[i][s]->attachAxis(statChartXaxis);
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



int mChartsWidget::getClosestIndexToDateTime(QDateTime dt)
{
   QString sql = "SELECT id, ABS(TIMESTAMPDIFF(MICROSECOND, timestamp, '";
   sql += dt.toString("yyyy-MM-dd HH:mm:ss.z");
   sql += "')) as mt FROM status ORDER by mt ASC LIMIT 1;";
   // qDebug() << "sql:" << sql;
   QSqlQuery query;
   if (!query.exec(sql)) { qDebug() << query.lastError(); }
   query.next();

   int id = query.value(0).toInt();
   // qDebug() << "id:" << id;

   return id;
}




void mChartsWidget::reloadModel()
{
   //qDebug() << "mChartsWidget::reload()";

   int cursorId       = getClosestIndexToDateTime(mbase.globalPosition);
   int sessionStartId = getClosestIndexToDateTime(mbase.sessionsDtStart);
   int sessionEndId   = getClosestIndexToDateTime(mbase.sessionsDtEnd);

   int sqlLimit = mbase.mChartsWidgetSqlModelLimit;

   // start at 1/2 limit less than cursor
   int sqlStartId = cursorId - sqlLimit / 2;

   // if less than session start, clamp to session start
   if (sqlStartId < sessionStartId) sqlStartId = sessionStartId;

   // end = start + limit
   int sqlEndId = sqlStartId + sqlLimit;

   // if greater than session end, clamp to session end
   if (sqlEndId > sessionEndId) sqlEndId = sessionEndId;


   // update the sql model
   // ----------------------------
   QString sql = "SELECT * FROM status WHERE id BETWEEN ";
   int lim = mbase.mChartsWidgetSqlModelLimit / 2;
   sql += QString::number(sqlStartId);
   sql += " AND ";
   sql += QString::number(sqlEndId);
   // qDebug() << "sql:" << sql;
   statChartModel->setQuery(sql);

   int rowCount = statChartModel->rowCount();

   if (rowCount == 0)
   {
      // qDebug() << "Error in mChartsWidget::reloadModel(): no rows for query\n" << sql;
      //return;
   }


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







   // iterate all rows in the model and fill series
   // ---------------------------------------------
   for (int r=0; r<statChartModel->rowCount(); r++)
   {
      // get datetime from column 1
      QModelIndex index = statChartModel->index(r, 1);
      QDateTime ts = statChartModel->data(index, Qt::DisplayRole).toDateTime();

      // check and update dateTimeStart and dateTimeEnd
      // if (ts < mbase.mChartsWidgetModelXAxisDateTimeStart) mbase.mChartsWidgetModelXAxisDateTimeStart = ts;
      // if (ts > mbase.mChartsWidgetModelXAxisDateTimeEnd)   mbase.mChartsWidgetModelXAxisDateTimeEnd   = ts;

      mbase.mChartsWidgetModelXAxisDateTimeStart = qMin(ts, mbase.mChartsWidgetModelXAxisDateTimeStart);
      mbase.mChartsWidgetModelXAxisDateTimeEnd   = qMax(ts, mbase.mChartsWidgetModelXAxisDateTimeEnd);


      // get player number from column 3
      index = statChartModel->index(r, 3);
      int p = statChartModel->data(index, Qt::DisplayRole).toInt();

      // if there are data points for series (players), set series visible
      mbase.statChartSeriesStructArray[p].active = true;
      mbase.statChartSeriesStructArray[p].visible = true;

      // extract data for all charts from remaining columns
      for (int s=0; s<NUM_CHARTS; s++)
         extract_series(s, r, mbase.statChartGraphTypeArray[s].sql_col_index, p, ts.toMSecsSinceEpoch());

   }


   // set the range for all charts
   // -----------------------------------
   for (int i=0; i<NUM_CHARTS; i++)
   {
      // get the first x axis from the chart
      QList qlxa = statChart[i]->axes(Qt::Horizontal);
      qlxa.at(0)->setRange(mbase.mChartsWidgetModelXAxisDateTimeStart, mbase.mChartsWidgetModelXAxisDateTimeEnd);

      // get the first y axis from the chart
      QList qlya = statChart[i]->axes(Qt::Vertical);
      qlya.at(0)->setRange(statChartMinY[i], statChartMaxY[i]);
   }


   // set x axis range in mbase
   // get first x axis axis of first chart
   QList qlxa = statChart[0]->axes(Qt::Horizontal);

   // cast to QValueAxis to get min and max
   const auto va = static_cast<QValueAxis*>(qlxa.at(0));

   // set new min and max
   mbase.mChartsWidgetXAxisMin = va->min();
   mbase.mChartsWidgetXAxisMax = va->max();








   mbase.mChartsWidgetControlWidgetUpdateFunction();
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
   // qDebug() << "void mChartsWidget::resetSplitter()";

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

void mChartsWidget::updateCursor()
{
   // qDebug() << "void mChartsWidget::updateCursor()";

   // is new global position in model range
   if ((mbase.globalPosition >= mbase.mChartsWidgetModelXAxisDateTimeStart) && (mbase.globalPosition <= mbase.mChartsWidgetModelXAxisDateTimeEnd))
   {
      for (int i=0; i<NUM_CHARTS; i++) statChart[i]->update();
   }
   else reloadModel();
}

void mChartsWidget::controlsChanged()
{
   // qDebug() << "mChartsWidget::controlsChanged()";
   for (int i=0; i<NUM_CHARTS; i++)
   {
      // set charts visible
      statChartView[i]->setVisible(mbase.statChartGraphTypeArray[i].visible);

      for (int s=0; s<8; s++)
      {
         // set series visible
         statChartSeries[i][s]->setVisible(mbase.statChartSeriesStructArray[s].visible);

         // set series line size
         statChartSeries[i][s]->setPen(QPen(mbase.statChartSeriesStructArray[s].col, mbase.mChartsWidgetPlotLineSize));
      }

      // set charts common x axis range
      double s = mbase.mChartsWidgetXAxisMin;
      double e = mbase.mChartsWidgetXAxisMax;
      if (e - s > 0) // check if range > 0
      {
         // get the first x axis from the chart
         QList qlxa = statChart[i]->axes(Qt::Horizontal);
         qlxa.at(0)->setRange(QDateTime::fromMSecsSinceEpoch(s), QDateTime::fromMSecsSinceEpoch(e));
      }
   }
}

void mChartsWidget::changeTheme()
{
//   qDebug() << "mChartsWidget::changeTheme()";

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
   mbase.mChartsWidgetControlWidgetUpdateFunction();
}
