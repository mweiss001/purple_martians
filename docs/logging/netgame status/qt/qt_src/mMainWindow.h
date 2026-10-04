#ifndef MMAINWINDOW_H
#define MMAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QSettings>
#include <QCloseEvent>
#include <QMenuBar>

#include <QDialog>
#include <QDialogButtonBox>


#include "m_base.h"
#include "mLogView.h"

#include "mSessionsWidget.h"
#include "mCurrentSessionTimelineWidget.h"

#include "mSettingsDialog.h"


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


      void menuSettings()
      {
         qDebug("menuSettings()");
         mSettingsDialog dialog;
      }


   private:

};

#endif // MMAINWINDOW_H
