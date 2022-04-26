// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef EDITCOSTUMER_H
#define EDITCOSTUMER_H

#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QWidget>

class SerialID;
class CostumerAdditional;
class CostumerContact;
class CostumerOverview;
class CostumerBillingInfo;

class EditCostumer : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *infoLabel;
  QToolBox *m_dataBox;
  SerialID *c_id;
  CostumerOverview *m_overview;
  CostumerContact *m_contact;
  CostumerBillingInfo *m_billing;
  CostumerAdditional *m_additional;

Q_SIGNALS:

public Q_SLOTS:

public:
  explicit EditCostumer(QWidget *parent = nullptr);
};

#endif // EDITCOSTUMER_H
