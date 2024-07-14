#ifndef MCHARTSWIDGETCONTROLWIDGET_H
#define MCHARTSWIDGETCONTROLWIDGET_H

#include <QWidget>
#include <QBoxLayout>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>
#include <QChart>


#include "m_base.h"


class mChartsWidgetControlWidget : public QWidget
{
   Q_OBJECT
public:
   explicit mChartsWidgetControlWidget(QWidget *parent = nullptr);

   int HSIZE = 320;
   int VSIZE = 300;
   QSize minimumSizeHint() const {      return QSize(HSIZE, VSIZE);   }
   QSize minimumSize () const    {      return QSize(HSIZE, VSIZE);   }

public slots:
   void lineSizeChanged();
   void chartSelToggle(Qt::CheckState);
   void chartSelNoneClicked();
   void chartSelAllClicked();
   void legendSelClicked();
   void updateLegend();


   void themeComboBoxChanged(int index);



signals:

private slots:

private:
   void set_array_from_cb(void);
   QDoubleSpinBox * lineSizeSpinBox;
   QCheckBox * chartSel[12];
   QPushButton * chartSelAll;
   QPushButton * chartSelNone;
   QCheckBox * legendSel[8];

   QComboBox * themeComboBox;



};

#endif // MCHARTSWIDGETCONTROLWIDGET_H
