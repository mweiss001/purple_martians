#ifndef MDELEGATE_H
#define MDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QDateTime>
#include "m_base.h"

class mDelegate : public QStyledItemDelegate
{
   Q_OBJECT
public:
   mDelegate() {}
   using QStyledItemDelegate::QStyledItemDelegate;

   void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
   {
      // find message_type sibling in column 1
      QModelIndex qmi = index.siblingAtColumn(1);
      QVariant v = index.model()->data(qmi, Qt::DisplayRole);
      int type = v.toInt();
      painter->fillRect(option.rect, mbase.log_types[type].color);

      QStyleOptionViewItem myOption = option;

      if (index.column() == 1) myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter;
      if (index.column() == 4) myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter;
      if (index.column() == 5) myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter;

//      if (index.column() == 6) myOption.font = QFont{ "Courier", 9, QFont::Monospace };
//      myOption.font = QFont{ "Courier", mbase.font_size, QFont::Monospace };
//      myOption.font = mbase.font;

      QStyledItemDelegate::paint(painter, myOption, index);
   }

   QString displayText(const QVariant& value, const QLocale& locale) const override
   {
      if (value.typeId() == QMetaType::QDateTime) return value.toDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
      else return QStyledItemDelegate::displayText(value, locale);
   }
};

#endif // MDELEGATE_H
