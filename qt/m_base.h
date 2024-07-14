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

#define NUM_CHARTS 11


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

      void setup_db(void);

      void update_sql(void);


      void init_log_types(void);
      struct log_type log_types[100];

      void init_col_types(void);
      struct col_type col_types[10];


      int max_model_rows = 400;

      QFont font;
      int font_size = 8;

      void set_font(void);

      QImage rectangle(qreal imageSize, const QColor &color);
      QImage circle(qreal imageSize, const QColor &color);

      QString sql;
      QSqlDatabase db;

      QString pml_sql_all;
      QString pml_sql_where_type;
      QString pml_sql_where_date;

      QString sqlWhereDt;
      QDateTime dtStart;
      QDateTime dtEnd;



      // mSessionsWidget
      // this is the time range set by selecting one or more sessions
      QDateTime sessionsDtStart;
      QDateTime sessionsDtEnd;
      QTime     sessionsRange;


      // mChartsWidget
      // charts types
      struct statChartGraphType statChartGraphTypeArray[NUM_CHARTS];
      void initStatChartGraphTypeArray(void);

      // series types
      void initStatChartSeriesStructArray(void);
      struct statChartSeriesStruct statChartSeriesStructArray[8];


      // mChartsWidgetControlsWidget
      QDateTime mChartsWidgetXAxisMin;
      QDateTime mChartsWidgetXAxisMax;

      int mChartsWidgetPlotLineSize = 1;

      int mChartsWidgetChartTheme = 2;

      void chartsWidgetsControlsChangedFunction()           {  emit chartsWidgetsControlsChangedSignal();   }
      void updateChartsWidgetFunction()                     {  emit updateChartsWidgetSignal();   }
      void updateLegendFunction()                           {  emit updateLegendSignal();   }
      void updateChartThemeFunction()                       {  emit updateChartThemeSignal();   }
      void resetChartSizeFunction()                         {  emit resetChartSizeSignal();   }

   signals:
      void chartsWidgetsControlsChangedSignal();
      void updateChartsWidgetSignal(void);
      void sql_changed(void);
      void updateLegendSignal(void);
      void updateChartThemeSignal(void);
      void resetChartSizeSignal(void);

};


#endif // M_BASE_H

extern m_base mbase;
