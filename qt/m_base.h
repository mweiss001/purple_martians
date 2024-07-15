#ifndef M_BASE_H
#define M_BASE_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QColor>
#include <QDateTime>
#include <QPainter>

#include <QSettings>


#define LOG_error                  9
#define LOG_NET                    10
#define LOG_NET_ending_stats       22
#define LOG_NET_bandwidth          23

#define LOG_NET_stdf_rewind        25
#define LOG_NET_stdf_create        26
#define LOG_NET_stdf               27
#define LOG_NET_stdf_packets       28

#define LOG_NET_dif_apply          30

#define LOG_NET_stak               33
#define LOG_NET_cdat               35
#define LOG_NET_client_ping        36
#define LOG_NET_timer_adjust       37
#define LOG_NET_file_transfer      39

#define NUM_CHARTS 12


struct log_type
{
   int valid;
   int shown; // 0=hidden, 1=shown
   int num;   // how many entries in the current loaded view
   char name[40];
   QColor color;
};

struct col_type
{
   int valid;
   int shown; // 0=hidden, 1=shown
   char db_name[40];
   char display_name[40];
};

struct statChartSeriesStruct
{
   QString name;
   QColor col;
   QString hostname;
   bool active;
   bool visible;
};

struct statChartGraphType
{
   int active;
   int visible;
   int sql_col_index;
   QString db_name;
   QString display_name;
   QColor color;
};


class m_base : public QObject
{
   Q_OBJECT

   public:

      m_base();

      QSqlDatabase db;
      void setup_db(void);

      QSettings * settings;



      struct log_type log_types[100];
      void init_log_types(void);

      struct col_type col_types[10];
      void init_col_types(void);


      QImage rectangle(qreal imageSize, const QColor &color);
      QImage circle(qreal imageSize, const QColor &color);

      // mTablesWidget
      //------------------------------------------------------------
      QFont mTablesWidgetFont;
      int mTablesWidgetFontSize = 8;
      void setFont(void);

      // mTablesWidgetControlWidget
      //------------------------------------------------------------
      QString mTablesWidgetControlWidgetSqlWhereTypeClause;
      int mTablesWidgetSqlModelLimit = 400;


      // mSessionsWidget
      // -----------------------------------------------------------

      // time range set by selecting one or more sessions
      QDateTime sessionsDtStart;
      QDateTime sessionsDtEnd;
      QTime     sessionsRange;
      QString   sessionsSqlWhereDateClause;

      void sessionSelectionChanged(); // this emits 2 signals !!!



      // mChartsWidget
      //------------------------------------------------------------

      // chart types
      struct statChartGraphType statChartGraphTypeArray[NUM_CHARTS];
      void initStatChartGraphTypeArray(void);

      // series types
      void initStatChartSeriesStructArray(void);
      void setStatChartSeriesStructArrayColors(void);
      struct statChartSeriesStruct statChartSeriesStructArray[8];

      // x axis ranges of sql model
      QDateTime mChartsWidgetModelXAxisDateTimeStart;
      QDateTime mChartsWidgetModelXAxisDateTimeEnd;
      int mChartsWidgetModelXAxisFrameStart;
      int mChartsWidgetModelXAxisFrameEnd;


      // mChartsWidgetControlsWidget
      //------------------------------------------------------------
      double mChartsWidgetXAxisMin;
      double mChartsWidgetXAxisMax;
      int mChartsWidgetPlotLineSize = 1;
      int mChartsWidgetChartTheme = 0;
      bool mChartsWidgetForceMySeriesColors = true;
      bool mChartsWidgetXAxisFrame = false;
      int mChartsWidgetSqlModelLimit = 200;



      // functions to emit signals
      //------------------------------------------------------------

      void mChartsWidgetUpdateFunction()                    {  emit mChartsWidgetUpdateSignal();   }
      void mChartsWidgetControlsChangedFunction()           {  emit mChartsWidgetControlsChangedSignal();   }
      void mChartsWidgetChangeThemeFunction()               {  emit mChartsWidgetChangeThemeSignal();   }
      void mChartsWidgetResetSplitterFunction()             {  emit mChartsWidgetResetSplitterSignal();   }

      void mTablesWidgetUpdateFunction()                    {  emit mTablesWidgetUpdateSignal();   }
      void mTablesWidgetUpdateColumnsFunction()             {  emit mTablesWidgetUpdateColumnsSignal();   }
      void mTablesWidgetFontChangeFunction()                {  emit mTablesWidgetFontChangeSignal();   }

      void updateLegendFunction()                           {  emit updateLegendSignal();   }




   signals:
      void mChartsWidgetUpdateSignal();
      void mChartsWidgetControlsChangedSignal();
      void mChartsWidgetChangeThemeSignal();
      void mChartsWidgetResetSplitterSignal();

      void mTablesWidgetUpdateSignal();
      void mTablesWidgetUpdateColumnsSignal();
      void mTablesWidgetFontChangeSignal();

      void updateLegendSignal();

};
#endif // M_BASE_H
extern m_base mbase;
