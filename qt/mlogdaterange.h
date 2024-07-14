#ifndef MLOGDATERANGE_H
#define MLOGDATERANGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>

#include "m_base.h"


class MLogDateRange : public QWidget
{
   Q_OBJECT
public:
   explicit MLogDateRange(QWidget *parent = nullptr);
   int VSIZE = 200;
   int HSIZE = 300;
   QSize minimumSizeHint() const   { return QSize(VSIZE, HSIZE); }
   QSize minimumSize () const      { return QSize(VSIZE, HSIZE); }
   void set_sql_where_date_range(void);

signals:


private slots:
   void on_pb_beg_earliest_clicked();
   void on_pb_end_latest_clicked();
   void on_dte_changed(const QDateTime &dateTime);

private:
   QDateTimeEdit * dte_beg;
   QDateTimeEdit * dte_end;
   QPushButton * pb_beg_earliest;
   QPushButton * pb_end_latest;
   QLabel * mldr_lb_range;
   QGroupBox * mldr_gb;
   QGroupBox * mldrb_gb;
   QGroupBox * mldre_gb;
   QGroupBox * mldrr_gb;
};

#endif // MLOGDATERANGE_H
