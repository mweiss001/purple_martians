#ifndef MCHARTSWIDGETCONTROLWIDGET_H
#define MCHARTSWIDGETCONTROLWIDGET_H

#include <QWidget>
#include <QBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>
#include <QChart>

#include <QLabel>


#include "m_base.h"

class mChartsWidgetControlWidget : public QWidget
{
   Q_OBJECT
   public:
      explicit mChartsWidgetControlWidget(QWidget *parent = nullptr);
      int HSIZE = 320;
      int VSIZE = 380;
      QSize minimumSizeHint() const {      return QSize(HSIZE, VSIZE);   }
      QSize minimumSize () const    {      return QSize(HSIZE, VSIZE);   }

   public slots:
      void lineSizeChanged(int);
      void chartSelClicked();
      void chartSelNoneClicked();
      void chartSelAllClicked();
      void legendSelClicked();
      void update();
      void themeComboBoxChanged(int);
      void seriesColorsComboBoxChanged(int);
      void sqlLimitChanged(int);

   private:
      void set_array_from_cb(void);
      QCheckBox * chartSel[12];
      QCheckBox * legendSel[8];
      QLabel * xAxisModel;
};

#endif // MCHARTSWIDGETCONTROLWIDGET_H
