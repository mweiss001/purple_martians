#ifndef MCHARTSWIDGET_H
#define MCHARTSWIDGET_H

#include <QValueAxis>
#include <QGraphicsLayout>
#include <QBoxLayout>
#include <QSplitter>
#include <QSqlQueryModel>
#include <QSqlQuery>


#include "mChartView.h"
#include "mChart.h"

#include "m_base.h"

class mChartsWidget : public QWidget
{
   Q_OBJECT

   public:
      explicit mChartsWidget(QWidget *parent = nullptr);

   signals:

   protected:

   public slots:
      void reloadModel();
      void controlsChanged();
      void changeTheme();
      void resetSplitter();
      void saveSplitterSizes(int pos, int index);
      void readSplitterSizes();

      void updateCursor();



   private:

      void buildCharts();

      void extract_series(int i, int r, int c, int p, double xAxisVal);


      int getClosestIndexToDateTime(QDateTime dt);



      QVBoxLayout * vbox;
      QSplitter * splitter = nullptr;

      QSqlQueryModel * statChartModel;

      QChart* statChart[NUM_CHARTS] = {nullptr};
      QChartView* statChartView[NUM_CHARTS] = {nullptr};

      QLineSeries* statChartSeries[NUM_CHARTS][8] = {nullptr};

      float statChartMinY[NUM_CHARTS];
      float statChartMaxY[NUM_CHARTS];
};

#endif // MCHARTSWIDGET_H
