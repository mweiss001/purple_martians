#ifndef MSESSIONSWIDGETTABLEVIEWDELEGATE_H
#define MSESSIONSWIDGETTABLEVIEWDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QDateTime>

class SessionsWidgetTableViewDelegate : public QStyledItemDelegate
{
   Q_OBJECT

   public:
   SessionsWidgetTableViewDelegate() {}
   using QStyledItemDelegate::QStyledItemDelegate;

   void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
   {
      QStyleOptionViewItem myOption = option;
      if (index.column() == 2) myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter;
      if (index.column() == 3) myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter;
      if (index.column() == 4) myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter;

      QStyledItemDelegate::paint(painter, myOption, index);
   }

   QString displayText(const QVariant& value, const QLocale& locale) const override
   {
      if (value.typeId() == QMetaType::QDateTime) return value.toDateTime().toString("yyyy-MM-dd HH:mm:ss");
      else return QStyledItemDelegate::displayText(value, locale);
   }

};

#endif // MSESSIONSWIDGETTABLEVIEWDELEGATE_H
