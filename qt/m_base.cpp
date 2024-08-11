#include "m_base.h"

#include <QMainWindow>

extern QMainWindow w;

// instance
m_base mbase;

// constructor
m_base::m_base()
{
   init_log_types();
   init_col_types();
   initStatChartSeriesStructArray();
   initStatChartGraphTypeArray();
   settings = new QSettings("pm_log.ini", QSettings::IniFormat);
}

void m_base::setup_db(void)
{
   dbHostName     = settings->value("dbHostName",     "purplemartians.org").toString();
   dbDatabaseName = settings->value("dbDatabaseName", "pm").toString();
   dbUserName     = settings->value("dbUserName",     "pmdb_ro").toString();
   dbPassword     = settings->value("dbPassword",     "readonly").toString();

   db = QSqlDatabase::addDatabase("QMYSQL");
   db.setHostName(dbHostName);
   db.setDatabaseName(dbDatabaseName);
   db.setUserName(dbUserName);
   db.setPassword(dbPassword);
   if (!db.open()) qDebug() << "database error:" << db.lastError().text();

   sessionsModel = new QSqlQueryModel();
   sessionsModel->setQuery("SELECT dt_start, dt_end, duration AS dur, player_num AS p, hostname AS host, endreason FROM sessions ORDER by dt_start");
}


void m_base::init_log_types(void)
{
   //printf("init log types\n");
   int i;
   for (i=0; i<100; i++)
   {
      log_types[i].valid = 0;
      log_types[i].shown = 0;
      log_types[i].num = 0;
      log_types[i].color.setRgbF(1, 1, 1, 1);
      strcpy(log_types[i].name, "");
   }
/*
   i = LOG_error;                      log_types[i].valid = 1; log_types[i].shown = 1;  strcpy(log_types[i].name, "LOG_error");
   i = LOG_NET;                        log_types[i].valid = 1; log_types[i].shown = 1;  strcpy(log_types[i].name, "LOG_NET");
   i = LOG_NET_ending_stats;           log_types[i].valid = 0; log_types[i].shown = 0;  strcpy(log_types[i].name, "LOG_NET_ending_stats");
   i = LOG_NET_bandwidth;              log_types[i].valid = 0; log_types[i].shown = 0;  strcpy(log_types[i].name, "LOG_NET_bandwidth");

   i = LOG_NET_stdf_rewind;            log_types[i].valid = 1; log_types[i].shown = 0;  strcpy(log_types[i].name, "LOG_NET_stdf_rewind");
   i = LOG_NET_stdf_create;            log_types[i].valid = 1; log_types[i].shown = 0;  strcpy(log_types[i].name, "LOG_NET_stdf_create");
   i = LOG_NET_stdf;                   log_types[i].valid = 1; log_types[i].shown = 1;  strcpy(log_types[i].name, "LOG_NET_stdf");
   i = LOG_NET_stdf_packets;           log_types[i].valid = 1; log_types[i].shown = 1;  strcpy(log_types[i].name, "LOG_NET_stdf_packets");

   i = LOG_NET_dif_apply;              log_types[i].valid = 1; log_types[i].shown = 1;  strcpy(log_types[i].name, "LOG_NET_dif_apply");

   i = LOG_NET_stak;                   log_types[i].valid = 1; log_types[i].shown = 1;  strcpy(log_types[i].name, "LOG_NET_stak");
   i = LOG_NET_cdat;                   log_types[i].valid = 1; log_types[i].shown = 1;  strcpy(log_types[i].name, "LOG_NET_cdat");
   i = LOG_NET_client_ping;            log_types[i].valid = 0; log_types[i].shown = 0;  strcpy(log_types[i].name, "LOG_NET_client_ping");
   i = LOG_NET_timer_adjust;           log_types[i].valid = 1; log_types[i].shown = 1;  strcpy(log_types[i].name, "LOG_NET_timer_adjust");
   i = LOG_NET_file_transfer;          log_types[i].valid = 0; log_types[i].shown = 1;  strcpy(log_types[i].name, "LOG_NET_file_transfer");

  */




   i = LOG_NET;                        log_types[i].valid = 1; log_types[i].shown = 1;  strcpy(log_types[i].name, "main");
   i = LOG_NET_stdf_rewind;            log_types[i].valid = 1; log_types[i].shown = 0;  strcpy(log_types[i].name, "stdf_rewind");
   i = LOG_NET_stdf_create;            log_types[i].valid = 1; log_types[i].shown = 0;  strcpy(log_types[i].name, "stdf_create");
   i = LOG_NET_stdf;                   log_types[i].valid = 1; log_types[i].shown = 1;  strcpy(log_types[i].name, "stdf");
   i = LOG_NET_stdf_packets;           log_types[i].valid = 1; log_types[i].shown = 1;  strcpy(log_types[i].name, "stdf_packets");
   i = LOG_NET_dif_apply;              log_types[i].valid = 1; log_types[i].shown = 1;  strcpy(log_types[i].name, "dif_apply");
   i = LOG_NET_stak;                   log_types[i].valid = 1; log_types[i].shown = 1;  strcpy(log_types[i].name, "stak");
   i = LOG_NET_cdat;                   log_types[i].valid = 1; log_types[i].shown = 1;  strcpy(log_types[i].name, "cdat");
   i = LOG_NET_timer_adjust;           log_types[i].valid = 1; log_types[i].shown = 1;  strcpy(log_types[i].name, "timer_adjust");

















   i = LOG_NET;                        log_types[i].color = QColor::fromString("lightblue");
   i = LOG_NET_ending_stats;           log_types[i].color = QColor::fromString("dodgerblue");
   i = LOG_NET_bandwidth;              log_types[i].color = QColor::fromString("darkkhaki");

   i = LOG_NET_stdf_rewind;            log_types[i].color = QColor::fromString("lime");
   i = LOG_NET_stdf_create;            log_types[i].color = QColor::fromString("lime");

   i = LOG_NET_stdf;                   log_types[i].color = QColor::fromString("cyan");
   i = LOG_NET_stdf_packets;           log_types[i].color = log_types[LOG_NET_stdf].color.darker(180);

   i = LOG_NET_dif_apply;              log_types[i].color = QColor::fromString("yellow");
   i = LOG_NET_stak;                   log_types[i].color = QColor::fromString("tomato");
   i = LOG_NET_cdat;                   log_types[i].color = QColor::fromString("chocolate");
   i = LOG_NET_client_ping;            log_types[i].color = QColor::fromString("thistle");
   i = LOG_NET_timer_adjust;           log_types[i].color = QColor::fromString("wheat");
   i = LOG_NET_file_transfer;          log_types[i].color = QColor::fromString("gray");

   for (i=0; i<100; i++)
      if (log_types[i].valid) log_types[i].color.setAlpha(128);
}

void m_base::init_col_types(void)
{
   //printf("init col types\n");
   int i;
   for (i=0; i<10; i++)
   {
      col_types[i].valid = 0;
      col_types[i].shown = 0;
      strcpy(col_types[i].db_name, "");
      strcpy(col_types[i].display_name, "");
   }
   i = 0;   col_types[i].valid = 1; col_types[i].shown = 0; strcpy(col_types[i].db_name, "id");        strcpy(col_types[i].display_name, "id");
   i = 1;   col_types[i].valid = 1; col_types[i].shown = 1; strcpy(col_types[i].db_name, "msg_type");  strcpy(col_types[i].display_name, "msg_type");
   i = 2;   col_types[i].valid = 1; col_types[i].shown = 1; strcpy(col_types[i].db_name, "sub_type");  strcpy(col_types[i].display_name, "sub_type");
   i = 3;   col_types[i].valid = 1; col_types[i].shown = 1; strcpy(col_types[i].db_name, "created");   strcpy(col_types[i].display_name, "created");
   i = 4;   col_types[i].valid = 1; col_types[i].shown = 0; strcpy(col_types[i].db_name, "agt");       strcpy(col_types[i].display_name, "agt");
   i = 5;   col_types[i].valid = 1; col_types[i].shown = 1; strcpy(col_types[i].db_name, "frame");     strcpy(col_types[i].display_name, "frame");
   i = 6;   col_types[i].valid = 1; col_types[i].shown = 1; strcpy(col_types[i].db_name, "player");    strcpy(col_types[i].display_name, "player");
   i = 7;   col_types[i].valid = 1; col_types[i].shown = 1; strcpy(col_types[i].db_name, "client");    strcpy(col_types[i].display_name, "client");
   i = 8;   col_types[i].valid = 1; col_types[i].shown = 1; strcpy(col_types[i].db_name, "message");   strcpy(col_types[i].display_name, "message");
}

void m_base::setStatChartSeriesStructArrayColors(void)
{
   statChartSeriesStructArray[0].col = QColorConstants::Svg::darkviolet;
   statChartSeriesStructArray[1].col = QColorConstants::Svg::red;
   statChartSeriesStructArray[2].col = QColorConstants::Svg::royalblue;
   statChartSeriesStructArray[3].col = QColorConstants::Svg::chartreuse;
   statChartSeriesStructArray[4].col = QColorConstants::Svg::cyan;
   statChartSeriesStructArray[5].col = QColorConstants::Svg::yellow;
   statChartSeriesStructArray[6].col = QColorConstants::Svg::mistyrose;
   statChartSeriesStructArray[7].col = QColorConstants::Svg::tan;
}

void m_base::initStatChartSeriesStructArray(void)
{
   statChartSeriesStructArray[0].name = "P0";
   statChartSeriesStructArray[1].name = "P1";
   statChartSeriesStructArray[2].name = "P2";
   statChartSeriesStructArray[3].name = "P3";
   statChartSeriesStructArray[4].name = "P4";
   statChartSeriesStructArray[5].name = "P5";
   statChartSeriesStructArray[6].name = "P6";
   statChartSeriesStructArray[7].name = "P7";

   for (int i=0; i<8; i++)
   {
      statChartSeriesStructArray[i].hostname = "";
      statChartSeriesStructArray[i].active = false;
      statChartSeriesStructArray[i].visible = false;
   }
   setStatChartSeriesStructArrayColors();
}

void m_base::initStatChartGraphTypeArray(void)
{
   int i;
   for (i=0; i<NUM_CHARTS; i++)
   {
      statChartGraphTypeArray[i].active = 0;
      statChartGraphTypeArray[i].db_name = "";
      statChartGraphTypeArray[i].display_name = "";
      statChartGraphTypeArray[i].color = Qt::black;
   }


   i=0;
   statChartGraphTypeArray[i].active = 1;
   statChartGraphTypeArray[i].sql_col_index = 5;
   statChartGraphTypeArray[i].db_name = "cpu";
   statChartGraphTypeArray[i].display_name = "cpu";
   statChartGraphTypeArray[i].color = Qt::red;

   i++;
   statChartGraphTypeArray[i].active = 1;
   statChartGraphTypeArray[i].sql_col_index = 6;
   statChartGraphTypeArray[i].db_name = "sync";
   statChartGraphTypeArray[i].display_name = "sync";
   statChartGraphTypeArray[i].color = Qt::blue;

   i++;
   statChartGraphTypeArray[i].active = 1;
   statChartGraphTypeArray[i].sql_col_index = 7;
   statChartGraphTypeArray[i].db_name = "ping";
   statChartGraphTypeArray[i].display_name = "ping";
   statChartGraphTypeArray[i].color = Qt::blue;


   i++;
   statChartGraphTypeArray[i].active = 1;
   statChartGraphTypeArray[i].sql_col_index = 4;
   statChartGraphTypeArray[i].db_name = "rwnd";
   statChartGraphTypeArray[i].display_name = "rwnd";
   statChartGraphTypeArray[i].color = Qt::red;

   i++;
   statChartGraphTypeArray[i].active = 1;
   statChartGraphTypeArray[i].sql_col_index = 8;
   statChartGraphTypeArray[i].db_name = "difs";
   statChartGraphTypeArray[i].display_name = "dif size";
   statChartGraphTypeArray[i].color = Qt::blue;

   i++;
   statChartGraphTypeArray[i].active = 1;
   statChartGraphTypeArray[i].sql_col_index = 9;
   statChartGraphTypeArray[i].db_name = "locr";
   statChartGraphTypeArray[i].display_name = "locr";
   statChartGraphTypeArray[i].color = Qt::green;

   i++;
   statChartGraphTypeArray[i].active = 1;
   statChartGraphTypeArray[i].sql_col_index = 10;
   statChartGraphTypeArray[i].db_name = "rcor";
   statChartGraphTypeArray[i].display_name = "rcor";
   statChartGraphTypeArray[i].color = Qt::green;

   i++;
   statChartGraphTypeArray[i].active = 1;
   statChartGraphTypeArray[i].sql_col_index = 11;
   statChartGraphTypeArray[i].db_name = "txbf";
   statChartGraphTypeArray[i].display_name = "txbf";
   statChartGraphTypeArray[i].color = Qt::green;

   i++;
   statChartGraphTypeArray[i].active = 1;
   statChartGraphTypeArray[i].sql_col_index = 12;
   statChartGraphTypeArray[i].db_name = "rxbf";
   statChartGraphTypeArray[i].display_name = "rxbf";
   statChartGraphTypeArray[i].color = Qt::green;

   i++;
   statChartGraphTypeArray[i].active = 1;
   statChartGraphTypeArray[i].sql_col_index = 13;
   statChartGraphTypeArray[i].db_name = "txpf";
   statChartGraphTypeArray[i].display_name = "txpf";
   statChartGraphTypeArray[i].color = Qt::green;

   i++;
   statChartGraphTypeArray[i].active = 1;
   statChartGraphTypeArray[i].sql_col_index = 14;
   statChartGraphTypeArray[i].db_name = "rxpf";
   statChartGraphTypeArray[i].display_name = "rxpf";
   statChartGraphTypeArray[i].color = Qt::green;


   i++;
   statChartGraphTypeArray[i].active = 1;
   statChartGraphTypeArray[i].sql_col_index = 2;
   statChartGraphTypeArray[i].db_name = "frame";
   statChartGraphTypeArray[i].display_name = "frame";
   statChartGraphTypeArray[i].color = Qt::white;


   int hstep = 360/(i+1);

   for (int h=0; h<=i; h++)
      statChartGraphTypeArray[h].color.setHsv(h*hstep, 255, 255);
}



void m_base::sessionSelectionChanged(void)
{
   globalPosition = sessionsDtStart;

   QString sql = " BETWEEN '";
   sql += sessionsDtStart.toString("yyyy-MM-dd HH:mm:ss.z");
   sql += "' AND '";
   sql += sessionsDtEnd.toString("yyyy-MM-dd HH:mm:ss.z");
   sql += "' ";

   sessionsSqlWhereDateClause = sql;

   emit mChartsWidgetReloadModelSignal();
   emit mTablesWidgetReloadModelSignal();
   emit mCurrentSessionTimelineWidgetUpdateSignal();
}


QString m_base::getRangeText(QDateTime start, QDateTime end, int precision)
{
   QString txt;
   int days = start.daysTo(end);
   if (days)
   {
      txt += QString::number(days);
      txt += "d ";
   }

   double msecs = end.toMSecsSinceEpoch() - start.toMSecsSinceEpoch();
   msecs = qRound(msecs/precision) * precision; // round to precision (100 = .1s)
   QTime timeRange = QTime::fromString("0", "s");
   timeRange = timeRange.addMSecs(msecs);

   if (precision == 1000) txt += timeRange.toString("hh:mm:ss");
   else txt += timeRange.toString("hh:mm:ss.zzz");

   return txt;
}




void m_base::updateGlobalPosition(QDateTime pos)
{
   globalPosition = pos;
   emit updateGlobalPositionSignal();
   emit mCurrentSessionTimelineWidgetUpdateSignal();

}

void m_base::mTablesWidgetFontChangeFunction()
{
   // qDebug() << "void m_base::mTablesWidgetFontChangeFunction(int fontSize)" << mTablesWidgetFontSize;
   mTablesWidgetFont = QFont{ "Courier", mTablesWidgetFontSize, QFont::Monospace };
   emit mTablesWidgetUpdateUISignal();
}



void m_base::mTablesWidgetTableFiltersChangeFunction()
{
   //qDebug() << "void m_base::mTablesWidgetTableFiltersChangeFunction()";
   saveFilters();
   emit mTablesWidgetUpdateUISignal();
   emit mTablesWidgetControlWidgetUpdateSignal();
}

void m_base::saveFilters()
{
   // qDebug() << "void mbase::saveFilters()";
   for (int t=0; t<NUM_TABLES; t++)
      for (int f=0; f<2; f++)
      {
         QString id = "mTableFilter";
         id += QString::number(t);
         id += QString::number(f);

         mbase.settings->beginWriteArray(id);
         for (int p=0; p<8; p++)
         {
            mbase.settings->setArrayIndex(p);
            mbase.settings->setValue("val", mbase.mTablesWidgetTableFilter[t][f][p]);
            // qDebug() << "w:" << id << p << " - " << mbase.mTablesWidgetTableFilter[t][f][p];
         }
         mbase.settings->endArray();
      }
}

void m_base::loadFilters()
{
   // qDebug() << "void mbase::loadFilters()";
   for (int t=0; t<NUM_TABLES; t++)
      for (int f=0; f<2; f++)
      {
         QString id = "mTableFilter";
         id += QString::number(t);
         id += QString::number(f);

         mbase.settings->beginWriteArray(id);
         for (int p=0; p<8; p++)
         {
            mbase.settings->setArrayIndex(p);
            mbase.mTablesWidgetTableFilter[t][f][p] = mbase.settings->value("val", 1).toBool();
            // qDebug() << "r:" << id << p << " - " << mbase.mTablesWidgetTableFilter[t][f][p];
         }
         mbase.settings->endArray();
      }
}

void m_base::setDefaultFilters(int type = 1)
{
   qDebug() << "setDefaultFilters()";


   // all player filters off and and all client filters on
   for (int t=0; t<NUM_TABLES; t++)
      for (int p=0; p<8; p++)
      {
         mTablesWidgetTableFilter[t][0][p] = false;
         mTablesWidgetTableFilter[t][1][p] = true;
      }

   if (type == 1)
   {
      numVisibleTables = 2;
      settings->setValue("numVisibleTables", numVisibleTables);

      // table 0
      mTablesWidgetTableFilter[0][0][0] = true; // server player 0

      // table 1
      mTablesWidgetTableFilter[1][0][1] = true; // client player 1
   }


   mTablesWidgetTableFiltersChangeFunction();

}

























QImage m_base::rectangle(qreal imageSize, const QColor &color)
{
   QImage image(imageSize, imageSize, QImage::Format_RGB32);
   QPainter painter;
   painter.begin(&image);
   painter.fillRect(0, 0, imageSize, imageSize, color);
   painter.end();
   return image;
}

QImage m_base::circle(qreal imageSize, const QColor &color)
{
   QImage image(imageSize, imageSize, QImage::Format_ARGB32);
   QPainter painter;

   painter.begin(&image);

   painter.setPen(color);
   painter.setBrush(color);

   // QColor c2 = color;
   // c2.setAlpha(128);
   // painter.setBrush(c2);

   painter.setRenderHint(QPainter::Antialiasing, true);

   painter.drawEllipse(0, 0, imageSize, imageSize);
   painter.end();
   return image;
}

