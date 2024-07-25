#ifndef MMAINWINDOW_H
#define MMAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QSettings>
#include <QCloseEvent>
#include <QMenuBar>

#include "m_base.h"
#include "mLogView.h"

#include "mSessionsWidget.h"
#include "mCurrentSessionTimelineWidget.h"




class mMainWindow : public QMainWindow
{
   Q_OBJECT

   public:
      mMainWindow(QWidget *parent = nullptr);
       ~mMainWindow() { };
      void writeSettings();
      void readSettings();
      void closeEvent(QCloseEvent *event);

   private slots:
      void menuUpdateChartTheme()
      {
         qDebug("menuUpdateChartTheme()");
         mbase.mChartsWidgetChangeThemeFunction();
      }
      void menuResetChartSize()
      {
         qDebug("menuResetChartSize()");
         mbase.mChartsWidgetResetSplitterFunction();
      }


      void menuDefaultFilters()
      {
         qDebug("menuDefaultFilters()");
         mbase.setDefaultFilters(1);
      }



   private:

};

#endif // MMAINWINDOW_H
