#ifndef MTABLESWIDGETTABLEFILTER_H
#define MTABLESWIDGETTABLEFILTER_H

#include <QWidget>
#include <QBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include "m_base.h"

class mTablesWidgetTableFilter : public QWidget
{
   Q_OBJECT
   public:
      mTablesWidgetTableFilter(QWidget *parent = nullptr, int id = 0)
      {
         // qDebug() << "mTablesWidgetTableFilter::mTablesWidgetTableFilter(QWidget *parent) : QWidget{parent}";

         tableId = id;

         int maxButtonWidthAll = 24;
         int maxButtonWidthNone = 40;
         int maxWidth = 370;

         // top level vbox
         QVBoxLayout *vbox = new QVBoxLayout;
         vbox->setContentsMargins(0,0,0,0);
         this->setLayout(vbox);

         // player filters GroupBox and hbox layout
         QHBoxLayout *pHbox = new QHBoxLayout;
         pGb = new QGroupBox("Player Filter", this);
         pGb->setMaximumWidth(maxWidth);
         pGb->setLayout(pHbox);
         vbox->addWidget(pGb);

         QPushButton * allP  = new QPushButton("all", this);
         QPushButton * noneP = new QPushButton("none", this);
         allP->setMaximumWidth(maxButtonWidthAll);
         noneP->setMaximumWidth(maxButtonWidthNone);

         connect(allP,  SIGNAL (clicked()), this, SLOT (onPAllClicked()));
         connect(noneP, SIGNAL (clicked()), this, SLOT (onPNoneClicked()));
         pHbox->addWidget(allP);
         pHbox->addWidget(noneP);

         for (int i=0; i<8; i++)
         {
            pCb[i] = new QCheckBox(QString::number(i), this);
            connect(pCb[i], SIGNAL (clicked()), this, SLOT (onPCbClicked()));
            pHbox->addWidget(pCb[i]);
         }


         // client filters GroupBox and hbox layout
         QHBoxLayout *cHbox = new QHBoxLayout;
         cGb = new QGroupBox("Client Filter", this);
         cGb->setMaximumWidth(maxWidth);
         cGb->setLayout(cHbox);
         vbox->addWidget(cGb);

         QPushButton * allC  = new QPushButton("all", this);
         QPushButton * noneC = new QPushButton("none", this);
         allC->setMaximumWidth(maxButtonWidthAll);
         noneC->setMaximumWidth(maxButtonWidthNone);


         connect(allC, SIGNAL  (clicked()), this, SLOT (onCAllClicked()));
         connect(noneC, SIGNAL (clicked()), this, SLOT (onCNoneClicked()));
         cHbox->addWidget(allC);
         cHbox->addWidget(noneC);

         for (int i=0; i<8; i++)
         {
            cCb[i] = new QCheckBox(QString::number(i), this);
            connect(cCb[i], SIGNAL (clicked()), this, SLOT (onCCbClicked()));
            cHbox->addWidget(cCb[i]);
         }

         updateCheckBoxesFromArray();

      }

      void updateCheckBoxesFromArray()
      {
         // qDebug() << "void updateCheckBoxesFromArray()";
         for (int i=0; i<8; i++)
         {
            pCb[i]->setChecked(mbase.mTablesWidgetTableFilter[tableId][0][i]);
            cCb[i]->setChecked(mbase.mTablesWidgetTableFilter[tableId][1][i]);
         }
      }

      QGroupBox * pGb;
      QGroupBox * cGb;

   private slots:
      void onPAllClicked()   { for (int i=0; i<8; i++) pCb[i]->setChecked(true);   onPCbClicked();  }
      void onPNoneClicked()  { for (int i=0; i<8; i++) pCb[i]->setChecked(false);  onPCbClicked();  }
      void onPCbClicked()
      {
         for (int i=0; i<8; i++) mbase.mTablesWidgetTableFilter[tableId][0][i] = pCb[i]->checkState();
         mbase.mTablesWidgetTableFiltersChangeFunction();
      }
      void onCAllClicked()   { for (int i=0; i<8; i++) cCb[i]->setChecked(true);   onCCbClicked();  }
      void onCNoneClicked()  { for (int i=0; i<8; i++) cCb[i]->setChecked(false);  onCCbClicked();  }
      void onCCbClicked()
      {
         for (int i=0; i<8; i++) mbase.mTablesWidgetTableFilter[tableId][1][i] = cCb[i]->checkState();
         mbase.mTablesWidgetTableFiltersChangeFunction();
      }

   private:
   int tableId;
   QCheckBox * pCb[8];
   QCheckBox * cCb[8];
};
#endif // MTABLESWIDGETTABLEFILTER_H
