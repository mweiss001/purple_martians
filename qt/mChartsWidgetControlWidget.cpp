#include "mChartsWidgetControlWidget.h"

mChartsWidgetControlWidget::mChartsWidgetControlWidget(QWidget *parent)
    : QWidget{parent}
{
   connect(&mbase, SIGNAL(mChartsWidgetControlWidgetUpdateSignal()), this, SLOT(update()));

   // create topLayout and apply to this->
   QHBoxLayout *topLayout = new QHBoxLayout;
   this->setLayout(topLayout);
   topLayout->setContentsMargins(0, 0, 0, 0);

   // create groupBox and add to topLayout
   QGroupBox* topGroupBox = new QGroupBox("Charts Controls");

   topLayout->addWidget(topGroupBox);
   topGroupBox->setObjectName("t2");
   QString style = "QGroupBox#t2 { border: 1px solid " + mbase.mChartsWidgetColor.name();
   style += "; margin: 0px; margin-top:4px; background-color:" + mbase.mChartsWidgetColor.lighter(195).name();
   style += "; } QGroupBox#t2::title { left: 10px; top: -8px; }";
   topGroupBox->setStyleSheet(style);

   // -------------------------------------------------------------
   // - top level grid
   // -------------------------------------------------------------
/*
   row, col (y, x)
   0, 0 chart selection
   0, 1 legend
   1, 0, 1, 2 form layout
*/

   // grid layout for topGroupBox
   QGridLayout* topGridLayout = new QGridLayout;
   topGroupBox->setLayout(topGridLayout);

   // -------------------------------------------------------------
   // - chart selection
   // -------------------------------------------------------------

   // group box for chart selection
   QGroupBox * chartSelGroupBox = new QGroupBox("Visible Charts");
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
   QPushButton * chartSelAll  = new QPushButton("all");
   QPushButton * chartSelNone  = new QPushButton("none");
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
         chartSel[i] = new QCheckBox(mbase.statChartGraphTypeArray[i].db_name);
         chartSel[i]->setChecked(mbase.statChartGraphTypeArray[i].visible);
         chartsCheckBoxesGridLayout->addWidget(chartSel[i], row, col);
         if (++row > 3) { row = 0;  col++; }
         connect(chartSel[i], SIGNAL(clicked()), this, SLOT (chartSelClicked())) ;
      }
   }



   // -------------------------------------------------------------
   // - legend
   // -------------------------------------------------------------

   // group box for legend
   QGroupBox * legendGroupBox = new QGroupBox("Legend");
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
      legendSel[i] = new QCheckBox(mbase.statChartSeriesStructArray[i].name);
      legendSel[i]->setVisible(false);
      connect(legendSel[i], SIGNAL (clicked()), this, SLOT (legendSelClicked())) ;
      legendGridLayout->addWidget(legendSel[i], row, col);
      if (++row > 3) { row = 0;  col++; }
   }


   // group box for form layout
   QGroupBox * formLayoutGroupBox = new QGroupBox("Settings");
   formLayoutGroupBox->setMaximumHeight(160);



   // add to top layout grid
   topGridLayout->addWidget(formLayoutGroupBox, 1, 0, 1, 2);

   // create form layout
   QFormLayout *formLayout = new QFormLayout();
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
   QSpinBox * lineSizeSpinBox = new QSpinBox();
   lineSizeSpinBox->setRange(0, 4);

   // get initial value from settings
   mbase.mChartsWidgetPlotLineSize = mbase.settings->value("mChartsWidgetPlotLineSize", 1).toInt();
   lineSizeSpinBox->setValue(mbase.mChartsWidgetPlotLineSize);

   connect(lineSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(lineSizeChanged(int)));
   formLayout->addRow(tr("Plot Line Size:"), lineSizeSpinBox);



   // -------------------------------------------------------------
   // - sql LIMIT clause
   // -------------------------------------------------------------

   // Create spinbox to modify sql Limit
   QSpinBox * sqlLimitSpinBox = new QSpinBox();
   sqlLimitSpinBox->setRange(100, 2000);
   sqlLimitSpinBox->setSingleStep(100);

   // get initial value from settings
   mbase.mChartsWidgetSqlModelLimit = mbase.settings->value("mChartsWidgetSqlModelLimit", 200).toInt();
   sqlLimitSpinBox->setValue(mbase.mChartsWidgetSqlModelLimit);

   connect(sqlLimitSpinBox, SIGNAL(valueChanged(int)), this, SLOT(sqlLimitChanged(int)));

   // add spinbox to layout
   formLayout->addRow(tr("SQL Model Limit:"), sqlLimitSpinBox);

   // -------------------------------------------------------------
   // - model x axis range label
   // -------------------------------------------------------------

   xAxisModel = new QLabel("-", this);
   formLayout->addRow(tr("Model Range:"), xAxisModel);
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

void mChartsWidgetControlWidget::update()
{
//   qDebug() << "void mChartsWidgetControlWidget::update()";

   // update legend
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

   // update the model range label
   double d_rng = mbase.mChartsWidgetModelXAxisDateTimeEnd.toMSecsSinceEpoch() - mbase.mChartsWidgetModelXAxisDateTimeStart.toMSecsSinceEpoch();
   char dmsg[100];
   sprintf(dmsg, "%.1fs", d_rng/1000);
   xAxisModel->setText(dmsg);

}

void mChartsWidgetControlWidget::legendSelClicked()
{
   //qDebug() << "void mChartsWidgetControlWidget::legendSelClicked()";
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
   //qDebug() << "void mChartsWidgetControlWidget::chartSelAllClicked()"
   for (int i=0; i<NUM_CHARTS; i++)
      if (mbase.statChartGraphTypeArray[i].active) chartSel[i]->setChecked(true);
   chartSelClicked();
}

void mChartsWidgetControlWidget::chartSelNoneClicked()
{
   //qDebug() << "void mChartsWidgetControlWidget::chartSelNoneClicked()";
   for (int i=0; i<NUM_CHARTS; i++)
      if (mbase.statChartGraphTypeArray[i].active) chartSel[i]->setChecked(false);
   chartSelClicked();
}

void mChartsWidgetControlWidget::chartSelClicked()
{
   //qDebug() << "void mChartsWidgetControlWidget::chartSelClicked()";
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
   // qDebug() << "sql Limit Changed:" << val;
   mbase.mChartsWidgetSqlModelLimit = val;
   mbase.settings->setValue("mChartsWidgetSqlModelLimit", mbase.mChartsWidgetSqlModelLimit);
   mbase.mChartsWidgetReloadModelFunction();
}





