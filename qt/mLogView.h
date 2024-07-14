#ifndef MLOGVIEW_H
#define MLOGVIEW_H

#include <QWidget>

#include <QTableView>
#include <QSqlQueryModel>
#include <QHeaderView>

#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QDateTime>

#include "mlogmessagetypeselector.h"
#include "mpmselect.h"
#include "msqlwidget.h"
#include "mSessionsWidget.h"
#include "mChartsWidget.h"
#include "mChartsWidgetControlWidget.h"

#include "m_base.h"
#include "mdelegate.h"


class mLogView : public QWidget
{
   Q_OBJECT
public:
   explicit mLogView(QWidget *parent = nullptr);

public slots:
   void pb_exec_clicked();
   void do_sql_changed();
   void update_table();
   void update_table_hidden_columns();
   void font_size_changed(int);

signals:

private slots:
   void on_tb1_clicked(QModelIndex index);
   void on_tb2_clicked(QModelIndex index);


private:
   MLogMessageTypeSelector * mlts;
   MPMSelect * mpms;
   MSQLWidget * msqw;

   mSessionsWidget * mSessionsWidgetInstance;
   mChartsWidget * mChartsWidgetInstance;
   mChartsWidgetControlWidget * mChartsWidgetControlWidgetInstance;

   QTableView * tableView1;
   QTableView * tableView2;

   QSqlQueryModel *model;
   QCheckBox * cb_autoexec;
   QPushButton * pb_exec;
   QLabel * lb_num_rows;
   QSpinBox * sb_font_size;

   QImage rectangle(qreal imageSize, const QColor &color);
   QImage circle(qreal imageSize, const QColor &color);

};

#endif // MLOGVIEW_H
