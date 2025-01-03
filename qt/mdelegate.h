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
      int type = index.model()->data(qmi, Qt::DisplayRole).toInt();

      // use message_type to set background color
      painter->fillRect(option.rect, mbase.log_types[type].color);


      QStyleOptionViewItem myOption = option;
      myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter;

      if (index.column() == 8) myOption.displayAlignment = Qt::AlignLeft   | Qt::AlignVCenter; // message

      /*
      if (index.column() == 1) myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter; // msg_type
      if (index.column() == 4) myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter; // agt
      if (index.column() == 5) myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter; // frame
      if (index.column() == 6) myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter; // player AS p
      if (index.column() == 7) myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter; // client AS c
      if (index.column() == 8) myOption.displayAlignment = Qt::AlignLeft   | Qt::AlignVCenter; // message

*/

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
