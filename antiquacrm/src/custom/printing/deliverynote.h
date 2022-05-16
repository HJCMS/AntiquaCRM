// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DELIVERYNOTE_PRINTING_H
#define DELIVERYNOTE_PRINTING_H

#include <QHash>
#include <QMetaType>
#include <QObject>
#include <QWidget>

#include "printing.h"

class ApplSettings;

class DeliveryNote : public Printing {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QString p_orderId;
  QString p_costumerId;
  QString p_deliveryId;
  QString p_costumerAddress;
  ApplSettings *config;
  QHash<QString, QString> companyData;
  QTextTable *m_articleTable;
  QTextTableCell p_infoCell;

  void readConfiguration();
  void constructHeader();
  void constructAddress();
  void constructBody();
  void constructFooter();

  void insertArticle(const QString &articleid, const QString &designation,
                     const QString &quantity);

  QTextDocument *docHeader();
  const QString htmlHeader();

  QTextDocument *docBody();
  const QString htmlBody();

  QTextDocument *docFooter();
  const QString htmlFooter();

private Q_SLOTS:
  void printToFile(QPrinter *printer) { /* not needed */ };
  void printDocument(QPrinter *printer);
  void saveDeliveryNote();
  void printDeliveryNote();

public Q_SLOTS:
  int warningMessageBox(const QString &);

public:
  struct Delivery {
    QString articleid;   /**< Article ID */
    QString designation; /**< Designation */
    QString quantity;    /**< Quantity */
  };
  explicit DeliveryNote(QWidget *parent = nullptr);

  /**
   * @brief Nummer für PDF Dateiname
   */
  const QString deliveryNumber();

  /**
   * @brief Add Costumer Address
   */
  void setCostumerAddress(const QString &);

  void setDelivery(int &orderId, int &costumerId);

  int exec(const QList<Delivery> &);
};
Q_DECLARE_METATYPE(DeliveryNote::Delivery);

#endif // DELIVERYNOTE_PRINTING_H
