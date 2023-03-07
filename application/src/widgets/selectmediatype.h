// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SELECTMEDIATYPE_H
#define ANTIQUACRM_SELECTMEDIATYPE_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QMap>

/**
 * @brief The SelectMediaType class
 * @ingroup widgets
 */
class SelectMediaType final : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_box;
  void loadDataset() override;

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  explicit SelectMediaType(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;

  static const QMap<AntiquaCRM::MediaType,QString> list();
};

#endif // ANTIQUACRM_SELECTMEDIATYPE_H
