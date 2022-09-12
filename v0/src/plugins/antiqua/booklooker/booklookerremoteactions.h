// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKLOOKERREMOTEACTIONS_PLUGIN_H
#define BOOKLOOKERREMOTEACTIONS_PLUGIN_H

#include <QAbstractButton>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QPushButton>
#include <QRadioButton>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTextEdit>

#include <AntiquaInterface>

class BooklookerRequester;

/**
 * @brief Bestellstatus ändern
 */
class ANTIQUACORE_EXPORT Bl_StatusPage final : public QWidget {
  Q_OBJECT

private:
  QRadioButton *m_rb1;
  QRadioButton *m_rb2;
  QRadioButton *m_rb3;
  QRadioButton *m_rb4;
  QRadioButton *m_rb5;
  QRadioButton *m_rb6;
  QRadioButton *m_rb7;
  QLabel *m_orderId;
  QPushButton *m_apply;

private Q_SLOTS:
  void prepareAction();

Q_SIGNALS:
  void sendModified(bool);
  void sendNotes(const QString &message);
  void sendAction(const QJsonObject &status);

public:
  explicit Bl_StatusPage(QWidget *parent = nullptr);
  void setOrderId(const QString &);
};

/**
 * @brief Dialog für Booklooker Bestellung bearbeiten
 */
class ANTIQUACORE_EXPORT BooklookerRemoteActions final : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QString p_orderId;
  QString p_purchaser;

  BooklookerRequester *m_requester;
  QDialogButtonBox *m_buttonBar;
  QPushButton *btn_commit;
  QPushButton *btn_quit;
  Bl_StatusPage *m_statusPage;
  QStatusBar *m_statusBar;

private Q_SLOTS:
  /**
   * @brief prepareAction
   * @code
   * {
   *  "action" : QString,
   *  "type" : QString,
   *  "value" : QString
   * }
   * @endcode
   */
  void prepareAction(const QJsonObject &jsObj);

  /**
   * @brief Status Nachrichten
   */
  void pushMessage(const QString &msg);

  /**
   * @brief Netzwerk antworten
   */
  void jsonResponse(const QJsonDocument &jdoc);

  /**
   * @brief Beenden
   */
  void closeAction();

public:
  explicit BooklookerRemoteActions(QWidget *parent = nullptr);
  void setPurchaser(const QString &person);
  int exec(const QString &orderId);
};

#endif // BOOKLOOKERREMOTEACTIONS_PLUGIN_H