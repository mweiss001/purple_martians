#include "mTablesWidgetControlWidget.h"

mTablesWidgetControlWidget::mTablesWidgetControlWidget(QWidget *parent) : QWidget{parent}
{
   // qDebug() << "mTablesWidgetControlWidget::constructor()";

   connect(&mbase, SIGNAL(mTablesWidgetControlWidgetUpdateSignal()), this, SLOT(updateUI()));

   // create topLayout and apply to this->
   QHBoxLayout *topLayout = new QHBoxLayout;
   topLayout->setContentsMargins(0, 0, 0, 0);
   this->setLayout(topLayout);

   // create groupBox and add to topLayout
   QGroupBox* topGroupBox = new QGroupBox("Tables Controls");
   topGroupBox->setMinimumSize(230,484);
   topGroupBox->setMaximumSize(230,484);
   topGroupBox->setObjectName("t2");
   QString style = "QGroupBox#t2 { border: 1px solid " + mbase.mTablesWidgetColor.name();
   style += "; margin: 0px; margin-top:4px; background-color:" + mbase.mTablesWidgetColor.lighter(195).name();
   style += "; } QGroupBox#t2::title { left: 10px; top: -8px; }";
   topGroupBox->setStyleSheet(style);
   topLayout->addWidget(topGroupBox);


   // create layout for topGroupBox
   QVBoxLayout *topGroupBoxLayout = new QVBoxLayout;
   topGroupBoxLayout->setContentsMargins(4, 8, 4, 4);
   topGroupBox->setLayout(topGroupBoxLayout);

   // hbox layout for row and col
   QHBoxLayout * hBoxRC = new QHBoxLayout();
   topGroupBoxLayout->addLayout(hBoxRC);

   // row select widget
   mTableRowSelectWidgetInstance = new mTableRowSelectWidget();
   hBoxRC->addWidget(mTableRowSelectWidgetInstance);

   hBoxRC->addStretch();

   // column select widget
   mTableColumnSelectWidgetInstance = new mTableColumnSelectWidget();
   hBoxRC->addWidget(mTableColumnSelectWidgetInstance);

   // group box for form layout
   QGroupBox * formLayoutGroupBox = new QGroupBox("Settings");
   topGroupBoxLayout->addWidget(formLayoutGroupBox);

   // create form layout and apply
   QFormLayout *formLayout = new QFormLayout();
   formLayout->setLabelAlignment(Qt::AlignRight);
   formLayoutGroupBox->setLayout(formLayout);

   // -----------------------------------------------------------------------
   // number of tables spin box
   // -----------------------------------------------------------------------
   numTablesSpinBox = new QSpinBox();
   numTablesSpinBox->setRange(0, 4);
   formLayout->addRow(tr("Number of Tables:"), numTablesSpinBox);
   connect(numTablesSpinBox, SIGNAL (valueChanged(int)), this, SLOT (numTablesChanged(int)));

   // -----------------------------------------------------------------------
   // show hide filter controls
   // -----------------------------------------------------------------------
   showHideFilterControls = new QPushButton("Show");
   showHideFilterControls->setCheckable(true);
   formLayout->addRow(tr("Filter Controls:"), showHideFilterControls);
   connect(showHideFilterControls, SIGNAL (toggled(bool)), this, SLOT (showHideFilterControlsToggled(bool)));

   // -----------------------------------------------------------------------
   // font size spin box
   // -----------------------------------------------------------------------
   fontSizeSpinBox = new QSpinBox();
   fontSizeSpinBox->setRange(4, 20);
   formLayout->addRow(tr("Table Font Size:"), fontSizeSpinBox);
   connect(fontSizeSpinBox, SIGNAL (valueChanged(int)), this, SLOT (fontSizeChanged(int)));

   // -------------------------------------------------------------
   // - sql LIMIT clause
   // -------------------------------------------------------------

   // Create spinbox to modify sql Limit
   sqlLimitSpinBox = new QSpinBox(this);
   sqlLimitSpinBox->setRange(100, 2000);
   sqlLimitSpinBox->setSingleStep(100);
   formLayout->addRow(tr("SQL Model Limit:"), sqlLimitSpinBox);
   connect(sqlLimitSpinBox, SIGNAL(valueChanged(int)), this, SLOT(sqlLimitChanged(int)));


   // -------------------------------------------------------------
   // - model x axis range label
   // -------------------------------------------------------------
   xAxisModel = new QLabel("-", this);
   formLayout->addRow(tr("Model Range:"), xAxisModel);

   updateUI();

}

void mTablesWidgetControlWidget::updateUI()
{
   // qDebug() << "void mTablesWidgetControlWidget::update()";


   // all of these sections should follow this format:
   // - set mbase values from setting
   // - set ui control
   // - update stuff specific to control


   // num tables spinbox
   mbase.numVisibleTables = mbase.settings->value("numVisibleTables", 2).toInt();
   numTablesSpinBox->setValue(mbase.numVisibleTables);
   numTablesChanged(mbase.numVisibleTables);

   // show/hide table filters checked pushbutton
   mbase.showFilterControls = mbase.settings->value("showFilterControls", 0).toBool();
   if (mbase.showFilterControls)
   {
      showHideFilterControls->setText("Hide");
      showHideFilterControls->setChecked(true);
   }
   else
   {
      showHideFilterControls->setText("Show");
      showHideFilterControls->setChecked(false);
   }
   showHideFilterControlsToggled(mbase.showFilterControls);

   // font size spinbox
   mbase.mTablesWidgetFontSize = mbase.settings->value("mTablesWidgetFontSize", 10).toInt();
   fontSizeSpinBox->setValue(mbase.mTablesWidgetFontSize);

   // sql limit spinbox
   mbase.mTablesWidgetSqlModelLimit = mbase.settings->value("mTablesWidgetSqlModelLimit", 200).toInt();
   sqlLimitSpinBox->setValue(mbase.mTablesWidgetSqlModelLimit);

   // update the model range label
   double d_rng = mbase.mTablesWidgetModelXAxisDateTimeEnd.toMSecsSinceEpoch() - mbase.mTablesWidgetModelXAxisDateTimeStart.toMSecsSinceEpoch();
   char dmsg[100];
   sprintf(dmsg, "%.1fs", d_rng/1000);
   xAxisModel->setText(dmsg);

}





void mTablesWidgetControlWidget::numTablesChanged(int val)
{
   // qDebug() << "void mTablesWidgetControlWidget::numTablesChanged(" << val << ")";

   // set mbase value
   mbase.numVisibleTables = val;

   // save in settings
   mbase.settings->setValue("numVisibleTables", mbase.numVisibleTables);

   // update stuff .....
   mbase.mTablesWidgetUpdateUIFunction();
}

void mTablesWidgetControlWidget::showHideFilterControlsToggled(bool checked)
{
   // set mbase value
   mbase.showFilterControls = checked;

   // save in settings
   mbase.settings->setValue("showFilterControls", mbase.showFilterControls);

   //qDebug() << "Show Hide changed to " << mbase.showFilterControls;

   // update button text
   if (checked) showHideFilterControls->setText("Hide");
   else         showHideFilterControls->setText("Show");

   // update stuff .....
   mbase.mTablesWidgetUpdateUIFunction();
}

void mTablesWidgetControlWidget::fontSizeChanged(int val)
{
   //qDebug() << "void mTablesWidgetControlWidget::fontSizeChanged(int val)";

   // set mbase value
   mbase.mTablesWidgetFontSize = val;

   // save in settings
   mbase.settings->setValue("mTablesWidgetFontSize", mbase.mTablesWidgetFontSize);

   // update stuff .....
   mbase.mTablesWidgetFontChangeFunction();
}

void mTablesWidgetControlWidget::sqlLimitChanged(int val)
{
   // qDebug() << "sql Limit Changed:" << val;

   mbase.mTablesWidgetSqlModelLimit = val;
   mbase.settings->setValue("mTablesWidgetSqlModelLimit", mbase.mTablesWidgetSqlModelLimit);
   mbase.mTablesWidgetReloadModelFunction();
}








