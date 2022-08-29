// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKSREMOTEACTIONS_PLUGIN_H
#define ABEBOOKSREMOTEACTIONS_PLUGIN_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QDomDocument>
#include <QObject>
#include <QRadioButton>
#include <QStatusBar>
#include <QWidget>

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

class ANTIQUACORE_EXPORT AbeBooksRemoteActions : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QString p_orderId;
  QString p_purchaser;
  QString p_purchaseId;
  QStringList p_articles;

  AbeBooksRequester *m_requester;

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

  /**
   * @brief responsed Update message
   */
  void responseUpdate(const QDomDocument &dom);

public:
  explicit AbeBooksRemoteActions(QWidget *parent = nullptr);
  void setPurchaser(const QString &person, const QString &purchaseId);
  void setArticleIds(const QStringList &ids);
  int exec(const QString &orderId);
};

#endif // ABEBOOKSREMOTEACTIONS_PLUGIN_H
