// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sslcaselecter.h"
#include "antiquaicon.h"

#include <QApplication>
#include <QFileDialog>
#include <QMetaType>
#include <QRegularExpressionMatch>
#include <QSslConfiguration>
#include <QtCore>

SslCaSelecter::SslCaSelecter(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent}, p_ca_bundle{} {
  // Select
  m_edit = new AntiquaCRM::AComboBox(this);
  m_edit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
  layout->addWidget(m_edit);

  // Open FileDialog
  m_open = new QPushButton(this);
  m_open->setIcon(AntiquaCRM::antiquaIcon("archive-insert-directory"));
  m_open->setToolTip(tr("Set CA Bundle file"));
  m_open->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
  layout->addWidget(m_open);

  m_search = new QLineEdit(this);
  m_search->setDragEnabled(false);
  m_search->setPlaceholderText(tr("Search"));
  m_search->setClearButtonEnabled(true);
  m_search->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
  layout->addWidget(m_search);

  initData();

  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
  connect(m_search, SIGNAL(textChanged(const QString &)),
          SLOT(setSearch(const QString &)));
  connect(m_open, SIGNAL(clicked()), SLOT(setBundleFile()));
}

const QRegularExpression SslCaSelecter::pattern() {
  const QString _prefix("(\\w+[\\-_])?");
  const QString _basename("ca[\\-_](bundle|certificate)");
  const QString _suffix("([\\-_\\.]trust)?");
  return QRegularExpression("^" + _prefix + _basename + _suffix + "$",
                            QRegularExpression::CaseInsensitiveOption);
}

void SslCaSelecter::updateBundle(const QString &path) {
  QFileInfo _fi(path);
  if (!_fi.isReadable())
    return;

  QSslConfiguration sslConfig;
  if (sslConfig.addCaCertificates(_fi.filePath(), QSsl::Pem)) {
    if (p_ca_bundle != _fi.filePath())
      p_ca_bundle = _fi.filePath();

    m_edit->clear();
    QList<QSslCertificate> _list = sslConfig.caCertificates();
    for (int i = 0; i < _list.size(); i++) {
      const QSslCertificate _cert = _list.at(i);
      if (!_cert.isNull()) {
        const QString _ca = _cert.issuerDisplayName();
        m_edit->addItem(_ca, _ca);
      }
    }
    _list.clear();
  }
}

void SslCaSelecter::valueChanged(int) {
  setWindowModified(true);
  emit sendInputChanged();
}

void SslCaSelecter::setSearch(const QString &search) {
  int _index = m_edit->findText(search, Qt::MatchStartsWith);
  if (_index > 0)
    m_edit->setCurrentIndex(_index);
}

void SslCaSelecter::setBundleFile() {
  QString _d = QDir::rootPath();
  QString _t = tr("Open Certfile");
  QString _e = tr("Certificates") + " (*.pem *.ca *.crt *.cert)";
  QString _file = QFileDialog::getOpenFileName(this, _t, _d, _e);
  if (_file.isEmpty())
    return;

  updateBundle(_file);
}

void SslCaSelecter::setValue(const QVariant &value) {
  int _index = m_edit->findData(value, Qt::UserRole, Qt::MatchExactly);
  if (_index > 0)
    m_edit->setCurrentIndex(_index);
}

void SslCaSelecter::setFocus() { m_edit->setFocus(); }

void SslCaSelecter::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SslCaSelecter::initData() {
  QStringList _dirs;
#ifdef Q_OS_LINUX
  _dirs << "/etc/pki/tls/certs";
  _dirs << "/var/lib/ca-certificates";
  _dirs << "/etc/ssl/certs";
#endif

#ifdef Q_OS_WIN
  QString _ap = qApp->applicationDirPath();
  _ap.append(QDir::separator());
  _ap.append("certs");
  _ap.append(QDir::separator());
  _dirs << _ap;
#endif

  QString _bundle = QString();
  QStringList _filter({"*.pem", "*.crt", "*.cert", "*.ca"});
  foreach (QString p, _dirs) {
    foreach (QFileInfo i, QDir(p).entryInfoList(_filter, QDir::Files)) {
      if (i.isReadable() && pattern().match(i.baseName()).isValid()) {
        _bundle = i.filePath();
        break;
      }
    }
    if (!_bundle.isEmpty())
      break;
  }

  if (_bundle.isEmpty())
    return;

  p_ca_bundle = _bundle;
  updateBundle(p_ca_bundle);
}

void SslCaSelecter::setBundle(const QString &path) {
  QFileInfo _fi(path);
  if (!_fi.isReadable())
    return;

  p_ca_bundle = _fi.filePath();
}

const QString SslCaSelecter::getBundle() { return p_ca_bundle; }

const QSslCertificate SslCaSelecter::getCert() {
  QSslCertificate cert;
  if (p_ca_bundle.isEmpty() || getValue().isNull())
    return cert;

  QSslConfiguration sslConfig;
  QList<QSslCertificate> list = sslConfig.caCertificates();
  if (list.size() < 1)
    return cert;

  QString _issuer = getValue().toString();
  for (int i = 0; i < list.size(); i++) {
    QSslCertificate pem = list.at(i);
    if (!pem.isNull() && (pem.issuerDisplayName() == _issuer)) {
      return pem;
    }
  }
  return cert;
}

void SslCaSelecter::setRestrictions(const QSqlField &field) {
  setRequired((field.requiredStatus() == QSqlField::Required));
}

void SslCaSelecter::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void SslCaSelecter::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  AntiquaCRM::ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SslCaSelecter::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QMetaType SslCaSelecter::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant SslCaSelecter::getValue() {
  return m_edit->itemData(m_edit->currentIndex(), Qt::UserRole);
}

const QString SslCaSelecter::popUpHints() {
  return tr("A selected CA Root Bundle is missing.");
}

const QString SslCaSelecter::statusHints() { return popUpHints(); }
