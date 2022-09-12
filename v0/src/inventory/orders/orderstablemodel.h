// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ORDERSTABLEMODEL_H
#define ORDERSTABLEMODEL_H

#include <QDate>
#include <QModelIndex>
#include <QObject>
#include <QSqlQueryModel>
#include <QSqlRelation>
#include <QString>
#include <QVariant>

class OrdersTableModel : public QSqlQueryModel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann (Undefined)")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Zeigt das Datum in Benutzersprache an.
   */
  const QString displayDate(const QVariant &value) const;

  /**
   * @brief Laufzeit Berechnung für den Auftrag
   * Konvertiert die Sekundenlaufzeit eines Auftrages
   * in eine Benutzerfreundliche ansicht.
   * @note Ist abhängig von der SQL Abfrage!
   * @code
   *  EXTRACT(EPOCH FROM timestamptz (CURRENT_TIMESTAMP)) - EXTRACT(EPOCH FROM
   * timestamptz (from_field_timestamp)) AS age
   * @endcode
   * @param seconds
   */
  const QString runTimeString(const qint64 &seconds) const;

private Q_SLOTS:
  /**
   * @brief Prüft Auf Datenänderungen und sendet Signal dataUpdated
   */
  void update(const QModelIndex &, const QModelIndex &);

Q_SIGNALS:
  /**
   * @brief Sendet Datenänderungen an parent
   */
  void dataUpdated(bool);

public:
  explicit OrdersTableModel(QObject *parent = nullptr);
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
};

#endif // ORDERSTABLEMODEL_H
