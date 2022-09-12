// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef EDITCOSTUMER_H
#define EDITCOSTUMER_H

#include <QObject>
#include <QRegularExpression>
#include <QLabel>
#include <QToolBox>
#include <QWidget>

#include <EditorMain>

class SerialID;
class CustomerContact;
class CustomerOverview;
class CustomerBillingInfo;
class EditorActionBar;

class EditCustomer : public EditorMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *infoLabel;
  QToolBox *m_dataBox;
  SerialID *c_id;
  CustomerOverview *m_overview;
  CustomerContact *m_contact;
  CustomerBillingInfo *m_billing;
  EditorActionBar *m_actionBar;

  /**
     @brief Wird für QObject::findchild benötigt!
     Dieser Reguläre Ausdruck wird verwendet um die Eingabe-Objektklassen
     zu finden. Sie sind Identisch mit den SQL Feldern und beginnen bei
     der Tabelle "customers" immer mit "c_".
  */
  const QRegularExpression p_objPattern = QRegularExpression("^c_[a-z0-9_]+$");

  Qt::FindChildOptions findOption = (Qt::FindChildrenRecursively);

  /**
   * @brief Erstelle QStringList::inputList
   */
  void setInputList();

  void importSqlResult();
  bool sendSqlQuery(const QString &);
  const QHash<QString, QVariant> createSqlDataset();
  void createSqlUpdate();
  void createSqlInsert();
  void setData(const QString &key, const QVariant &value, bool required = false);

private Q_SLOTS:
  void saveData();
  void checkLeaveEditor();
  void finalLeaveEditor();
  /**
   * @brief Öffnen E-Mail Nachrichten Dialog
   */
  void openEMailDialog(const QString &tpl);

public Q_SLOTS:
  void restoreDataset();
  void updateCustomer(const QString &statement);
  void createCustomer();

public:
  explicit EditCustomer(QWidget *parent = nullptr);
};

#endif // EDITCOSTUMER_H
