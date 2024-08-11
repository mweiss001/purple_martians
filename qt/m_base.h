#ifndef M_BASE_H
#define M_BASE_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>
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


#define NUM_TABLES 4


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
      QString dbHostName, dbDatabaseName, dbUserName, dbPassword;
      void setup_db(void);

      QSettings * settings;

      struct log_type log_types[100];
      void init_log_types(void);

      struct col_type col_types[10];
      void init_col_types(void);

      QString getRangeText(QDateTime start, QDateTime end, int precision);

      QImage rectangle(qreal imageSize, const QColor &color);
      QImage circle(qreal imageSize, const QColor &color);


      // Global Cursor DateTime Position
      //------------------------------------------------------------
      QDateTime globalPosition;
      void updateGlobalPosition(QDateTime pos);
//      QColor globalPositionColor = QColorConstants::Svg::purple;

      QColor globalPositionColor = QColor(150, 0, 255);




      // mTablesWidget
      //------------------------------------------------------------


      QColor mTablesWidgetColor = Qt::green;



      QFont mTablesWidgetFont;
      void mTablesWidgetFontChangeFunction();

      bool mTablesWidgetTableFilter[NUM_TABLES][2][8] = { 0 };

      void saveFilters();
      void loadFilters();

      void setDefaultFilters(int type);

      // x axis ranges of sql model
      QDateTime mTablesWidgetModelXAxisDateTimeStart;
      QDateTime mTablesWidgetModelXAxisDateTimeEnd;




      // mTablesWidgetControlWidget
      //------------------------------------------------------------
      int numVisibleTables = 2;
      bool showFilterControls = true;
      int mTablesWidgetFontSize = 8;

      QString mTablesWidgetControlWidgetSqlWhereTypeClause;
      int mTablesWidgetSqlModelLimit = 400;



      // mSessionsWidget
      // -----------------------------------------------------------

      QSqlQueryModel *sessionsModel = nullptr;

      // time range set by selecting one or more sessions
      QDateTime sessionsDtStart;
      QDateTime sessionsDtEnd;
      QString   sessionsSqlWhereDateClause;

      void sessionSelectionChanged(); // this emits 3 signals !!!

      QColor sessionsColor = Qt::blue;



      // mChartsWidget
      //------------------------------------------------------------

      QColor mChartsWidgetColor = Qt::red;

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

      // mChartsWidgetControlsWidget
      //------------------------------------------------------------
      double mChartsWidgetXAxisMin;
      double mChartsWidgetXAxisMax;
      int mChartsWidgetPlotLineSize = 1;
      int mChartsWidgetChartTheme = 0;
      bool mChartsWidgetForceMySeriesColors = true;
      int mChartsWidgetSqlModelLimit = 200;



      // functions to emit signals
      //------------------------------------------------------------

      void mChartsWidgetReloadModelFunction()               {  emit mChartsWidgetReloadModelSignal(); emit mCurrentSessionTimelineWidgetUpdateSignal(); }
      void mChartsWidgetControlsChangedFunction()           {  emit mChartsWidgetControlsChangedSignal();   }
      void mChartsWidgetChangeThemeFunction()               {  emit mChartsWidgetChangeThemeSignal();   }
      void mChartsWidgetResetSplitterFunction()             {  emit mChartsWidgetResetSplitterSignal();   }

      void mChartsWidgetControlWidgetUpdateFunction()       { emit mChartsWidgetControlWidgetUpdateSignal(); }

      void mTablesWidgetReloadModelFunction()               {  emit mTablesWidgetReloadModelSignal(); emit mCurrentSessionTimelineWidgetUpdateSignal();  }
      void mTablesWidgetUpdateUIFunction()                  {  emit mTablesWidgetUpdateUISignal();  }

      void mTablesWidgetTableFiltersChangeFunction();

      void mTablesWidgetControlWidgetUpdateFunction()       {  emit mTablesWidgetControlWidgetUpdateSignal(); }



   signals:
      void mChartsWidgetReloadModelSignal();
      void mChartsWidgetControlsChangedSignal();
      void mChartsWidgetChangeThemeSignal();
      void mChartsWidgetResetSplitterSignal();

      void mChartsWidgetControlWidgetUpdateSignal();

      void mTablesWidgetReloadModelSignal();
      void mTablesWidgetUpdateUISignal();

      void mTablesWidgetControlWidgetUpdateSignal();

      void mCurrentSessionTimelineWidgetUpdateSignal();

      void updateGlobalPositionSignal();


};
#endif // M_BASE_H
extern m_base mbase;
