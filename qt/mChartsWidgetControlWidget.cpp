#include "mChartsWidgetControlWidget.h"


mChartsWidgetControlWidget::mChartsWidgetControlWidget(QWidget *parent)
    : QWidget{parent}
{


   connect(&mbase, SIGNAL(updateLegendSignal()), this, SLOT(updateLegend()));




   // top level group box to contain everything
   QGroupBox* topGroupBox = new QGroupBox("Charts Controls", this);
   topGroupBox->setGeometry(10, 10, HSIZE-20, VSIZE-20);

   // layout for topGroupBox
//   QVBoxLayout* topVBoxLayout = new QVBoxLayout;
//   topGroupBox->setLayout(topVBoxLayout);

   // layout for topGroupBox
   QGridLayout* topGridLayout = new QGridLayout;
   topGroupBox->setLayout(topGridLayout);



   // group box for line size
   QGroupBox* lineSizeGroupBox = new QGroupBox("Line Size", this);
   lineSizeGroupBox->setMaximumHeight(60);
//   topVBoxLayout->addWidget(lineSizeGroupBox);

   topGridLayout->addWidget(lineSizeGroupBox, 0, 0);


   // layout for lineSizeGroupBox
   QVBoxLayout * lineSizeVBoxLayout = new QVBoxLayout;
   lineSizeGroupBox->setLayout(lineSizeVBoxLayout);



   // Create spinbox to modify line size
   lineSizeSpinBox = new QDoubleSpinBox(this);
   lineSizeSpinBox->setValue(1);
   lineSizeSpinBox->setRange(0, 4);

   connect(lineSizeSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
           this, &mChartsWidgetControlWidget::lineSizeChanged);


   lineSizeVBoxLayout->addWidget(lineSizeSpinBox);



   // group box for chart selection
   QGroupBox * chartSelGroupBox = new QGroupBox("Chart Selection", this);

   chartSelGroupBox->setMaximumHeight(160);

   // add Chart Selection Group Box to Top Level Group Box Layout
//   topVBoxLayout->addWidget(chartSelGroupBox);



   topGridLayout->addWidget(chartSelGroupBox, 1, 0);








   // create layout for Chart Selection Group Box
   QVBoxLayout * chartSelVBoxLayout = new QVBoxLayout;

   // set layout for Chart Selection Group Box
   chartSelGroupBox->setLayout(chartSelVBoxLayout);



   // create all and none buttons


   // create a hbox for all and none buttons
   QHBoxLayout *anbHBoxLayout = new QHBoxLayout;

   // add it to its parent
   chartSelVBoxLayout->addLayout(anbHBoxLayout);

   // create all and none buttons and connections
   chartSelAll  = new QPushButton("all", this);
   chartSelNone  = new QPushButton("none", this);
   connect(chartSelAll, SIGNAL (clicked()), this, SLOT (chartSelAllClicked()));
   connect(chartSelNone, SIGNAL (clicked()), this, SLOT (chartSelNoneClicked()));

   // add buttons to hbox layout
   anbHBoxLayout->addWidget(chartSelNone);
   anbHBoxLayout->addWidget(chartSelAll);




   // for (int i=0; i<NUM_CHARTS; i++)
   // {
   //    if (mbase.statChartGraphTypeArray[i].active)
   //    {
   //       chartSel[i] = new QCheckBox(mbase.statChartGraphTypeArray[i].db_name, this);
   //       chartSel[i]->setChecked(mbase.statChartGraphTypeArray[i].visible);
   //       chartSelVBoxLayout->addWidget(chartSel[i]);
   //       connect(chartSel[i], SIGNAL (checkStateChanged(Qt::CheckState)), this, SLOT (chartSelToggle(Qt::CheckState))) ;
   //    }
   // }


   // can I put these in a grid??



   // create a grid layout for charts checkboxes
   QGridLayout* chartsCheckBoxesGridLayout = new QGridLayout;

   // add it to its parent
   chartSelVBoxLayout->addLayout(chartsCheckBoxesGridLayout);

   int row = 0;
   int col = 0;

   for (int i=0; i<NUM_CHARTS; i++)
   {
      if (mbase.statChartGraphTypeArray[i].active)
      {
         chartSel[i] = new QCheckBox(mbase.statChartGraphTypeArray[i].db_name, this);
         chartSel[i]->setChecked(mbase.statChartGraphTypeArray[i].visible);
         chartsCheckBoxesGridLayout->addWidget(chartSel[i], row, col);
         if (++row > 3) { row = 0;  col++; }
         connect(chartSel[i], SIGNAL (checkStateChanged(Qt::CheckState)), this, SLOT (chartSelToggle(Qt::CheckState))) ;
      }
   }



   // legend ----------------------------------------


   // group box for legend
   QGroupBox * legendGroupBox = new QGroupBox("Legend", this);

   legendGroupBox->setMaximumHeight(160);

   // add Legend Group Box to Top Level Group Box Layout
//   topVBoxLayout->addWidget(legendGroupBox);
   topGridLayout->addWidget(legendGroupBox, 0, 1, 2, 1);




// create layout for Chart Selection Group Box
   QVBoxLayout * legendVBoxLayout = new QVBoxLayout;

// set layout for Chart Selection Group Box
   legendGroupBox->setLayout(legendVBoxLayout);


  // // create a grid layout for charts checkboxes
  //   QGridLayout* legendGridLayout = new QGridLayout;

  //  // add it to its parent
  //  legendGroupBox->setLayout(legendGridLayout);

  //  row = 0;
  //  col = 0;


   for (int i=0; i<8; i++)
   {
      legendSel[i] = new QCheckBox(mbase.statChartSeriesStructArray[i].name, this);
      QPalette p = legendSel[i]->palette();

      QColor col = mbase.statChartSeriesStructArray[i].col;

      // see if I can set color from series
      // that way if I have theme, the legend will have the same colors

      p.setColor(QPalette::Active, QPalette::WindowText, col);
      legendSel[i]->setPalette(p);
      legendSel[i]->setVisible(false);
      connect(legendSel[i], SIGNAL (clicked()), this, SLOT (legendSelClicked())) ;

//      legendGridLayout->addWidget(legendSel[i], row, col);
//      if (++col > 3) { col = 0;  row++; }

      legendVBoxLayout->addWidget(legendSel[i]);
   }




   themeComboBox = new QComboBox;

   themeComboBox->addItem("Light", QChart::ChartThemeLight);
   themeComboBox->addItem("Blue Cerulean", QChart::ChartThemeBlueCerulean);
   themeComboBox->addItem("Dark", QChart::ChartThemeDark);
   themeComboBox->addItem("Brown Sand", QChart::ChartThemeBrownSand);
   themeComboBox->addItem("Blue NCS", QChart::ChartThemeBlueNcs);
   themeComboBox->addItem("High Contrast", QChart::ChartThemeHighContrast);
   themeComboBox->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
   themeComboBox->addItem("Qt", QChart::ChartThemeQt);

   connect(themeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(themeComboBoxChanged(int)));


   topGridLayout->addWidget(themeComboBox, 2, 0, 1, 1);



}


void mChartsWidgetControlWidget::themeComboBoxChanged(int index)
{
   mbase.mChartsWidgetChartTheme = index;
   mbase.updateChartThemeFunction();
}




void mChartsWidgetControlWidget::updateLegend()
{
   qDebug() << "void mChartsWidgetControlWidget::updateLegend()";

   for (int i=0; i<8; i++)
   {
      if (mbase.statChartSeriesStructArray[i].active)
      {
         legendSel[i]->setVisible(true);
         legendSel[i]->setChecked(mbase.statChartSeriesStructArray[i].visible);
      }
      else legendSel[i]->setVisible(false);
   }
}

void mChartsWidgetControlWidget::legendSelClicked()
{
   qDebug() << "legend sel clicked";
   for (int i=0; i<8; i++)
   {
      if (mbase.statChartGraphTypeArray[i].active)
         mbase.statChartSeriesStructArray[i].visible = legendSel[i]->isChecked();
   }
   mbase.chartsWidgetsControlsChangedFunction();
}






void mChartsWidgetControlWidget::set_array_from_cb(void)
{
   for (int i=0; i<NUM_CHARTS; i++)
      if (mbase.statChartGraphTypeArray[i].active) mbase.statChartGraphTypeArray[i].visible = chartSel[i]->checkState();
}


void mChartsWidgetControlWidget::chartSelAllClicked()
{
   qDebug() << "chart sel all";
   for (int i=0; i<NUM_CHARTS; i++)
      if (mbase.statChartGraphTypeArray[i].active) chartSel[i]->setChecked(true);
}

void mChartsWidgetControlWidget::chartSelNoneClicked()
{
   qDebug() << "chart sel none";
   for (int i=0; i<NUM_CHARTS; i++)
      if (mbase.statChartGraphTypeArray[i].active) chartSel[i]->setChecked(false);
}

void mChartsWidgetControlWidget::chartSelToggle(Qt::CheckState)
{
   qDebug() << "chart sel toggle";
   set_array_from_cb();
   mbase.chartsWidgetsControlsChangedFunction();
}




void mChartsWidgetControlWidget::lineSizeChanged()
{
   //qDebug() << "Line Size Changed" << lineSizeSpinBox->value();
   mbase.mChartsWidgetPlotLineSize = lineSizeSpinBox->value();
   mbase.chartsWidgetsControlsChangedFunction();
}
