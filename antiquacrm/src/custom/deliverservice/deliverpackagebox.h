// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DELIVERPACKAGEBOX_H
#define DELIVERPACKAGEBOX_H

#include <QComboBox>
#include <QWheelEvent>
#include <QWidget>
#include <SqlCore>

/**
 * @brief Auswahl der Paketzusteller Angebote
 * @ingroup DeliverService
 * Die Daten werden aus Tabelle "ref_delivery_cost" gelesen.
 */
class DeliverPackageBox final : public QComboBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;

protected:
  /**
   * @brief Ungewollte fehlerhafte Eingabe verhindern.
   */
  void wheelEvent(QWheelEvent *) override {};

public Q_SLOTS:
  /**
   * @brief setCurrentService
   * Suche Daten mit FOREIGN KEY ("delivery_id")
   */
  void setCurrentService(int id);

Q_SIGNALS:
  /**
   * @brief Eine gültige Dienstleister Paketliste wurde ausgewählt.
   */
  void validServiceChanged(bool);

public:
  explicit DeliverPackageBox(QWidget *parent = nullptr);
  qreal getPackagePrice(int);
};

#endif // DELIVERPACKAGEBOX_H
