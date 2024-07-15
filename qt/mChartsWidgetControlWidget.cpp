#include "mChartsWidgetControlWidget.h"

mChartsWidgetControlWidget::mChartsWidgetControlWidget(QWidget *parent)
    : QWidget{parent}
{
   connect(&mbase, SIGNAL(updateLegendSignal()), this, SLOT(updateLegend()));



   // -------------------------------------------------------------
   // - top level grid
   // -------------------------------------------------------------
/*
   row, col (y, x)
   0, 0 chart selection
   0, 1 legend
   1, 0, 1, 2 form layout

*/

   // top level group box to contain everything
   QGroupBox* topGroupBox = new QGroupBox("Charts Controls", this);
   topGroupBox->setGeometry(10, 10, HSIZE-20, VSIZE-20);

   // grid layout for topGroupBox
   QGridLayout* topGridLayout = new QGridLayout;
   topGroupBox->setLayout(topGridLayout);

   // -------------------------------------------------------------
   // - chart selection
   // -------------------------------------------------------------

   // group box for chart selection
   QGroupBox * chartSelGroupBox = new QGroupBox("Visible Charts", this);
   chartSelGroupBox->setMaximumHeight(160);

   // add to top layout grid
   topGridLayout->addWidget(chartSelGroupBox, 0, 0);

   // layout for chartSelGroupBox
   QVBoxLayout * chartSelVBoxLayout = new QVBoxLayout;
   chartSelGroupBox->setLayout(chartSelVBoxLayout);


   // all and none buttons
   // -----------------------------------------------------

   // create a hbox for all and none buttons
   QHBoxLayout *anbHBoxLayout = new QHBoxLayout;

   // add it to its parent
   chartSelVBoxLayout->addLayout(anbHBoxLayout);

   // create all and none buttons and connections
   QPushButton * chartSelAll  = new QPushButton("all", this);
   QPushButton * chartSelNone  = new QPushButton("none", this);
   connect(chartSelAll, SIGNAL (clicked()), this, SLOT (chartSelAllClicked()));
   connect(chartSelNone, SIGNAL (clicked()), this, SLOT (chartSelNoneClicked()));

   // add buttons to hbox layout
   anbHBoxLayout->addWidget(chartSelNone);
   anbHBoxLayout->addWidget(chartSelAll);


   // chart checkboxes
   // -----------------------------------------------------

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



   // -------------------------------------------------------------
   // - legend
   // -------------------------------------------------------------

   // group box for legend
   QGroupBox * legendGroupBox = new QGroupBox("Legend", this);
   legendGroupBox->setMaximumHeight(160);

   // add to top layout grid
   topGridLayout->addWidget(legendGroupBox, 0, 1);

   // grid layout for legendGroupBox
   QGridLayout* legendGridLayout = new QGridLayout;
   legendGroupBox->setLayout(legendGridLayout);

   row = 0;
   col = 0;

   for (int i=0; i<8; i++)
   {
      legendSel[i] = new QCheckBox(mbase.statChartSeriesStructArray[i].name, this);

      QPalette p = legendSel[i]->palette();
      p.setColor(QPalette::Active, QPalette::WindowText, mbase.statChartSeriesStructArray[i].col);
      legendSel[i]->setPalette(p);

      legendSel[i]->setVisible(false);
      connect(legendSel[i], SIGNAL (clicked()), this, SLOT (legendSelClicked())) ;

      legendGridLayout->addWidget(legendSel[i], row, col);
      if (++row > 3) { row = 0;  col++; }
   }





   // group box for form layout
   QGroupBox * formLayoutGroupBox = new QGroupBox("Settings", this);

   // add to top layout grid
   topGridLayout->addWidget(formLayoutGroupBox, 1, 0, 1, 2);

   // create form layout
   QFormLayout *formLayout = new QFormLayout(this);
   formLayout->setLabelAlignment(Qt::AlignRight);

   // apply layout to formLayoutGroupBox
   formLayoutGroupBox->setLayout(formLayout);

   // -------------------------------------------------------------
   // - theme combo box
   // -------------------------------------------------------------

   // create theme combo box
   QComboBox * themeComboBox = new QComboBox;
   themeComboBox->addItem("Light", QChart::ChartThemeLight);
   themeComboBox->addItem("Blue Cerulean", QChart::ChartThemeBlueCerulean);
   themeComboBox->addItem("Dark", QChart::ChartThemeDark);
   themeComboBox->addItem("Brown Sand", QChart::ChartThemeBrownSand);
   themeComboBox->addItem("Blue NCS", QChart::ChartThemeBlueNcs);
   themeComboBox->addItem("High Contrast", QChart::ChartThemeHighContrast);
   themeComboBox->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
   themeComboBox->addItem("Qt", QChart::ChartThemeQt);

   // get initial state from settings
   mbase.mChartsWidgetChartTheme = mbase.settings->value("mChartsWidgetChartTheme", 0).toInt();
   themeComboBox->setCurrentIndex(mbase.mChartsWidgetChartTheme);

   connect(themeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(themeComboBoxChanged(int)));

   formLayout->addRow(tr("Theme:"), themeComboBox);




   // -------------------------------------------------------------
   // - Series colors combobox
   // -------------------------------------------------------------

   // create combobox for series colors
   QComboBox * seriesColorsComboBox = new QComboBox;
   seriesColorsComboBox->addItem("From Theme", 0);
   seriesColorsComboBox->addItem("Original", 1);

   // get initial state from settings
   mbase.mChartsWidgetForceMySeriesColors = mbase.settings->value("mChartsWidgetForceMySeriesColors", true).toBool();
   seriesColorsComboBox->setCurrentIndex(mbase.mChartsWidgetForceMySeriesColors);

   connect(seriesColorsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(seriesColorsComboBoxChanged(int)));
   formLayout->addRow(tr("Series Colors:"), seriesColorsComboBox);


   // -------------------------------------------------------------
   // - line size
   // -------------------------------------------------------------

   // Create spinbox to modify line size
   QSpinBox * lineSizeSpinBox = new QSpinBox(this);
   lineSizeSpinBox->setRange(0, 4);

   // get initial value from settings
   mbase.mChartsWidgetPlotLineSize = mbase.settings->value("mChartsWidgetPlotLineSize", 1).toInt();
   lineSizeSpinBox->setValue(mbase.mChartsWidgetPlotLineSize);

   connect(lineSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(lineSizeChanged(int)));
   formLayout->addRow(tr("Plot Line Size:"), lineSizeSpinBox);


   // -------------------------------------------------------------
   // - x axis frame
   // -------------------------------------------------------------

   // create a axis frame combo box
   QComboBox * xAxisFrameComboBox = new QComboBox;
   xAxisFrameComboBox->addItem("DateTime", 0);
   xAxisFrameComboBox->addItem("Frames", 1);

   // get initial state from settings
   mbase.mChartsWidgetXAxisFrame = mbase.settings->value("mChartsWidgetXAxisFrame", true).toInt();
   xAxisFrameComboBox->setCurrentIndex(mbase.mChartsWidgetXAxisFrame);

   connect(xAxisFrameComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(xAxisFrameComboBoxChanged(int)));
   formLayout->addRow(tr("X Axis Format:"), xAxisFrameComboBox);

   // -------------------------------------------------------------
   // - sql LIMIT clause
   // -------------------------------------------------------------

   // Create spinbox to modify sql Limit
   QSpinBox * sqlLimitSpinBox = new QSpinBox(this);
   sqlLimitSpinBox->setRange(100, 2000);
   sqlLimitSpinBox->setSingleStep(100);

   // get initial value from settings
   mbase.mChartsWidgetSqlModelLimit = mbase.settings->value("mChartsWidgetSqlModelLimit", 200).toInt();
   sqlLimitSpinBox->setValue(mbase.mChartsWidgetSqlModelLimit);

   connect(sqlLimitSpinBox, SIGNAL(valueChanged(int)), this, SLOT(sqlLimitChanged(int)));

   // add spinbox to layout
   formLayout->addRow(tr("SQL Model Limit:"), sqlLimitSpinBox);

   // -------------------------------------------------------------
   // - model x axis label
   // -------------------------------------------------------------

   xAxisModel = new QLabel("-", this);
   formLayout->addRow(tr("Model Range:"), xAxisModel);
}

void mChartsWidgetControlWidget::xAxisFrameComboBoxChanged(int val)
{
   mbase.mChartsWidgetXAxisFrame = val;
   mbase.settings->setValue("mChartsWidgetXAxisFrame", mbase.mChartsWidgetXAxisFrame);
   mbase.mChartsWidgetUpdateFunction();
}

void mChartsWidgetControlWidget::seriesColorsComboBoxChanged(int val)
{
   mbase.mChartsWidgetForceMySeriesColors = val;
   mbase.settings->setValue("mChartsWidgetForceMySeriesColors", mbase.mChartsWidgetForceMySeriesColors);
   mbase.mChartsWidgetChangeThemeFunction();
}

void mChartsWidgetControlWidget::themeComboBoxChanged(int index)
{
   mbase.mChartsWidgetChartTheme = index;
   mbase.settings->setValue("mChartsWidgetChartTheme", mbase.mChartsWidgetChartTheme);
   mbase.mChartsWidgetChangeThemeFunction();
}

void mChartsWidgetControlWidget::updateLegend()
{
//   qDebug() << "void mChartsWidgetControlWidget::updateLegend()";
   for (int i=0; i<8; i++)
   {
      const QString ss = "QCheckBox {  color: %1 }";
      legendSel[i]->setStyleSheet(legendSel[i]->styleSheet() + ss.arg(mbase.statChartSeriesStructArray[i].col.name()));

      if (mbase.statChartSeriesStructArray[i].active)
      {
         legendSel[i]->setVisible(true);
         legendSel[i]->setChecked(mbase.statChartSeriesStructArray[i].visible);
      }
      else legendSel[i]->setVisible(false);
   }

   double d_rng = mbase.mChartsWidgetModelXAxisDateTimeEnd.toMSecsSinceEpoch() - mbase.mChartsWidgetModelXAxisDateTimeStart.toMSecsSinceEpoch();
   char dmsg[100];
   sprintf(dmsg, "%.1fs  -  ", d_rng/1000);

   int f_rng = mbase.mChartsWidgetModelXAxisFrameEnd - mbase.mChartsWidgetModelXAxisFrameStart;
   char fmsg[100];
   sprintf(fmsg, "%d frames", f_rng);

   QString txt = dmsg;
   txt += fmsg;
   xAxisModel->setText(txt);
}

void mChartsWidgetControlWidget::legendSelClicked()
{
   //qDebug() << "legend sel clicked";
   for (int i=0; i<8; i++)
   {
      if (mbase.statChartGraphTypeArray[i].active)
         mbase.statChartSeriesStructArray[i].visible = legendSel[i]->isChecked();
   }
   mbase.mChartsWidgetControlsChangedFunction();
}

void mChartsWidgetControlWidget::set_array_from_cb(void)
{
   for (int i=0; i<NUM_CHARTS; i++)
      if (mbase.statChartGraphTypeArray[i].active) mbase.statChartGraphTypeArray[i].visible = chartSel[i]->checkState();
}

void mChartsWidgetControlWidget::chartSelAllClicked()
{
   //qDebug() << "chart sel all";
   for (int i=0; i<NUM_CHARTS; i++)
      if (mbase.statChartGraphTypeArray[i].active) chartSel[i]->setChecked(true);
}

void mChartsWidgetControlWidget::chartSelNoneClicked()
{
   //qDebug() << "chart sel none";
   for (int i=0; i<NUM_CHARTS; i++)
      if (mbase.statChartGraphTypeArray[i].active) chartSel[i]->setChecked(false);
}

void mChartsWidgetControlWidget::chartSelToggle(Qt::CheckState)
{
   //qDebug() << "chart sel toggle";
   set_array_from_cb();
   mbase.mChartsWidgetControlsChangedFunction();
}

void mChartsWidgetControlWidget::lineSizeChanged(int val)
{
   //qDebug() << "Line Size Changed" << val;
   mbase.mChartsWidgetPlotLineSize = val;
   mbase.settings->setValue("mChartsWidgetPlotLineSize", mbase.mChartsWidgetPlotLineSize);
   mbase.mChartsWidgetControlsChangedFunction();
}

void mChartsWidgetControlWidget::sqlLimitChanged(int val)
{
   qDebug() << "sql Limit Changed:" << val;
   mbase.mChartsWidgetSqlModelLimit = val;
   mbase.settings->setValue("mChartsWidgetSqlModelLimit", mbase.mChartsWidgetSqlModelLimit);
}

















