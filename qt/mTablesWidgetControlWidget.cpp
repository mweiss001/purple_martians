#include "mTablesWidgetControlWidget.h"

mTablesWidgetControlWidget::mTablesWidgetControlWidget(QWidget *parent) : QWidget{parent}
{

   connect(&mbase, SIGNAL(mTablesWidgetControlWidgetUpdateSignal()), this, SLOT(update()));



   // top level group box to contain everything
   QGroupBox* topGroupBox = new QGroupBox("Tables Controls", this);
   topGroupBox->setGeometry(10, 10, HSIZE-20, VSIZE-20);


   // hbox layout for inside main groupbox
   QHBoxLayout * hBox = new QHBoxLayout();
   topGroupBox->setLayout(hBox);


   // vbox layout for lhs
   QVBoxLayout * vBox = new QVBoxLayout();
   hBox->addLayout(vBox);

   // vbox layout for rhs
   QVBoxLayout * vBox2 = new QVBoxLayout();
   hBox->addLayout(vBox2);





   // message line type selector
   mTableRowSelectWidgetInstance = new mTableRowSelectWidget(this);

   // add to layout
   vBox->addWidget(mTableRowSelectWidgetInstance);








   // column select widget
   mTableColumnSelectWidgetInstance = new mTableColumnSelectWidget(this);

   // add to layout
   vBox2->addWidget(mTableColumnSelectWidgetInstance);

   vBox2->addStretch();

   hBox->addStretch();



   // group box for form layout
   QGroupBox * formLayoutGroupBox = new QGroupBox("Settings", this);
   formLayoutGroupBox->setMaximumHeight(100);
   formLayoutGroupBox->setMaximumWidth(200);



   vBox->addWidget(formLayoutGroupBox);


   vBox->addStretch();



   // create form layout
   QFormLayout *formLayout = new QFormLayout(this);
   formLayout->setLabelAlignment(Qt::AlignRight);

   // apply layout to formLayoutGroupBox
   formLayoutGroupBox->setLayout(formLayout);

   // -----------------------------------------------------------------------
   // font size spin box
   // -----------------------------------------------------------------------
   QSpinBox * fontSizeSpinBox = new QSpinBox();
   fontSizeSpinBox->setRange(4, 20);

   // get initial value from settings
   int fontSize = mbase.settings->value("mTablesWidgetFontSize", 10).toInt();
   mbase.mTablesWidgetFontChangeFunction(fontSize);
   fontSizeSpinBox->setValue(fontSize);

   formLayout->addRow(tr("Table Font Size:"), fontSizeSpinBox);
   connect(fontSizeSpinBox, SIGNAL (valueChanged(int)), this, SLOT (fontSizeChanged(int)));

   // -------------------------------------------------------------
   // - sql LIMIT clause
   // -------------------------------------------------------------

   // Create spinbox to modify sql Limit
   QSpinBox * sqlLimitSpinBox = new QSpinBox(this);
   sqlLimitSpinBox->setRange(100, 2000);
   sqlLimitSpinBox->setSingleStep(100);

   // get initial value from settings
   mbase.mTablesWidgetSqlModelLimit = mbase.settings->value("mTablesWidgetSqlModelLimit", 200).toInt();
   sqlLimitSpinBox->setValue(mbase.mTablesWidgetSqlModelLimit);

   connect(sqlLimitSpinBox, SIGNAL(valueChanged(int)), this, SLOT(sqlLimitChanged(int)));

   // add spinbox to layout
   formLayout->addRow(tr("SQL Model Limit:"), sqlLimitSpinBox);

   // -------------------------------------------------------------
   // - model x axis range label
   // -------------------------------------------------------------

   xAxisModel = new QLabel("-", this);
   formLayout->addRow(tr("Model Range:"), xAxisModel);






















}















   // -------------------------------------------------------------
   // - top level grid
   // -------------------------------------------------------------
   /*
   row, col (y, x)
   0, 0 chart selection
   0, 1 legend
   1, 0, 1, 2 form layout

*/

//   // top level group box to contain everything
//   QGroupBox* topGroupBox = new QGroupBox("Tables Controls", this);
//   topGroupBox->setGeometry(10, 10, HSIZE-20, VSIZE-20);



   // // grid layout for topGroupBox
   // QGridLayout* topGridLayout = new QGridLayout;
   // topGroupBox->setLayout(topGridLayout);




   // // message line type selector
   // mTableRowSelectWidgetInstance = new mTableRowSelectWidget(this);

   // // add to top layout grid
   // topGridLayout->addWidget(mTableRowSelectWidgetInstance, 0, 0);


   // // column select widget
   // mTableColumnSelectWidgetInstance = new mTableColumnSelectWidget(this);

   // // add to top layout grid
   // topGridLayout->addWidget(mTableColumnSelectWidgetInstance, 0, 1);






void mTablesWidgetControlWidget::fontSizeChanged(int val)
{
   //qDebug() << "void mTablesWidgetControlWidget::fontSizeChanged(int val)";
   mbase.mTablesWidgetFontChangeFunction(val);
   mbase.settings->setValue("mTablesWidgetFontSize", mbase.mTablesWidgetFontSize);
}

void mTablesWidgetControlWidget::sqlLimitChanged(int val)
{
   // qDebug() << "sql Limit Changed:" << val;
   mbase.mTablesWidgetSqlModelLimit = val;
   mbase.settings->setValue("mTablesWidgetSqlModelLimit", mbase.mTablesWidgetSqlModelLimit);
   mbase.mTablesWidgetUpdateFunction();
}

void mTablesWidgetControlWidget::update()
{
   //qDebug() << "void mTablesWidgetControlWidget::update()";
   // update the model range label
   double d_rng = mbase.mTablesWidgetModelXAxisDateTimeEnd.toMSecsSinceEpoch() - mbase.mTablesWidgetModelXAxisDateTimeStart.toMSecsSinceEpoch();
   char dmsg[100];
   sprintf(dmsg, "%.1fs", d_rng/1000);
   xAxisModel->setText(dmsg);
}







