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
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(bool required READ isRequired WRITE setRequired NOTIFY requireChanged);

private:
  bool required = false;
  bool modified;

  /**
     @brief Tabellendaten "ref_storage_location"
   */
  void setStorageData();

private Q_SLOTS:
  void dataChanged(int);

Q_SIGNALS:
  void requireChanged();

public Q_SLOTS:
  Q_INVOKABLE void setModified(bool b = true);

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
  void setRequired(bool b);
  bool isRequired();
  Q_INVOKABLE bool hasModified();
  const QVariant value();
  bool isValid();
  const QString notes();
};

#endif // STORAGEEDIT_H
