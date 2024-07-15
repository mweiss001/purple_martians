#include "mTablesWidgetControlWidget.h"

mTablesWidgetControlWidget::mTablesWidgetControlWidget(QWidget *parent) : QWidget{parent}
{

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
   QGroupBox* topGroupBox = new QGroupBox("Tables Controls", this);
   topGroupBox->setGeometry(10, 10, HSIZE-20, VSIZE-20);

   // grid layout for topGroupBox
   QGridLayout* topGridLayout = new QGridLayout;
   topGroupBox->setLayout(topGridLayout);


   // message line type selector
   mTableRowSelectWidgetInstance = new mTableRowSelectWidget(this);

   // add to top layout grid
   topGridLayout->addWidget(mTableRowSelectWidgetInstance, 0, 0);



   // column select widget
   mTableColumnSelectWidgetInstance = new mTableColumnSelectWidget(this);

   // add to top layout grid
   topGridLayout->addWidget(mTableColumnSelectWidgetInstance, 0, 1);





   // group box for form layout
   QGroupBox * formLayoutGroupBox = new QGroupBox(this);

   // add to top layout grid
   topGridLayout->addWidget(formLayoutGroupBox, 1, 0, 1, 2);

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
   mbase.mTablesWidgetFontSize = mbase.settings->value("mTablesWidgetFontSize", 10).toInt();
   mbase.setFont();
   fontSizeSpinBox->setValue(mbase.mTablesWidgetFontSize);

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
}



void mTablesWidgetControlWidget::fontSizeChanged(int val)
{
   mbase.mTablesWidgetFontSize = val;
   mbase.setFont();
   mbase.settings->setValue("mTablesWidgetFontSize", mbase.mTablesWidgetFontSize);
   mbase.mTablesWidgetFontChangeFunction();
}

void mTablesWidgetControlWidget::sqlLimitChanged(int val)
{
   qDebug() << "sql Limit Changed:" << val;
   mbase.mTablesWidgetSqlModelLimit = val;
   mbase.settings->setValue("mTablesWidgetSqlModelLimit", mbase.mTablesWidgetSqlModelLimit);
   mbase.mTablesWidgetUpdateFunction();
}













