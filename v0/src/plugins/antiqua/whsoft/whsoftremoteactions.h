// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef WHSOFTREMOTEACTIONS_PLUGIN_H
#define WHSOFTREMOTEACTIONS_PLUGIN_H

#include <QComboBox>
#include <QDateTimeEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QStackedWidget>
#include <QStatusBar>
#include <QWidget>

#include <AntiquaInterface>

class WHSoftJSonQuery;

class ANTIQUACORE_EXPORT WHSoftRemoteActions : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QString p_orderId;
  WHSoftJSonQuery *m_requester;
  QWidget *m_mainWidget;
  QPushButton *btn_status;
  QLabel *m_orderId;
  QComboBox *m_supplier;
  QLabel *m_shippingMethod;
  QLineEdit *m_trackingNumber;
  QDateTimeEdit *m_deliveryDate;
  QDialogButtonBox *m_buttonBar;
  QPushButton *btn_commit;
  QPushButton *btn_quit;
  QStatusBar *m_statusBar;

  bool initDatabaseSetup();

private Q_SLOTS:
  /**
   * @brief Erstelle Lieferinformationen
   * @code
   * {
   *   "id": string,
   *   "versender" : string (32),
   *   "sendungsart" : string (255),
   *   "sendungsnummer" : string (255)|null,
   *   "einlieferungsdatum" : string (Format: YYYY-mm-dd HH:ii:ss)
   * }
   * @endcode
   */
  void prepareAction();

  /**
   * @brief Status Nachrichten
   */
  void pushMessage(const QString &msg);

  /**
   * @brief Netzwerk antworten
   */
  void jsonResponse(const QJsonDocument &jdoc);

Q_SIGNALS:
  void sendUpdateStatus(const QJsonObject &jobj);

public:
  explicit WHSoftRemoteActions(QWidget *parent = nullptr);
  int exec(const QString &orderId);
};

#endif // WHSOFTREMOTEACTIONS_PLUGIN_H
