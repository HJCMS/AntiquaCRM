// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef STORAGEEDIT_H
#define STORAGEEDIT_H

#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QWidget>

class StorageEdit : public QComboBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  /**
     @brief Tabellendaten "ref_storage_location"
   */
  void setStorageData();

public Q_SLOTS:
  void setValue(const QVariant &);

public:
  explicit StorageEdit(QWidget *parent = nullptr);

  /**
     @brief Identisch mit currentIndex()
     @return sl_id
   */
  const QVariant value();
};

#endif // STORAGEEDIT_H
