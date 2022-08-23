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
 * @brief Nachrichten
 */
class ANTIQUACORE_EXPORT BL_Message final {
private:
  QString p_type;
  QString p_title;
  QString p_body;

public:
  explicit BL_Message();
  void setType(const QString &);
  const QString getType();
  void setTitle(const QString &);
  const QString getTitle();
  void setBody(const QString &);
  const QString getBody();
};

/**
 * @brief Abstrakte Klasse für die Seiten
 */
class ANTIQUACORE_EXPORT Bl_PageWidget : public QWidget {
  Q_OBJECT
  Q_PROPERTY(bool modified WRITE setModified READ isModified NOTIFY hasModified)

private:
  bool modified = false;

protected Q_SLOTS:
  void setModified(bool b);
  virtual void prepareAction() = 0;

Q_SIGNALS:
  void sendNotes(const QString &message);
  void sendAction(const QJsonObject &status);
  void hasModified(bool);

public:
  explicit Bl_PageWidget(QWidget *parent = nullptr);
  bool isModified();
};

/**
 * @brief Bestellstatus ändern
 */
class ANTIQUACORE_EXPORT Bl_StatusPage final : public Bl_PageWidget {
  Q_OBJECT

private:
  QRadioButton *m_rb1;
  QRadioButton *m_rb2;
  QRadioButton *m_rb3;
  QRadioButton *m_rb4;
  QRadioButton *m_rb5;
  QRadioButton *m_rb6;
  QPushButton *m_apply;

private Q_SLOTS:
  void prepareAction();

public:
  explicit Bl_StatusPage(QWidget *parent = nullptr);
};

/**
 * @brief Nachricht an Käufer senden
 */
class ANTIQUACORE_EXPORT Bl_MessagePage final : public Bl_PageWidget {
  Q_OBJECT

private:
  QComboBox *m_type;
  QTextEdit *m_message;
  QPushButton *m_apply;
  QMap<QString, QString> p_replacements;
  QMap<int, BL_Message> p_messages;
  const QString convertTemplate(const QString &body);

private Q_SLOTS:
  void messageTypeChanged(int);
  void prepareAction();

public:
  explicit Bl_MessagePage(QWidget *parent = nullptr);
  void setPurchaser(QString &person, const QString &orderId);
  bool initSqlMessages();
};

/**
 * @brief Startseite
 */
class ANTIQUACORE_EXPORT Bl_StartPage final : public QWidget {
  Q_OBJECT

private:
  QPushButton *btn_status;
  QPushButton *btn_message;

private Q_SLOTS:
  void statusClicked();
  void messageClicked();

Q_SIGNALS:
  void sendGotoPage(int);

public:
  explicit Bl_StartPage(QWidget *parent = nullptr);
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
  QStackedWidget *m_stackedWidget;
  Bl_StartPage *m_startPage;
  Bl_StatusPage *m_statusPage;
  Bl_MessagePage *m_messagePage;
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
  explicit BooklookerRemoteActions(QWidget *parent = nullptr);
  void setPurchaser(const QString &person);
  int exec(const QString &orderId);
};

#endif // BOOKLOOKERREMOTEACTIONS_PLUGIN_H
