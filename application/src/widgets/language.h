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
 * @ingroup widgets
 */
class Language final : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_comboBox;
  void loadDataset() override;

private Q_SLOTS:
  void itemChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &) override;
  Q_INVOKABLE void reset() override;
  void setFocus() override;

public:
  explicit Language(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_LANGUAGE_H
