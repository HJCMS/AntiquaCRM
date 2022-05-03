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
class CostumerContact;
class CostumerOverview;
class CostumerBillingInfo;
class EditorActionBar;

class EditCostumer : public EditorMain {
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
  EditorActionBar *m_actionBar;

  /**
     @brief Wird für QObject::findchild benötigt!
     Dieser Reguläre Ausdruck wird verwendet um die Eingabe-Objektklassen
     zu finden. Sie sind Identisch mit den SQL Feldern und beginnen bei
     der Tabelle "costumers" immer mit "c_".
  */
  const QRegularExpression p_objPattern = QRegularExpression("^c_[a-z_]+\\b$");

  const QString tableName = "costumers";

  Qt::FindChildOptions findOption = (Qt::FindChildrenRecursively);

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

public Q_SLOTS:
  void restoreDataset();
  void updateCostumer(const QString &);
  void createCostumer();

public:
  explicit EditCostumer(QWidget *parent = nullptr);
};

#endif // EDITCOSTUMER_H
