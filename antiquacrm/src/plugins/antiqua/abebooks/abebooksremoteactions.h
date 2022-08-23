// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKSREMOTEACTIONS_PLUGIN_H
#define ABEBOOKSREMOTEACTIONS_PLUGIN_H

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QRadioButton>
#include <QStackedWidget>
#include <QDialogButtonBox>
#include <QStatusBar>

#include <AntiquaInterface>

class AbeBooksRequester;

class ANTIQUACORE_EXPORT ABE_StatusPage final : public QWidget {
  Q_OBJECT

private:
  QRadioButton *m_rb1;
  QRadioButton *m_rb2;
  QRadioButton *m_rb3;
  QRadioButton *m_rb4;
  QRadioButton *m_rb5;
  QRadioButton *m_rb6;
  QPushButton *m_apply;

Q_SIGNALS:
  void sendNotes(const QString &message);
  void sendAction(const QString &nodeValue);

private Q_SLOTS:
  void prepareAction();

public:
  explicit ABE_StatusPage(QWidget *parent = nullptr);
};

/**
 * @brief Startseite
 */
class ANTIQUACORE_EXPORT ABE_StartPage final : public QWidget {
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
  explicit ABE_StartPage(QWidget *parent = nullptr);
};

class ANTIQUACORE_EXPORT AbeBooksRemoteActions : public QDialog
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QString p_orderId;
  QString p_purchaser;
  QString p_purchaser_mail;

  AbeBooksRequester *m_requester;

  QStackedWidget *m_stackedWidget;
  ABE_StartPage *m_startPage;
  ABE_StatusPage *m_statusPage;

  QDialogButtonBox *m_buttonBar;
  QPushButton *btn_commit;
  QPushButton *btn_quit;
  QStatusBar *m_statusBar;

private Q_SLOTS:
  /**
   * @brief orderUpdateStatus erstellen
   */
  void prepareStatusAction(const QString &cmd);

  /**
   * @brief Status Nachrichten
   */
  void pushMessage(const QString &msg);

public:
  explicit AbeBooksRemoteActions(QWidget * parent = nullptr);
  void setPurchaser(const QString &person);
  void setEMail(const QString &email);
  int exec(const QString &orderId);
};

#endif // ABEBOOKSREMOTEACTIONS_PLUGIN_H
