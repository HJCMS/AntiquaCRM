// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SELECTMEDIATYPE_H
#define ANTIQUACRM_SELECTMEDIATYPE_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QMap>

class SelectMediaType final : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_box;
  void loadDataset();

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit SelectMediaType(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();

  static const QMap<AntiquaCRM::MediaType,QString> list();
};

#endif // ANTIQUACRM_SELECTMEDIATYPE_H
