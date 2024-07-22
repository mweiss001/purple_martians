#ifndef MTABLEVIEWOVERLAY_H
#define MTABLEVIEWOVERLAY_H

#include <QTableView>
#include <QGraphicsItem>
#include <QPainter>
#include <QFont>

#include "m_base.h"

class mTableViewOverlay : public QGraphicsItem
{
   public:


      mTableViewOverlay(QTableView * table) : QGraphicsItem() , m_table(table) { }

      QRectF boundingRect() const override;
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  private:

      QFont m_font;
      QTableView *m_table = nullptr;



};

#endif // MTABLEVIEWOVERLAY_H
