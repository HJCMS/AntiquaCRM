// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "assistant.h"
#include "config_p.h"
#include "configpgsql.h"
#include "configssl.h"
#include "infopage.h"
#include "testconfig.h"

#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSslConfiguration>
#include <QVBoxLayout>

static const QString my_domain() {
  QString str = QString::fromUtf8(HJCMS_ASSISTANT_DOMAIN);
  str.append(QDir::separator());
  str.append(QString::fromUtf8(HJCMS_PROJECT_NAME));
  return str;
}

static const QString my_app() { return QString::fromUtf8(HJCMS_PROJECT_NAME); }

Assistant::Assistant(QWidget *parent) : QDialog(parent) {
  setObjectName("assistant_dialog");
  setWindowTitle(HJCMS_HOMEPAGE);
  setSizeGripEnabled(true);
  setMinimumSize(600, 400);

  Q_INIT_RESOURCE(documents);

  cfg = new QSettings(QSettings::NativeFormat, QSettings::UserScope,
                      my_domain(), my_app(), this);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_stackedWidget = new QStackedWidget(this);
  layout->addWidget(m_stackedWidget);

  m_page0 = new InfoPage(m_stackedWidget);
  m_page0->setObjectName("page0");
  m_page0->setContent("Welcome");
  m_stackedWidget->insertWidget(0, m_page0);

  m_page1 = new InfoPage(m_stackedWidget);
  m_page1->setObjectName("page1");
  m_page1->setContent("Database");
  m_stackedWidget->insertWidget(1, m_page1);

  m_configPgSQL = new ConfigPgSQL(this);
  m_configPgSQL->setObjectName("page2");
  m_stackedWidget->insertWidget(2, m_configPgSQL);

  m_configSSL = new ConfigSSL(this);
  m_configSSL->setObjectName("page3");
  m_stackedWidget->insertWidget(3, m_configSSL);

  m_lastTest = new TestConfig(this);
  m_lastTest->setObjectName("page4");
  m_stackedWidget->insertWidget(4, m_lastTest);

  m_buttonBox = new QDialogButtonBox(this);
  setButtonBox();
  layout->addWidget(m_buttonBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);

  // Signals
  connect(m_stackedWidget, SIGNAL(currentChanged(int)), this,
          SLOT(pageEntered(int)));
  connect(this, SIGNAL(accepted()), this, SLOT(beforeAccept()));
  connect(this, SIGNAL(rejected()), this, SLOT(beforeClose()));
  connect(m_lastTest, SIGNAL(startTest()), this, SLOT(test()));
}

void Assistant::setButtonBox() {
  m_buttonBox->setOrientation(Qt::Horizontal);
  QPushButton *btn_restore =
      m_buttonBox->addButton(QDialogButtonBox::RestoreDefaults);
  btn_restore->setText(tr("Restart"));
  btn_restore->setStatusTip(tr("Restart Assistant"));
  btn_restore->setObjectName("restore_button");
  connect(btn_restore, SIGNAL(clicked()), this, SLOT(restart()));

  QPushButton *btn_prev =
      m_buttonBox->addButton(tr("Back"), QDialogButtonBox::ActionRole);
  btn_prev->setStatusTip(tr("Previous page"));
  btn_prev->setObjectName("previous_page_button");
  connect(btn_prev, SIGNAL(clicked()), this, SLOT(previousPage()));

  QPushButton *btn_next =
      m_buttonBox->addButton(tr("Next"), QDialogButtonBox::ActionRole);
  btn_next->setStatusTip(tr("Next page"));
  btn_next->setObjectName("next_page_button");
  connect(btn_next, SIGNAL(clicked()), this, SLOT(nextPage()));

  QPushButton *btn_accept = m_buttonBox->addButton(QDialogButtonBox::Save);
  btn_accept->setStatusTip(tr("Save current changes"));
  btn_accept->setObjectName("accept_button");
  connect(btn_accept, SIGNAL(clicked()), this, SLOT(save()));

  btn_close = m_buttonBox->addButton(QDialogButtonBox::Apply);
  btn_close->setText(tr("Finalize"));
  btn_close->setStatusTip(tr("Finishing and close the assistant"));
  btn_close->setObjectName("close_button");
  btn_close->setEnabled(false);
  connect(btn_close, SIGNAL(clicked()), this, SLOT(beforeClose()));
}

bool Assistant::connectionTest() {
  QMap<QString, QString> db = m_configPgSQL->configuration();
  QMap<QString, QString> ssl = m_configSSL->configuration();
  QSqlDatabase psql = QSqlDatabase::addDatabase("QPSQL", my_domain());
  psql.setHostName(db.value("hostname"));
  psql.setDatabaseName(db.value("database"));
  psql.setPort(db.value("port").toInt());
  psql.setUserName(db.value("username"));
  psql.setPassword(db.value("password"));
  bool requiressl = (m_configSSL->getCA().isNull()) ? false : true;
  if (requiressl && ssl.size() >= 3) {
    qDebug() << Q_FUNC_INFO << "SSL Enabled";
    // https://www.postgresql.org/docs/current/libpq-connect.html
    QSslConfiguration sslconf;
    sslconf.addCaCertificate(m_configSSL->getCA());
    QStringList options("connect_timeout=5");
    options.append("sslmode=verify-ca");
    psql.setConnectOptions(options.join(";"));
  }
  if (psql.open()) {
    QString result = tr("Connection successfully!");
    btn_close->setEnabled(true);
    m_statusBar->showMessage(result);
    m_lastTest->setBodyText(result);
    return true;
  } else {
    p_lastError = psql.lastError().text();
    m_lastTest->setBodyText(p_lastError);
    qDebug() << Q_FUNC_INFO << p_lastError;
  }
  return false;
}

int Assistant::index() { return m_stackedWidget->currentIndex(); }

void Assistant::goTo(int index) { m_stackedWidget->setCurrentIndex(index); }

void Assistant::restart() {
  if (index() > 0)
    goTo(0);

  unsaved = false;
}

void Assistant::previousPage() {
  if (index() > 0)
    goTo((index() - 1));

  unsaved = true;
}

void Assistant::nextPage() {
  if (index() >= 0 && index() < m_stackedWidget->count())
    goTo((index() + 1));

  unsaved = true;
}

void Assistant::beforeAccept() { save(); }

void Assistant::beforeClose() {
  unsaved = true;
  save();
  accept();
}

void Assistant::test() {
  save();
  connectionTest();
}

void Assistant::save() {
  QMapIterator<QString, QString> sql(m_configPgSQL->configuration());
  cfg->beginGroup("postgresql");
  while (sql.hasNext()) {
    sql.next();
    if(sql.value().isNull())
      continue;

    cfg->setValue(sql.key(), sql.value());
    if (sql.key() == "password") {
      QByteArray pw =
          sql.value().toLocal8Bit().toBase64(QByteArray::Base64Encoding);
      cfg->setValue(sql.key(), pw);
    }
  }
  cfg->endGroup();

  QMapIterator<QString, QString> ssl(m_configSSL->configuration());
  cfg->beginGroup("ssloptions");
  while (ssl.hasNext()) {
    ssl.next();
    if(sql.value().isNull())
      continue;

    cfg->setValue(ssl.key(), ssl.value());
  }
  cfg->endGroup();

  unsaved = false;
}

void Assistant::pageEntered(int index) {
  // SQL Daten Laden
  if (m_stackedWidget->widget(index)->objectName() == "page2") {
    cfg->beginGroup("postgresql");
    foreach (QString k, cfg->allKeys()) {
      if (k.contains("password"))
        continue;

      QPair<QString, QVariant> data;
      data.first = k;
      data.second = cfg->value(k);
      m_configPgSQL->setData(data);
    }
    cfg->endGroup();
  } else if (m_stackedWidget->widget(index)->objectName() == "page3") {
    cfg->beginGroup("ssloptions");
    foreach (QString k, cfg->allKeys()) {
      if (k.contains("ssl_peer_pass"))
        continue;

      QPair<QString, QVariant> data;
      data.first = k;
      data.second = cfg->value(k);
      m_configSSL->setData(data);
    }
    cfg->endGroup();
  }
}

bool Assistant::event(QEvent *e) {
  if (e->type() == QEvent::StatusTip) {
    QStatusTipEvent *t = static_cast<QStatusTipEvent *>(e);
    if (t->tip().isEmpty())
      return false;

    m_statusBar->showMessage(t->tip(), 1000);
    return true;
  }
  return QDialog::event(e);
}

void Assistant::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Return) {
    qInfo("Key press enter or return will ignored!");
    return;
  }
  QDialog::keyPressEvent(e);
}

void Assistant::closeEvent(QCloseEvent *e) {
  if (e->type() == QEvent::Close) {
    if (unsaved) {
      int retval =
          QMessageBox::warning(this, tr("Unfinished Assistant"),
                               tr("Do not close the Assistant without saving!"),
                               (QMessageBox::Ok | QMessageBox::Ignore));
      if (retval == QMessageBox::Ok) {
        e->ignore();
        return;
      }
    }
  }
  QDialog::closeEvent(e);
}

Assistant::~Assistant() {}
