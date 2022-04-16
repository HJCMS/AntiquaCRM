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
  bool modified;

  /**
     @brief Tabellendaten "ref_storage_location"
   */
  void setStorageData();

public Q_SLOTS:
  void setModified(bool b = true);

  /**
     Lade Datenbankdaten in die Auswahlbox
     Wird nach der Klasseninitialisierung
     aufgerufen!
  */
  void loadStorageData();

  /**
    Suche Lager mit ID
  */
  void setValue(const QVariant &val);

  Q_INVOKABLE void reset();

public:
  explicit StorageEdit(QWidget *parent = nullptr);

  Q_INVOKABLE bool hasModified();

  /**
     @brief Identisch mit currentIndex()
     @return sl_id
   */
  const QVariant value();
};

#endif // STORAGEEDIT_H
