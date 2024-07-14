#ifndef MCHARTSWIDGET_H
#define MCHARTSWIDGET_H

#include <QValueAxis>
#include <QGraphicsLayout>
#include <QBoxLayout>
#include <QSplitter>
#include <QSqlQueryModel>

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
   void resizeEvent(QResizeEvent *event) override;

public slots:
   void update();
   void controlsChanged();
   void resetSplitter();

   void changeTheme();

private:
   void extract_series(int i, int r, int c, int p, QDateTime ts);
   QVBoxLayout * vbox;
   QSplitter * splitter;

   QSqlQueryModel* statChartModel;
   QChart* statChart[NUM_CHARTS];
   QChartView* statChartView[NUM_CHARTS];

   QDateTimeAxis* statChartXaxis[NUM_CHARTS];
   QValueAxis*    statChartYaxis[NUM_CHARTS];
   QLineSeries* statChartSeries[NUM_CHARTS][8];

   float statChartMinY[NUM_CHARTS];
   float statChartMaxY[NUM_CHARTS];
};

#endif // MCHARTSWIDGET_H
