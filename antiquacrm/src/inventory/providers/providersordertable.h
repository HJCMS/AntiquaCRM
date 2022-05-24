// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_TABLE_H
#define INVENTORY_PROVIDERS_TABLE_H

#include <QObject>
#include <QVariant>
#include <QTableWidget>
#include <QTableWidgetItem>

class ProvidersOrderTable : public QTableWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Einen neuen QTableWidgetItem erstellen.
   */
  void addHeaderItem(int, const QString &name);

public:
  ProvidersOrderTable(QWidget *parent = nullptr);
  QTableWidgetItem *createItem(const QString &title) const;
};

#endif // INVENTORY_PROVIDERS_TABLE_H
