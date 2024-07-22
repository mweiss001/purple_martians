#ifndef MSESSIONSWIDGET_H
#define MSESSIONSWIDGET_H

#include <QWidget>

#include <QGroupBox>
#include <QBoxLayout>
#include <QLabel>

#include <QTableView>
#include <QSqlQueryModel>
#include <QHeaderView>

#include "mdelegate2.h"


class mSessionsWidget : public QWidget
{
   Q_OBJECT
   public:
      explicit mSessionsWidget(QWidget *parent = nullptr);
      int VSIZE = 600;
      int HSIZE = 280;
      QSize minimumSizeHint() const {      return QSize(VSIZE, HSIZE);   }
      QSize minimumSize () const    {      return QSize(VSIZE, HSIZE);   }

   private slots:
      void on_stv_changed(const QItemSelection &sel, const QItemSelection &dsel);

   signals:

   private:
      QGroupBox * mpss_gb;
      QTableView * sessionsTableView;
      QLabel * mpss_lb_start;
      QLabel * mpss_lb_end;
      QLabel * mpss_lb_range;
      QGroupBox * mpss_gb_start;
      QGroupBox * mpss_gb_end;
      QGroupBox * mpss_gb_range;
};
#endif // MSESSIONSWIDGET_H
