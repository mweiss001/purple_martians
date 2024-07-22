#ifndef MSESSIONSTIMELINEWIDGET_H
#define MSESSIONSTIMELINEWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "m_base.h"
#include "mSessionsGraphicsItem.h"


class mSessionsTimelineWidget : public QWidget
{
   Q_OBJECT
   public:
   mSessionsTimelineWidget(QWidget *parent = nullptr);

   private slots:
   void onSelectionChanged();

   private:
   QGraphicsScene * scene;
   QGraphicsView * view;
};

#endif // MSESSIONSTIMELINEWIDGET_H





















