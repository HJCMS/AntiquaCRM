// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef WHSOFTREMOTEACTIONS_PLUGIN_H
#define WHSOFTREMOTEACTIONS_PLUGIN_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QObject>
#include <QStackedWidget>
#include <QPushButton>
#include <QStatusBar>
#include <QWidget>

#include <AntiquaInterface>

class WHSoftJSonQuery;

/**
 * @brief Startseite
 */
class ANTIQUACORE_EXPORT BF_StartPage final : public QWidget {
  Q_OBJECT

private:
  QPushButton *btn_status;
  QPushButton *btn_cancel;

private Q_SLOTS:
  void statusClicked();
  void cancelClicked();

Q_SIGNALS:
  void sendGotoPage(int);

public:
  explicit BF_StartPage(QWidget *parent = nullptr);
};

class ANTIQUACORE_EXPORT WHSoftRemoteActions : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QString p_orderId;
  QString p_purchaser;
  QString p_purchaser_mail;

  WHSoftJSonQuery *m_requester;

  QStackedWidget *m_stackedWidget;
  BF_StartPage *m_startPage;

  QDialogButtonBox *m_buttonBar;
  QPushButton *btn_commit;
  QPushButton *btn_quit;
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

public:
  explicit WHSoftRemoteActions(QWidget *parent = nullptr);
  void setPurchaser(const QString &person);
  void setEMail(const QString &email);
  int exec(const QString &orderId);
};

#endif // WHSOFTREMOTEACTIONS_PLUGIN_H
