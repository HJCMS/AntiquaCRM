// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef STORAGEEDIT_UTILS_H
#define STORAGEEDIT_UTILS_H

#include <QtCore/QObject>
#include <QtGui/QFocusEvent>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

#include <UtilsMain>

class StorageEdit : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QComboBox *m_storage;
  QLineEdit *m_search;
  /**
     @brief Tabellendaten "ref_storage_location"
   */
  void setStorageData();

private Q_SLOTS:
  void dataChanged(int);
  void filterChanged(const QString &);

protected:
  void focusInEvent(QFocusEvent *);

public Q_SLOTS:
  /**
     Lade Datenbankdaten in die Auswahlbox
     Wird nach der Klasseninitialisierung
     aufgerufen!
  */
  void loadDataset();

  /**
    Suche Lager mit ID
  */
  void setValue(const QVariant &val);

  Q_INVOKABLE void reset();

public:
  explicit StorageEdit(QWidget *parent = nullptr);
  Q_INVOKABLE bool hasModified();
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // STORAGEEDIT_UTILS_H
