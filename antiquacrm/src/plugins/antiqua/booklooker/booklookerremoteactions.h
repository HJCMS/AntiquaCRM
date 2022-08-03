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
#include <QObject>
#include <QPushButton>
#include <QRadioButton>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTextEdit>

#include <AntiquaInterface>

/**
 * @brief Abstrakte Klasse für die Seiten
 */
class ANTIQUACORE_EXPORT Bl_PageWidget : public QWidget {
  Q_OBJECT
  Q_PROPERTY(bool modified WRITE setModified READ isModified NOTIFY hasModified)

private:
  bool modified = false;

protected Q_SLOTS:
  void setModified(bool b) {
    modified = b;
    emit hasModified(b);
  };
  virtual void prepareAction() = 0;

Q_SIGNALS:
  void sendAction(const QString &action, const QString &status);
  void hasModified(bool);

public:
  explicit Bl_PageWidget(QWidget *parent = nullptr) : QWidget{parent} {};
  bool isModified() { return modified; };
};

/**
 * @brief Startseite
 */
class ANTIQUACORE_EXPORT Bl_StartPage final : public QWidget {
  Q_OBJECT

private:
  QPushButton *btn_status;
  QPushButton *btn_email;
  QPushButton *btn_cancel;
  QPushButton *btn_message;

private Q_SLOTS:
  void statusClicked();
  void emailClicked();
  void cancelClicked();
  void messageClicked();

Q_SIGNALS:
  void sendGotoPage(int);

public:
  explicit Bl_StartPage(QWidget *parent = nullptr);
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
 * @brief Bestellung Stornieren
 */
class ANTIQUACORE_EXPORT Bl_CancelPage final : public Bl_PageWidget {
  Q_OBJECT

private Q_SLOTS:
  void prepareAction();

public:
  explicit Bl_CancelPage(QWidget *parent = nullptr);
};

/**
 * @brief E-Mail Körper erstellen
 */
class ANTIQUACORE_EXPORT Bl_EMailPage final : public Bl_PageWidget {
  Q_OBJECT

private Q_SLOTS:
  void prepareAction();

public:
  explicit Bl_EMailPage(QWidget *parent = nullptr);
};

/**
 * @brief Nachricht an Käufer senden
 */
class ANTIQUACORE_EXPORT Bl_MessagePage final : public Bl_PageWidget {
  Q_OBJECT

private:
  QComboBox *m_type;
  QComboBox *m_preDefTxt;
  QTextEdit *m_message;

private Q_SLOTS:
  void prepareAction();

public:
  explicit Bl_MessagePage(QWidget *parent = nullptr);
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
  QDialogButtonBox *m_buttonBar;
  QPushButton *btn_commit;
  QPushButton *btn_quit;
  QStackedWidget *m_stackedWidget;
  Bl_StartPage *m_startPage;
  Bl_StatusPage *m_statusPage;
  Bl_CancelPage *m_cancelPage;
  Bl_EMailPage *m_emailPage;
  Bl_MessagePage *m_messagePage;
  QStatusBar *m_statusBar;

private Q_SLOTS:
  void prepareAction(const QString &action, const QString &value);

public:
  explicit BooklookerRemoteActions(QWidget *parent = nullptr);
  int exec(const QString &orderId);
};

#endif // BOOKLOOKERREMOTEACTIONS_PLUGIN_H
