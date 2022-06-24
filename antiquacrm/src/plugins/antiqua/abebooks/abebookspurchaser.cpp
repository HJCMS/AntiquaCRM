// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebookspurchaser.h"
#include "abebooksconfig.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QTabWidget>
#include <QVBoxLayout>

AbeBooksPurchaser::AbeBooksPurchaser(QWidget *parent)
    : Antiqua::PurchaserWidget{parent} {
  QStyle *wStyle = style();
  QVBoxLayout *customerLayout = new QVBoxLayout(this);
  QHBoxLayout *h1l = new QHBoxLayout();
  QLabel *anrede = new QLabel(this);
  anrede->setObjectName("c_gender");
  anrede->setIndent(5);
  h1l->addWidget(anrede);
  QLabel *vorname = new QLabel(this);
  vorname->setObjectName("c_firstname");
  h1l->addWidget(vorname);
  QLabel *name = new QLabel(this);
  name->setObjectName("c_lastname");
  h1l->addWidget(name);
  QLabel *email = new QLabel(this);
  email->setObjectName("c_email_0");
  email->setTextInteractionFlags(Qt::TextEditorInteraction);
  email->setIndent(5);
  h1l->addWidget(email);
  h1l->addStretch(1);
  QLabel *info_CustomerId = new QLabel(this);
  info_CustomerId->setIndent(5);
  info_CustomerId->setText(tr("Customer") + ":");
  h1l->addWidget(info_CustomerId);
  QLabel *costumerId = new QLabel(this);
  costumerId->setObjectName("costumerId");
  costumerId->setText("<b>" + tr("Not exists") + "</b>");
  h1l->addWidget(costumerId);
  customerLayout->addLayout(h1l);

  QTabWidget *m_tabWidget = new QTabWidget(this);
  m_tabWidget->setMinimumHeight(100);
  customerLayout->addWidget(m_tabWidget);

  rechnungsadresse = new QTextEdit(this);
  rechnungsadresse->setObjectName("rechnungsadresse");
  m_tabWidget->addTab(rechnungsadresse,
                      wStyle->standardIcon(QStyle::SP_FileIcon),
                      tr("shipping address"));

  lieferadresse = new QTextEdit(this);
  lieferadresse->setObjectName("lieferadresse");
  m_tabWidget->addTab(lieferadresse, wStyle->standardIcon(QStyle::SP_FileIcon),
                      tr("delivery address"));

  QTextEdit *kundenkommentar = new QTextEdit(this);
  kundenkommentar->setObjectName("kundenkommentar");
  kundenkommentar->setReadOnly(true);
  m_tabWidget->addTab(kundenkommentar,
                      wStyle->standardIcon(QStyle::SP_MessageBoxQuestion),
                      tr("Comments"));

  setLayout(customerLayout);
}

void AbeBooksPurchaser::setCustomerId(int customerId) {
  id = customerId;
  setValue("costumerId", QString::number(id));
  emit customerIdChanged(id);
}

void AbeBooksPurchaser::setValue(const QString &objName,
                                 const QVariant &value) {
  if (objName.isEmpty() || value.isNull())
    return;

  QLabel *lb = findChild<QLabel *>(objName, Qt::FindChildrenRecursively);
  if (lb != nullptr) {
    lb->setText(value.toString());
  }
  QTextEdit *tx = findChild<QTextEdit *>(objName, Qt::FindChildrenRecursively);
  if (tx != nullptr) {
    tx->setPlainText(value.toString());
  }
}

const QVariant AbeBooksPurchaser::getValue(const QString &objName) {
  QLabel *lb = findChild<QLabel *>(objName, Qt::FindChildrenRecursively);
  if (lb != nullptr)
    return lb->text();

  QTextEdit *tx = findChild<QTextEdit *>(objName, Qt::FindChildrenRecursively);
  if (tx != nullptr) {
    return tx->toPlainText();
  }

  return QVariant();
}
