// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_DBINFO_H
#define ANTIQUACRM_DBINFO_H

#include <QDialog>
#include <QGridLayout>
#include <QObject>
#include <QWidget>

class AntiquaDbInfo final : public QDialog {
  Q_OBJECT

private:
  qint8 row_count = 0;
  QGridLayout *m_layout;
  void addRow(const QString &title, QString &value);

public:
  explicit AntiquaDbInfo(QWidget *parent = nullptr);
  int exec() override;
};

#endif // ANTIQUACRM_DBINFO_H
