// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_CUSTOMER_H
#define INVENTORY_PROVIDERS_CUSTOMER_H

#include <QDialog>
#include <QPair>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTextEdit>

#include <SqlCore>

class ProviderSelectCustomer : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int selected_id = -1;
  HJCMS::SqlCore *m_sql;
  QDialogButtonBox *m_btnBox;
  QTextEdit *m_view;
  QListWidget *m_list;
  QStringList p_fields;
  bool sqlQueryCustomers(const QString &query);
  void addListItem(int cid, const QString &str);

private Q_SLOTS:
  void itemSelected(QListWidgetItem *);

public:
  explicit ProviderSelectCustomer(QWidget *parent = nullptr);
  QPair<int,QString> getSelectedCustomer();
  int exec(const QList<int> &cids);
};

#endif // INVENTORY_PROVIDERS_CUSTOMER_H
