// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_LANGUAGE_H
#define ANTIQUACRM_LANGUAGE_H

#include <AntiquaInputEdit>
#include <QHash>
#include <QList>
#include <QObject>
#include <QVariant>
#include <QWidget>

/**
 * @brief ISO639-1 Language Code
 */
class Language final : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_comboBox;
  void loadDataset();

private Q_SLOTS:
  void itemChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit Language(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_LANGUAGE_H
