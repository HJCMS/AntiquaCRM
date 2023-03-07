// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_EMAILEDIT_H
#define ANTIQUACRM_EMAILEDIT_H

#include <AntiquaInputEdit>
#include <QRegExpValidator>

/**
 * @brief The EMailEdit class
 * @ingroup widgets
 */
class EMailEdit final : public InputEdit {
  Q_OBJECT

private:
  AntiquaLineEdit *m_edit;
  QRegExpValidator *m_validator;
  void loadDataset() override{};
  bool validate(const QString &mail) const;

private Q_SLOTS:
  void dataChanged(const QString &);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  explicit EMailEdit(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_EMAILEDIT_H
