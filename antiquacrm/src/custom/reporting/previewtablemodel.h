// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PREVIEWTABLEMODEL_H
#define PREVIEWTABLEMODEL_H

#include <QObject>
#include <QSqlQueryModel>
#include <QVariant>

class PreviewTableModel final : public QSqlQueryModel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  const QString p_currency;

public:
  explicit PreviewTableModel(QObject *parent = nullptr);
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
};

#endif // PREVIEWTABLEMODEL_H
