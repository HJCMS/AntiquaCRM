// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef EDITCUSTOMER_H
#define EDITCUSTOMER_H

#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QWidget>

class ArticleID;
class CustomerAdditional;
class CustomerContact;
class CustomerOverview;
class CustomerBillingInfo;

class EditCustomer : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *infoLabel;
  QToolBox *m_dataBox;
  ArticleID *c_id;
  CustomerOverview *m_overview;
  CustomerContact *m_contact;
  CustomerBillingInfo *m_billing;
  CustomerAdditional *m_additional;

Q_SIGNALS:

public Q_SLOTS:

public:
  explicit EditCustomer(QWidget *parent = nullptr);
};

#endif // EDITCUSTOMER_H
