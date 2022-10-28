// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_VIEWSTABLEMODEL_H
#define ANTIQUACRM_VIEWSTABLEMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QWidget>

class ViewsTableModel : public QSqlTableModel {
  Q_OBJECT

private:
  const QString ucFirst(const QString &title) const;

public:
  explicit ViewsTableModel(const QSqlDatabase &db, QObject *parent = nullptr);
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  const QString getLabelText();
};

#endif // ANTIQUACRM_VIEWSTABLEMODEL_H
