#ifndef MSETTINGSDIALOG_H
#define MSETTINGSDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QBoxLayout>
#include <QTabWidget>

#include <QFormLayout>

#include <QGroupBox>

#include <QSpinBox>
#include <QPushButton>

#include <QLineEdit>

#include "m_base.h"


class mSettingsDialog : public QDialog
{
   Q_OBJECT


   public:
      mSettingsDialog()
      {
         connect(this, SIGNAL(accepted()), this, SLOT(accpt()));

         setWindowTitle(tr("Settings"));

         QTabWidget * tabWidget = new QTabWidget;

         // widget for tab
         auto w = new QWidget;

         // create form layout and apply
         QFormLayout *formLayout = new QFormLayout();
         formLayout->setLabelAlignment(Qt::AlignRight);
         w->setLayout(formLayout);

         dbHostNameLineEdit = new QLineEdit(mbase.dbHostName);
         formLayout->addRow(tr("Database Hostname:"), dbHostNameLineEdit);

         dbDatabaseNameLineEdit = new QLineEdit(mbase.dbDatabaseName);
         formLayout->addRow(tr("Database Name:"), dbDatabaseNameLineEdit);

         dbUserNameLineEdit = new QLineEdit(mbase.dbUserName);
         formLayout->addRow(tr("Database User Name:"), dbUserNameLineEdit);

         dbPasswordLineEdit = new QLineEdit(mbase.dbPassword);
         formLayout->addRow(tr("Database Password:"), dbPasswordLineEdit);


         tabWidget->addTab(w, tr("Database"));

         QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

         connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
         connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);


         QVBoxLayout *mainLayout = new QVBoxLayout;

         mainLayout->addWidget(tabWidget);

         mainLayout->addWidget(buttonBox);
         setLayout(mainLayout);


         this->exec();


      }




   public slots:
      void accpt()
      {
         // qDebug() << "accpt";

         // save in settings
         mbase.settings->setValue("dbHostName", dbHostNameLineEdit->text());
         mbase.settings->setValue("dbDatabaseName", dbDatabaseNameLineEdit->text());
         mbase.settings->setValue("dbUserName", dbUserNameLineEdit->text());
         mbase.settings->setValue("dbPassword", dbPasswordLineEdit->text());
         mbase.setup_db();
      }


   private:
      QLineEdit * dbHostNameLineEdit;
      QLineEdit * dbDatabaseNameLineEdit;
      QLineEdit * dbUserNameLineEdit;
      QLineEdit * dbPasswordLineEdit;







};

#endif // MSETTINGSDIALOG_H
